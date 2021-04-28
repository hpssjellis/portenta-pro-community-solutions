# Portenta-Pro-Community-Solutions
Portenta Pro Community Solutions and Links. Zoom Wednesdays 6:00 am PST at least 15 minutes, max 60 minutes.

Started March 20st 2021

Note: This is a downloadable zippable Arduino library. Install using the library manager --> add .zip file

# Goal

To find and maintain example code and useful links that work with the Arduino Pro Portenta Series of Boards and Shields

## Wednesday Zoom Times  
6:00 am PST zoom link posted each Wednesday if I get up!   

6:00 am PST USA/Canada California/Vancouver  
 

Come chat for 10 minutes, not suppossed to be a long meeting. Zoom link posted here a few minutes before it starts.

# Not happening this Wed April 27th. I am still too tired. Darn day job!

Sorry. Touch base on Twitter https://twitter.com/rocksetta 





## General Rules:


1. I am a teacher. I will remove any inappropriate behaviour without warning!
1. Meeting may continue until 7:00 am PST but only if needed. (Meeting as short and sweet as possible)
1. Zoom Image taken at 6:15 PST (disable video if you don't want to be in the image)
1. Meeting potentailly locked at 6:15 am PST if needed
1. Meetings generally not recorded
1. Appropriate chat transcript might be added to [zoom-meetings-history](zoom-meetings-history)



# LoRA/Murata Module:

Super exciting is the ability to use the LoRa/Murata module directly on the Portenta that has the LoRa Vision Shield. This module is like a 3rd core that just runs the LoRa interface. Made to work with the MKRWAN library, but that gives no access to LoRa only LoRaWan.

Thanks to Arduino I have got this library working

https://github.com/hpssjellis/ArduinoCore-stm32l0

It is a bit weird. Hard to install, weird to get going as the Portenta needs to flash some code (PortentaBridge.ino) to allow the STM32LO board to flash code to the muarata module which can then communicate with the portenta over Serial3. All example code on the core when properly installed.

The big deal is this gives better LoRaWan abilities and pure LoRa abilities openning up some really interesting  Node / Gateway code for Portenta series.
