 /**
 ******************************************************************************
 * @file    app_config.h
 * @author  GPM Application Team
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef APP_CONFIG
#define APP_CONFIG

#include "arm_math.h"

#define USE_DCACHE

/*Defines: CMW_MIRRORFLIP_NONE; CMW_MIRRORFLIP_FLIP; CMW_MIRRORFLIP_MIRROR; CMW_MIRRORFLIP_FLIP_MIRROR;*/
#define CAMERA_FLIP CMW_MIRRORFLIP_NONE

#define ASPECT_RATIO_CROP       (1) /* Crop both pipes to nn input aspect ratio; Original aspect ratio kept */
#define ASPECT_RATIO_FIT        (2) /* Resize both pipe to NN input aspect ratio; Original aspect ratio not kept */
#define ASPECT_RATIO_FULLSCREEN (3) /* Resize camera image to NN input size and display a maximized image. See Doc/Build-Options.md#aspect-ratio-mode */
#define ASPECT_RATIO_MODE ASPECT_RATIO_CROP

/* Model Related Info */
#define POSTPROCESS_TYPE    POSTPROCESS_SPE_MOVENET_UF

#define NN_WIDTH      (192)
#define NN_HEIGHT     (192)
#define NN_BPP 3

#define COLOR_BGR (0)
#define COLOR_RGB (1)
#define COLOR_MODE    COLOR_RGB

/* I/O configuration */
#define AI_SPE_MOVENET_POSTPROC_HEATMAP_WIDTH        (NN_WIDTH/4)
#define AI_SPE_MOVENET_POSTPROC_HEATMAP_HEIGHT       (NN_HEIGHT/4)

/* Post processing values */
#define AI_POSE_PP_CONF_THRESHOLD           (0.5f)
#define AI_POSE_PP_POSE_KEYPOINTS_NB        (13) /* Movenet: 13 or 17 keypoints ; YOLOv8 pose: 17 keypoints */

/* Display */
#define WELCOME_MSG_1         "st_movenet_lightning_heatmaps_192_int8_pc.tflite"
#define WELCOME_MSG_2         "Model Running in STM32 MCU internal memory"

#endif
