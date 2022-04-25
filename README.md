# JimCarverK64F-FXOS8700UpdatedExample
Thi is My updated version of JimCarter's FXOS8700 6-axis IMU example code. The measurement unit in question is present on F64K development board.

At the time of making this example, the latest Mbed-os version was 6.15.1. This example is based on Jim Carver's example written for Mbed 2. I made following changes to the code:

- Library dependencies have changed. changed the FXOS8700 library code from Jim Carver's version (http://mbed.org/users/JimCarver/code/FXOS8700Q/#5553a64d0762) to the offical NXP verison found here: https://os.mbed.com/teams/NXP/code/FXOS8700Q/#834488c11340

- Since there where changes made in library dependencies, the eCompass_FPU_Lib library code written by Jim Carver had to be also slightly modified accordingly. Link to Jim Carter's eCompass_lib library: https://os.mbed.com/users/JimCarver/code/eCompass_FPU_Lib/

In my example, I'm using simple Mbed Thread instance. depending on one's needs, situation and point of view, this is ofcourse, not the best way to utilize the sensor as there are more efficient ways to do it. However, I only had simplicity in mind and wanted to keep it as simple as possible to suit my needs. Jim Carver used, now deprecated, RtosTimer. If you want to implement what Jim Carver originally had in mind in his example, check EventQueue documentation found here: https://os.mbed.com/docs/mbed-os/v6.15/apis/eventqueue.html
