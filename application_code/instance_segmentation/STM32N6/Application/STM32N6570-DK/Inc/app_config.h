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
#define POSTPROCESS_TYPE    POSTPROCESS_ISEG_YOLO_V8_UI

#define NN_WIDTH      (256)
#define NN_HEIGHT     (256)
#define NN_BPP 3

#define COLOR_BGR (0)
#define COLOR_RGB (1)
#define COLOR_MODE    COLOR_RGB

/* Classes */
#define NB_CLASSES (80)
#define CLASSES_TABLE const char* classes_table[NB_CLASSES] = {\
  "person",\
  "bicycle",\
  "car",\
  "motorcycle",\
  "airplane",\
  "bus",\
  "train",\
  "truck",\
  "boat",\
  "traffic light",\
  "fire hydrant",\
  "stop sign",\
  "parking meter",\
  "bench",\
  "bird",\
  "cat",\
  "dog",\
  "horse",\
  "sheep",\
  "cow",\
  "elephant",\
  "bear",\
  "zebra",\
  "giraffe",\
  "backpack",\
  "umbrella",\
  "handbag",\
  "tie",\
  "suitcase",\
  "frisbee",\
  "skis",\
  "snowboard",\
  "sports ball",\
  "kite",\
  "baseball bat",\
  "baseball glove",\
  "skateboard",\
  "surfboard",\
  "tennis racket",\
  "bottle",\
  "wine glass",\
  "cup",\
  "fork",\
  "knife",\
  "spoon",\
  "bowl",\
  "banana",\
  "apple",\
  "sandwich",\
  "orange",\
  "broccoli",\
  "carrot",\
  "hot dog",\
  "pizza",\
  "donut",\
  "cake",\
  "chair",\
  "couch",\
  "potted plant",\
  "bed",\
  "dining table",\
  "toilet",\
  "tv",\
  "laptop",\
  "mouse",\
  "remote",\
  "keyboard",\
  "cell phone",\
  "microwave",\
  "oven",\
  "toaster",\
  "sink",\
  "refrigerator",\
  "book",\
  "clock",\
  "vase",\
  "scissors",\
  "teddy bear",\
  "hair drier",\
  "toothbrush"};

/* I/O configuration */
#define AI_YOLOV8_SEG_PP_TOTAL_BOXES       (1344)
#define AI_YOLOV8_SEG_PP_NB_CLASSES        (NB_CLASSES)
#define AI_YOLOV8_SEG_PP_MASK_NB           (32)
#define AI_YOLOV8_SEG_PP_MASK_SIZE         (64)
#define AI_YOLOV8_SEG_ZERO_POINT           (25)
#define AI_YOLOV8_SEG_SCALE                (0.020020058378577232f)
#define AI_YOLOV8_SEG_MASK_ZERO_POINT      (-115)
#define AI_YOLOV8_SEG_MASK_SCALE           (0.0207486841827631f)

/* --------  Tuning below can be modified by the application --------- */
#define AI_YOLOV8_SEG_PP_CONF_THRESHOLD        (0.5f)
#define AI_YOLOV8_SEG_PP_IOU_THRESHOLD         (0.4f)
#define AI_YOLOV8_SEG_PP_MAX_BOXES_LIMIT       (10)

/* Display */
#define WELCOME_MSG_1         "yolov8n_256_quant_pc_ii_seg_coco-st.tflite"
#define WELCOME_MSG_2         "Model Running in STM32 MCU internal memory"

#endif
