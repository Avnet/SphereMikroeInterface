
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

#include "os_hal_gpio.h"
#include "drv.h"

// Add typedef to match MikroE driver files
typedef enum
{
    DIGITAL_OUT_SUCCESS = 0,    
    DIGITAL_OUT_UNSUPPORTED_PIN = (-1)
} digital_out_err_t;

// Add typedef to match MikroE driver files
typedef struct
{
    os_hal_gpio_pin pin; 
} digital_out_t;


// Add function prototypes to match MikroE driver files
err_t digital_out_init( digital_out_t*, name);
void digital_out_high( digital_out_t*);
void digital_out_low( digital_out_t*);
void digital_out_toggle( digital_out_t*);
void digital_out_write( digital_out_t*, uint8_t);
