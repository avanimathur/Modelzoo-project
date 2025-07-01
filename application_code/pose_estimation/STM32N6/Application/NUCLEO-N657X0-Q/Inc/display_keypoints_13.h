 /**
 ******************************************************************************
 * @file    display_keypoints_13.h
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

#ifndef DISPLAY_KEYPOINTS_13_H
#define DISPLAY_KEYPOINTS_13_H

#include "main.h"
#include "stm32_lcd.h"

static const int bindings[][3] = {
  { 11, 9, COLOR_LEGS},
  { 9,  7, COLOR_LEGS},
  {12, 10, COLOR_LEGS},
  {10,  8, COLOR_LEGS},
  { 7,  8, COLOR_TRUNK},
  { 1,  7, COLOR_TRUNK},
  { 2,  8, COLOR_TRUNK},
  { 1,  2, COLOR_ARMS},
  { 1,  3, COLOR_ARMS},
  { 2,  4, COLOR_ARMS},
  { 3,  5, COLOR_ARMS},
  { 4,  6, COLOR_ARMS},
  { 0,  1, COLOR_HEAD},
  { 0,  2, COLOR_HEAD},
};

static const int kp_color[13] = {
  COLOR_HEAD,
  COLOR_ARMS,
  COLOR_ARMS,
  COLOR_ARMS,
  COLOR_ARMS,
  COLOR_ARMS,
  COLOR_ARMS,
  COLOR_TRUNK,
  COLOR_TRUNK,
  COLOR_LEGS,
  COLOR_LEGS,
  COLOR_LEGS,
  COLOR_LEGS,
};

#endif /* __DISPLAY_KEYPOINTS_13_H */
