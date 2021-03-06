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

#include "drv.h"

void Delay_ms(int delayMs){
    tx_thread_sleep(delayMs);
}

void Delay_10us(void){
    printf("Delay_10us() not supported on the MT3620 under threadX!\n");
}

void Delay_22us(void){
    printf("Delay_22us() not supported on the MT3620 under threadX!\n");
}

void Delay_50us(void){
    printf("Delay_50us() not supported on the MT3620 under threadX!\n");
}

void Delay_1ms(void){
    Delay_ms(1);
}

void Delay_10ms(void){
    Delay_ms(10);
}

void Delay_30ms(void){
    Delay_ms(30);
}

void Delay_50ms(void){
    Delay_ms(50);
}

void Delay_80ms(void){
    Delay_ms(80);
}

void Delay_100ms(void){
    Delay_ms(100);
}
