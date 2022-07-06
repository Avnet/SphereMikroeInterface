#pragma once
/*

MIT License

Copyright (c) Avnet Corporation. All rights reserved.
Author: Brian Willess

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "tx_api.h"  // For Delay_ms()
#include <stdint.h>  // For uint8_t
#include <stdio.h>  // For printf()

#define MT3620_TIMER_TICKS_PER_SECOND ((ULONG) 100*10)

// Constantes for the MikroE Shim
#define HAL_PIN_NC -1

// Types for the MikroE Shim
typedef int pin_name_t;
typedef int name;
typedef int err_t;
typedef int handle_t;

// TODO Implement Ring Buffer for UART Support
typedef struct
{
    uint8_t *buffer;
    size_t capacity;
    volatile size_t size;
    volatile size_t head;
    volatile size_t tail;
} ring_buf8_t;

typedef enum
{
    ACQUIRE_SUCCESS = 0,
    ACQUIRE_INIT,
    ACQUIRE_FAIL = (-1)
} acquire_t;

// Function Prototypes for the MikroE Shim
void Delay_ms(int);
void Delay_10us(void);
void Delay_22us(void);
void Delay_50us(void);
void Delay_1ms(void);
void Delay_10ms(void);
void Delay_30ms(void);
void Delay_80ms(void);
void Delay_100ms(void);

// Defines and Macro to leverage examples hardware mapping
#define MIKROBUS_AN AN
#define MIKROBUS_RST RST
#define MIKROBUS_CS CS
#define MIKROBUS_SCK SCK
#define MIKROBUS_MISO MISO
#define MIKROBUS_MOSI MOSI
#define MIKROBUS_PWM PWM
#define MIKROBUS_INT INT
#define MIKROBUS_RX RX
#define MIKROBUS_TX TX
#define MIKROBUS_SCL SCL
#define MIKROBUS_SDA SDA

#define MIKROBUS(index, pinout) MIKROBUS_IMPL(index, pinout)
#define MIKROBUS_IMPL(index, pinout) MIKROBUS_##index##_##pinout
