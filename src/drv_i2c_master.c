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

#include "drv_i2c_master.h"

static i2c_master_t *_owner = NULL;

/* DMA write/read buffers need to be located in sysram */
__attribute__((section(".sysram"))) uint8_t i2c_master_write_buf[I2C_BUFFER_MAX_LEN];
__attribute__((section(".sysram"))) uint8_t i2c_master_read_buf[I2C_BUFFER_MAX_LEN];

static err_t _acquire( i2c_master_t *obj, bool obj_open_state )
{
    err_t status = ACQUIRE_SUCCESS;

    if ( obj_open_state == true && _owner == obj )
    {
        return ACQUIRE_FAIL;
    }

    if ( _owner != obj )
    {
        status = mtk_os_hal_i2c_ctrl_init(obj->config.sda);
        if(status > 0){
            status = ACQUIRE_FAIL;
        }

        if ( status != ACQUIRE_FAIL )
        {
            _owner = obj;
        }
    }

    return status;
}

void i2c_master_configure_default( i2c_master_config_t *config )
{
    config->addr = 0;

    config->sda = 0xFFFFFFFF;
    config->scl = 0xFFFFFFFF;

    config->speed = I2C_MASTER_SPEED_STANDARD;
    config->timeout_pass_count = 10000;
}

err_t i2c_master_open( i2c_master_t *obj, i2c_master_config_t *config )
{

    i2c_master_config_t *p_config = &obj->config;
    memcpy( p_config, config, sizeof( i2c_master_config_t ) );

    return _acquire( obj, true );
}

err_t i2c_master_set_speed( i2c_master_t *obj, uint32_t speed )
{
    if( _acquire( obj, false ) == ACQUIRE_SUCCESS )
    {
        obj->config.speed = speed;
        if (mtk_os_hal_i2c_speed_init(obj->config.sda, obj->config.speed ) == 0)
        {
            return I2C_MASTER_SUCCESS;
        }
        else{
            return I2C_MASTER_ERROR;    
        }
    } else {
        return I2C_MASTER_ERROR;
    }
}

err_t i2c_master_set_timeout( i2c_master_t *obj, uint16_t timeout_pass_count )
{
    if( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        obj->config.timeout_pass_count = timeout_pass_count;
        return I2C_MASTER_SUCCESS;
    } else {
        return I2C_MASTER_ERROR;
    }
}

err_t i2c_master_set_slave_address( i2c_master_t *obj, uint8_t address )
{
    if( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        obj->config.addr = address;
        int result = mtk_os_hal_i2c_set_slave_addr(obj->config.sda, address);
        if(result == 0){
            return I2C_MASTER_SUCCESS;
        }
        
        return I2C_MASTER_ERROR;
    } else {
        return I2C_MASTER_ERROR;
    }
}

err_t i2c_master_write( i2c_master_t *obj, uint8_t *write_data_buf, size_t len_write_data )
{
    // Verify that the DMA buffer is large enough for the write data
    if(len_write_data > I2C_BUFFER_MAX_LEN){
        return I2C_MASTER_ERROR;
    }

    // Copy the incomming buffer data to the DMA buffer
    memcpy(i2c_master_write_buf, write_data_buf, len_write_data);

    if(_acquire( obj, false ) != ACQUIRE_FAIL )
    {
        if(mtk_os_hal_i2c_write(obj->config.sda, obj->config.addr, i2c_master_write_buf,len_write_data) == 0){
            return I2C_MASTER_SUCCESS;
        }
        else{
            return I2C_MASTER_ERROR; 
        }
    } else {
        return I2C_MASTER_ERROR;
    }
}

err_t i2c_master_read(i2c_master_t *obj, uint8_t *read_data_buf, size_t len_read_data )
{
    int returnValue = -1;

    // Verify the sram buffer is large enough for the read data
    if(len_read_data > I2C_BUFFER_MAX_LEN){
        return I2C_MASTER_ERROR;
    }

    if( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        returnValue = mtk_os_hal_i2c_read(obj->config.sda,obj->config.addr, i2c_master_read_buf, len_read_data);
        
        // Copy the rx data from the DMA buffer and into the passed in buffer
        if(returnValue >= 0){

            memcpy(read_data_buf, i2c_master_read_buf, len_read_data);
        }

        return I2C_MASTER_SUCCESS;
    
    } else {
        return I2C_MASTER_ERROR;
    }
}

err_t i2c_master_write_then_read( i2c_master_t *obj, uint8_t *write_data_buf, size_t len_write_data,
                                                     uint8_t *read_data_buf, size_t len_read_data )
{
    int returnValue = I2C_MASTER_ERROR;

    if( _acquire( obj, false ) != ACQUIRE_FAIL )
    {

        // Verify that the write buffer is large enough for the incomming data
        if(len_write_data > I2C_BUFFER_MAX_LEN){
            return I2C_MASTER_ERROR;
        }

        // Verify the read buffer is large enough for the read data
        if(len_read_data > I2C_BUFFER_MAX_LEN){
            return I2C_MASTER_ERROR;
        }

        // Copy the incomming write buffer into the DMA buffer
        memcpy(i2c_master_write_buf, write_data_buf, len_read_data);

        returnValue = mtk_os_hal_i2c_write_read(obj->config.sda, obj->config.addr, i2c_master_write_buf,
                                                i2c_master_read_buf,len_write_data, len_read_data);

        // Copy the return data from the DMA buffer into the passed in return buffer
        if(returnValue >= 0){
            memcpy(read_data_buf, i2c_master_read_buf, len_read_data);

        }

        return I2C_MASTER_SUCCESS;
    } else {
        return I2C_MASTER_ERROR;
    }
}

void i2c_master_close( i2c_master_t *obj )
{
    if( mtk_os_hal_i2c_ctrl_deinit(obj->config.sda) >= 0)
    {
        obj->handle = -1; //NULL;
        _owner = NULL;
    }
}
