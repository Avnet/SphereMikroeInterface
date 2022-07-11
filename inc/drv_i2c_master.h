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

#include "os_hal_i2c.h"
#include "drv.h"

#define I2C_BUFFER_MAX_LEN 256

// Add typedef to match MikroE driver files
typedef enum
{
    I2C_MASTER_SUCCESS = 0,  
    I2C_MASTER_ERROR = (-1) 
} i2c_master_err_t;

//Add enum to match MikroE driver files and map to MT3620 driver enums
typedef enum
{
    I2C_MASTER_SPEED_STANDARD = I2C_SCL_100kHz,
    I2C_MASTER_SPEED_FULL = I2C_SCL_400kHz,
    I2C_MASTER_SPEED_FAST = I2C_SCL_1000kHz
} i2c_master_speed_t;

// Add typedef to match MikroE driver files
typedef struct
{
    uint8_t addr; 

    pin_name_t sda;
    pin_name_t scl;

    uint32_t speed;
    uint16_t timeout_pass_count;
} i2c_master_config_t;

// Add typedef to match MikroE driver files
typedef struct
{
    handle_t handle;
    i2c_master_config_t config;
} i2c_master_t;

// Add function prototypes to match MikroE driver files
void i2c_master_configure_default( i2c_master_config_t*);
err_t i2c_master_open( i2c_master_t*, i2c_master_config_t*);
err_t i2c_master_set_speed( i2c_master_t*, uint32_t);
err_t i2c_master_set_timeout( i2c_master_t*, uint16_t);
err_t i2c_master_set_slave_address( i2c_master_t*, uint8_t);
err_t i2c_master_write( i2c_master_t*, uint8_t*, size_t);
err_t i2c_master_read( i2c_master_t*, uint8_t*, size_t);
err_t i2c_master_write_then_read( i2c_master_t*, uint8_t*, size_t, uint8_t*, size_t);