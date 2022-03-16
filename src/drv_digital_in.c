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

#include "drv_digital_in.h"

err_t digital_in_init( digital_in_t *in, pin_name_t gpio_pin )
{
    if ( HAL_PIN_NC == gpio_pin)
    {
        return DIGITAL_IN_UNSUPPORTED_PIN;
    }

    mtk_os_hal_gpio_set_direction(gpio_pin, MHAL_GPIO_DIRECTION_INPUT);
    return DIGITAL_IN_SUCCESS;
}

uint8_t digital_in_read( digital_in_t *in )
{
    os_hal_gpio_data pval;

    if ( HAL_PIN_NC == in->pin)
    {
        return DIGITAL_IN_UNSUPPORTED_PIN;
    }

    mtk_os_hal_gpio_get_input(in->pin, &pval);
    return pval;
}