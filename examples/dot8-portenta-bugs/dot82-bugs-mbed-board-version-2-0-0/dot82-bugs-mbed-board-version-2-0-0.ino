/*
 *  No Issues yet. March 8th, 2021
 *
 * I was using the multiboard install that looks like it is now deprecated
 * I have now installed the single Portenta board version 2.0.0 which worked great on Ubuntu
 *
 * Possible some issues with my Machine learning example programs
 * I will have to test on another computer. The following single file Tensorflow Lite program did not run
 * https://github.com/hpssjellis/my-examples-for-the-arduino-portentaH7/blob/master/m09-Tensoflow/b01_makerML_hello_world.ino
 *
 * presently having this error but only on windows. (mar8th, 2021)
 * arm-none-eabi-gcc: error: CreateProcess: No such file or directory

 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * Above is too negative. Here I list the library programs that are working:
 * Tested and for me works using the Arduino 1.8.13 IDE:
 *
 * Testing March 8th, 2021
 *
 * Serial print from M7 core works
 * On board LED works on both PortentaH7 M7 and M4 cores
 * Serial.println on M4 core does not crash program (Need RPI to send to M7)
 * MBED on both M7 and M4 cores works
 * using the mbed namspace works
 * using the rtos namespace works
 * using RPC to send information from the M4 core to the M7 core works.
 *
 *
 *
 *
 *
 *
 *
*/
