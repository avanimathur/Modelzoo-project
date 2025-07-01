# /*---------------------------------------------------------------------------------------------
#  * Copyright (c) 2022 STMicroelectronics.
#  * All rights reserved.
#  *
#  * This software is licensed under terms that can be found in the LICENSE file in
#  * the root directory of this software component.
#  * If no LICENSE file comes with this software, it is provided AS-IS.
#  *--------------------------------------------------------------------------------------------*/

import tensorflow as tf
from keras.models import Model
import numpy as np


def _choose_tensors_when_multiple_outputs(layer_input_tensor, layer_input_signature):

    layer_input_selection = []
    list_signature_names = []

    if type(layer_input_signature) is list:
        # print('layer in signature first input {}'.format(layer_input_signature[0].name))
        for elem in layer_input_signature:
            if hasattr(elem, 'name'):
                list_signature_names.append(elem.name)
    else:
        list_signature_names = [layer_input_signature.name]

    for elem in layer_input_tensor:
        if type(elem) is tuple:
            for sub_elem in elem:
                if sub_elem.name in list_signature_names:
                    layer_input_selection.append(sub_elem)
        else:
            layer_input_selection.append(elem)

    return layer_input_selection

    
def insert_layer_in_graph(model, layer_list, insert_layer, inv_scale, insert_layer_name=None, position='replace'):
    """
        Returns a model where some layers (layer_List) have been replaced by a new layer type 'insert_layer' with
        as parameter an element of 'inv_scale'

        Args:
            model: keras model after CLE and bias absorption
            layer_list: list of layer names we want to replace in the graph
            insert_layer: the layer we want to insert in replacement in the graph
            inv_scale: inverse of 's' in Nagel's paper. Equalisation coefficient
            insert_layer_name: name of the layer we insert. Not used at the moment
            position: could be 'replace', 'after', 'before'. Always 'replace' for CLE

        Returns: a keras model with specified layers replaced by new insert_layer
    """

    # early exit
    if not layer_list:
        return model
    # Auxiliary dictionary to describe the network graph
    network_dict = {'input_layers_of': {}, 'new_output_tensor_of': {}}

    # Set the input layers of each layer
    for layer in model.layers:
        for node in layer.outbound_nodes:
            layer_name = node.outbound_layer.name
            if layer_name not in network_dict['input_layers_of']:
                network_dict['input_layers_of'].update(
                        {layer_name: [layer.name]})
            else:
                # condition added Jan16, 2024, because due to duplication of a tensor in outbound_nodes (2 same tensors
                # instead of 1 expected), unexplained, causing issues later on at conversion/evaluation
                # by having this condition we may lose some generality in some corner cases, but should be rare
                if layer.name not in network_dict['input_layers_of'][layer_name]:
                    network_dict['input_layers_of'][layer_name].append(layer.name)

    # Set the output tensor of the input layer
    network_dict['new_output_tensor_of'].update({model.layers[0].name: model.input})

    # Iterate over all layers after the input
    model_outputs = []
    count_scale = 0

    for layer in model.layers[1:]:
        # Determine input tensors
        layer_input = [network_dict['new_output_tensor_of'][layer_aux]
                       for layer_aux in network_dict['input_layers_of'][layer.name]]
        layer_input = _choose_tensors_when_multiple_outputs(layer_input, layer.input)

        if len(layer_input) == 1:
            layer_input = layer_input[0]
            nb_inputs_layer = 1
        else:
            nb_inputs_layer = len(layer_input)

        # Insert layer if name matches the regular expression
        if layer.name in layer_list:
            if position == 'replace':
                x = layer_input
            elif position == 'after':
                x = layer(layer_input)
            elif position == 'before':
                pass
            else:
                raise ValueError('position must be: before, after or replace')

            #debug
            #.print(layer.name + ' outside if ')
            #print(insert_layer.__class__.__name__ + ' outside if ')
            # function if adaptive_clip
            if insert_layer.__class__.__name__ == 'ReLU':
                new_layer = insert_layer()
                new_layer._name = '{}_{}'.format(layer.name, 'modified_to_relu')
                x = new_layer(x)
            elif (insert_layer.__class__.__name__ == 'function' or
                  insert_layer.__class__.__name__ == 'cython_function_or_method'):
                # adaptive clip
                #print(insert_layer.__class__.__name__ + ' inside if ' + '\n')
                x = insert_layer(t=x, invs=inv_scale[count_scale])
            else:
                pass

            count_scale = count_scale + 1

            if position == 'before':
                x = layer(x)
        else:
            if layer.__class__.__name__ == 'TFOpLambda' or layer.__class__.__name__ == 'SlicingOpLambda':
                kwargs = dict(layer.output.node.call_kwargs)
                layer_config_dict = layer.get_config()

                if len(layer.output.node.call_args) < len(layer.output.node.keras_inputs):
                    # 'abnormal case' where inputs number is lower than expected inputs number from graph parsing.
                    # It means that one graph input is treated among the kwargs instead of the args. So the fix is to
                    # pop the kwargs, and use the tensor as an actual input
                    list_key_to_remove = [k for k in kwargs.keys() if hasattr(kwargs[k], 'is_tensor_like') if
                                          kwargs[k].is_tensor_like]
                    if list_key_to_remove:
                        for k in list_key_to_remove:
                            kwargs.pop(k, None)
                        if nb_inputs_layer == 2:
                            x = layer(layer_input[0], layer_input[1], **kwargs)
                        elif nb_inputs_layer == 3:
                            x = layer(layer_input[0], layer_input[1], layer_input[2], **kwargs)
                        else:
                            print("Unsupported Lambda layer {} with {} inputs".format(layer.name, nb_inputs_layer))
                    else:
                        # standard list of inputs tensor
                        x = layer(layer_input, **kwargs)
                elif layer.__class__.__name__ == 'TFOpLambda' and layer_config_dict['function'] == 'tile':
                    kwargs['multiples'][0] = layer_input
                    x = layer(layer.output.node.call_args[0], **kwargs)
                else:
                    # standard list of inputs tensor
                    x = layer(layer_input, **kwargs)
            else:
                x = layer(layer_input)

        # Set new output tensor (the original one, or the one of the inserted layer)
        network_dict['new_output_tensor_of'].update({layer.name: x})

        # Save tensor in output list if it is output in initial model at origin, if layer_name
        if layer.name in model.output_names:
            model_outputs.append(x)

    return Model(inputs=model.inputs, outputs=model_outputs)


