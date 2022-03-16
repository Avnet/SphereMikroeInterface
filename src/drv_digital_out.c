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

#include "drv_digital_out.h"

err_t digital_out_init( digital_out_t *out, pin_name_t unusedParm )
{
    if ( HAL_PIN_NC == out->pin )
    {
        return DIGITAL_OUT_UNSUPPORTED_PIN;
    }
    
    if( 0 == mtk_os_hal_gpio_set_direction( out->pin, OS_HAL_GPIO_DIR_OUTPUT)){
        return DIGITAL_OUT_SUCCESS;
    }
    return DIGITAL_OUT_UNSUPPORTED_PIN;
}

void digital_out_high( digital_out_t *out )
{
    if ( OS_HAL_GPIO_MAX > out->pin)
    {
        mtk_os_hal_gpio_set_output(out->pin, OS_HAL_GPIO_DATA_HIGH);
    }
}

void digital_out_low( digital_out_t *out )
{
    if ( OS_HAL_GPIO_MAX > out->pin )
    {
        mtk_os_hal_gpio_set_output(out->pin, OS_HAL_GPIO_DATA_LOW);
    }
}

void digital_out_toggle( digital_out_t *out )
{
    os_hal_gpio_data currentValue;    

    if ( OS_HAL_GPIO_MAX > out->pin )
    {
        mtk_os_hal_gpio_get_output(out->pin, &currentValue);
        
        if(currentValue == OS_HAL_GPIO_DATA_LOW){
            mtk_os_hal_gpio_set_output(out->pin, OS_HAL_GPIO_DATA_HIGH);
        }
        else{
            mtk_os_hal_gpio_set_output(out->pin, OS_HAL_GPIO_DATA_LOW);
        }
    }
}

void digital_out_write( digital_out_t *out, uint8_t state )
{
    if ( OS_HAL_GPIO_MAX > out->pin )
    {
        mtk_os_hal_gpio_set_output(out->pin, (os_hal_gpio_data)state);
    }
}

