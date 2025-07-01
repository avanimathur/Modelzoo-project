 /**
 ******************************************************************************
 * @file    display_mpe.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef __DISPLAY_MPE_H
#define __DISPLAY_MPE_H

#include "mpe_yolov8_pp_if.h"
#include "mpe_pp_output_if.h"

void Display_mpe_InitFunctions(int clamp_point_init(int *x, int *y),
                               void convert_length_init(float32_t wi, float32_t hi, int *wo, int *ho),
                               void convert_point_init(float32_t xi, float32_t yi, int *xo, int *yo),
                               void Display_binding_line_init(int x0, int y0, int x1, int y1, uint32_t color));
void Display_mpe_Detection(mpe_pp_outBuffer_t *detect);

#endif /*__DISPLAY_MPE_H */