def _fold_bn_in_weights(weights, bias, gamma, beta, moving_avg, moving_var, epsilon=1e-3):
    """
         Implements equation for Backward BN weights folding.
         Args:
              weights: original weights
              bias: original bias
              gamma: multiplicative trainable parameter of the batch normalisation. Per-channel
              beta: additive trainable parameter of the batch normalisation. Per-channel
              moving_avg: moving average of the layer output. Used for centering the samples distribution after
              batch normalisation
              moving_var: moving variance of the layer output. Used for reducing the samples distribution after batch
              normalisation
              epsilon: a small number to void dividing by 0
         Returns: folded weights and bias
    """

    scaler = gamma / np.sqrt(moving_var + epsilon)
    weights_prime = [scaler[k] * channel for k, channel in enumerate(weights)]
    bias_prime = scaler * (bias - moving_avg) + beta

    return weights_prime, bias_prime


def bw_bn_folding(model, epsilon=1e-3, dead_channel_th=1e-10):
    """
        Search for BN to fold in Backward direction. Neutralise them before removal by setting gamma to all ones, beta
        to all zeros, moving_avg to all zeros and moving_var to all ones

        Args:
            model: input keras model
            epsilon: a small number to avoid dividing dy 0.0
            dead_channel_th: a threshold (very small) on moving avg and var below which channel is considered as dead
            with respect to the weights

        Returns: a keras model, with BN folded in backward direction, BN neutralised and then removed form the graph
    """
    folded_bn_name_list = []

    list_layers = model.layers
    for i, layer in enumerate(model.layers):
        if layer.__class__.__name__ == 'Functional':
            list_layers = model.layers[i].layers
            break

    for i, layer in enumerate(list_layers):
        if layer.__class__.__name__ == "DepthwiseConv2D" or layer.__class__.__name__ == "Conv2D":
            nodes = layer.outbound_nodes
            list_node_first = [n.layer for n in nodes]
            one_node = len(list_node_first) == 1
            # one_node controls that DW and BN are sequential
            # otherwise algo undefined
            if one_node and list_node_first[0].__class__.__name__ == "BatchNormalization":
                # store name previous DW and gamma, beta
                gamma = list_node_first[0].get_weights()[0]
                beta = list_node_first[0].get_weights()[1]
                moving_avg = list_node_first[0].get_weights()[2]
                moving_var = list_node_first[0].get_weights()[3]

                bias_exist = len(layer.get_weights()) == 2
                if bias_exist:
                    w = layer.get_weights()[0]
                    b = layer.get_weights()[1]
                else:
                    w = layer.get_weights()[0]
                    layer.use_bias = True
                    b = layer.bias = layer.add_weight(name=layer.name + '/kernel_1',
                                                      shape=(len(moving_avg),), initializer='zeros')

                if layer.__class__.__name__ == "DepthwiseConv2D":
                    # dead channel feature:
                    # when at the BN level there is a moving avg AND a moving variance very weak, it means the given
                    # channel is dead. Most probably the input channel was already close to zero, if the layer was a DW
                    # in fact this channel is dead w.r.t weight but plays a role with bias and beta. If the bias was
                    # used, most probably moving_avg would not be that weak
                    # however a very small moving_avg results in a great increase of weight dynamics for this channel
                    # which brings nothing in the end. This would degrade per-tensor quantization

                    for k, value in enumerate(moving_var):
                        if moving_var[k] <= dead_channel_th and moving_avg[k] <= dead_channel_th:
                            moving_var[k] = 1.0
                            moving_avg[k] = 0.0
                            gamma[k] = 0.0

                    # have ch_out first
                    w = np.transpose(w, (2, 0, 1, 3))
                    w, b = _fold_bn_in_weights(weights=w, bias=b, gamma=gamma, beta=beta, moving_avg=moving_avg,
                                              moving_var=moving_var, epsilon=epsilon)
                    w = np.transpose(w, (1, 2, 0, 3))
                    layer.set_weights([w, b])
                elif layer.__class__.__name__ == "Conv2D":
                    # have ch_out first
                    w = np.transpose(w, (3, 0, 1, 2))
                    w, b = _fold_bn_in_weights(weights=w, bias=b, gamma=gamma, beta=beta, moving_avg=moving_avg,
                                              moving_var=moving_var, epsilon=epsilon)
                    w = np.transpose(w, (1, 2, 3, 0))
                    layer.set_weights([w, b])

                # neutralise BN
                list_node_first[0].set_weights(
                    [np.ones(len(gamma)), np.zeros(len(beta)), np.zeros(len(moving_avg)), np.ones(len(moving_var))])
                folded_bn_name_list.append(list_node_first[0].name)

    model_folded = insert_layer_in_graph(model, layer_list=folded_bn_name_list, insert_layer=None,
                                         inv_scale=None, insert_layer_name=None, position='replace')

    return model_folded
