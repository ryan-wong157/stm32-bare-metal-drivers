/*
 * types.h
 *
 * Contains universal macros, structs, enumsS used by multiple drivers
 *
 *  Created on: Sep 8, 2025
 *      Author: Ryan Wong
 */

#ifndef TYPES_H_
#define TYPES_H_

 typedef enum {
    PIN_RESET = 0x00UL,
    PIN_SET = 0x01UL
 } PIN_State;

 typedef enum {
    HAL_OK = 0x00UL,
    HAL_ERROR = 0x01UL
 } HAL_Status;

 #endif