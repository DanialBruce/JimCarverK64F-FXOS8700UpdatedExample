/*******************************************************************************
MIT License

Copyright (c) [year] [fullname]

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

*******************************************************************************


This example is based on Jim Carver's original code " K64F_eCompass". Link: https://os.mbed.com/users/JimCarver/code/K64F_eCompass/

The Athor of this example updated the outdated and deprecated elements of the K64F_ecompass example,  modified library dependacies along with required changes in eCompass_lib, simplified some aspects of original code to suit the Author's needs.


---------------------------------------
Author: Danial Mousavi
Date of Creation: April 13th, 2022
Last modifed by: Danial Mousavi
on: April 25th, 2022

*/




#include "mbed.h"
#include "FXOS8700Q.h"
#include "eCompass_Lib.h"
#include "rtos.h"
 
I2C i2c(PTE25, PTE24);
FXOS8700QAccelerometer acc(i2c, FXOS8700CQ_SLAVE_ADDR1);
FXOS8700QMagnetometer mag( i2c, FXOS8700CQ_SLAVE_ADDR1);

DigitalOut gpo(D0);
DigitalOut led(LED_RED);
eCompass compass;


Thread sensorThread;

 
 
 
extern axis6_t axis6;
extern uint32_t seconds;
extern uint32_t compass_type; // optional, NED compass is default
extern int32_t tcount;
extern uint8_t cdebug;
int  l = 0;
volatile int sflag = 0;
 
 motion_data_counts_t mag_raw;
 motion_data_counts_t acc_raw;

void hal_map(  motion_data_counts_t * acc_raw, motion_data_counts_t * mag_raw);
void debug_print(void);
void sensorFunction_thread();


int main() {
 
 
 
//cdebug = 1;  // uncomment to disable compass
printf("\r\n\n\n\n\n\n\n");
printf("Who AM I= %X\r\n", acc.whoAmI());
acc.enable();
 
 
acc.getAxis( acc_raw);
mag.getAxis( mag_raw);

sensorThread.start(sensorFunction_thread);
    while(1) {
        debug_print();
        ThisThread::sleep_for(20ms);
    }
 
}

//optional
void hal_map(  motion_data_counts_t * acc_raw,  motion_data_counts_t * mag_raw)
{
    int16_t t;
    // swap and negate X & Y axis
    t = acc_raw->x;
    acc_raw->x = acc_raw->y * -1;
    acc_raw->y = t * -1;
    // swap mag X & Y axis
    t = mag_raw->x;
    mag_raw->x = mag_raw->y;
    mag_raw->y = t;
    // negate mag Z axis
    mag_raw->z *= -1;
}

//
// Print data values for debug
//
void debug_print(void)
{
    // Some useful printf statements for debug
    printf("time: %d roll=%d, pitch=%d, yaw=%d\r\n", axis6.timestamp, axis6.roll, axis6.pitch, axis6.yaw);
}
 
 
void sensorFunction_thread() {
    while (true) {
        // get raw data from the sensors
        acc.getAxis( acc_raw);
        mag.getAxis( mag_raw);
        if(tcount) compass.run( acc_raw, mag_raw); // calculate the eCompass
        if(l++ >= 50) { // take car of business once a second
            seconds++;
            sflag = 1;
            compass.calibrate();
            debug_print();
            l = 0;
            led = !led;
            }
        tcount++;
        ThisThread::sleep_for(20ms);
    }
}