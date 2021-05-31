On an aside note:


### LoRA/Murata Module:

Super exciting is the ability to use the LoRa/Murata module directly on the Portenta that has the LoRa Vision Shield. This module is like a 3rd core that just runs the LoRa interface. Made to work with the MKRWAN library, but that gives no access to LoRa only LoRaWan.

Thanks to Arduino I have got this library working

https://github.com/hpssjellis/ArduinoCore-stm32l0

It is a bit weird. Hard to install, weird to get going as the Portenta needs to flash some code (PortentaBridge.ino) to allow the STM32LO board to flash code to the muarata module which can then communicate with the portenta over Serial3. All example code on the core when properly installed.

The big deal is this gives better LoRaWan abilities and pure LoRa abilities openning up some really interesting  Node / Gateway code for Portenta series.
