 /**
 ******************************************************************************
 * @file    display_keypoints_17.h
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

#ifndef DISPLAY_KEYPOINTS_17_H
#define DISPLAY_KEYPOINTS_17_H

#include "main.h"
#include "stm32_lcd.h"

static const int bindings[][3] = {
  {15, 13, COLOR_LEGS},
  {13, 11, COLOR_LEGS},
  {16, 14, COLOR_LEGS},
  {14, 12, COLOR_LEGS},
  {11, 12, COLOR_TRUNK},
  { 5, 11, COLOR_TRUNK},
  { 6, 12, COLOR_TRUNK},
  { 5,  6, COLOR_ARMS},
  { 5,  7, COLOR_ARMS},
  { 6,  8, COLOR_ARMS},
  { 7,  9, COLOR_ARMS},
  { 8, 10, COLOR_ARMS},
  { 1,  2, COLOR_HEAD},
  { 0,  1, COLOR_HEAD},
  { 0,  2, COLOR_HEAD},
  { 1,  3, COLOR_HEAD},
  { 2,  4, COLOR_HEAD},
  { 3,  5, COLOR_HEAD},
  { 4,  6, COLOR_HEAD},
};

static const int kp_color[17] = {
  COLOR_HEAD,
  COLOR_HEAD,
  COLOR_HEAD,
  COLOR_HEAD,
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

#endif /* __DISPLAY_KEYPOINTS_17_H */
