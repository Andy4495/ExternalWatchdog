External Watchdog
=================

This Energia sketch implements a simple, independent watchdog that can be loaded
onto a low-end MSP processor to provide a watchdog reset signal.

The compiled code is under 1200 bytes and can therefore fit on a low-cost
chip like the MSP430G2231 or MSP430G2201.

It should also work on AVR processors, although it has not been tested on AVRs.

While processors generally contain internal watchdog timers, they are often
limited on the length of time before a watchdog reset occurs.

There is a lively discussion on the Internet on whether a device like this
should be necessary, as there potentially are ways to solve this problem
without using an external device. Regardless, in my case, this fit my needs,
was easy to program, and used components from my spare parts pile.

Usage
-----
The sketch has several definitions to configure the watchdog:

    LED_pin
Defines the output pin number used as an indicator to show that a watchdog reset is imminent (see almostTimeout_ms below). Note that the LED is optional and a physical LED does not need to be part of the watchdog circuit.

    WD_monitor_pin
Defines the input pin that is used to monitor the processor being watched. The device being watched needs to toggle this pin in under timeout_ms or else the External Watchdog will pull the reset line low.

    WD_reset_pin
Defines the output pin that is used to reset the device being watched. Note that this pin is normally held in a high impedence state (INPUT) unless it is actively pulling the RESET line low after a watchdog timeout. This allows the reset line on the device being watched to be connected in a standard configuration which typically includes a pull-up resistor to Vcc.

    timeout_ms
Defines the time (in ms) before a watchdog reset occurs.

    almostTimeout_ms
Defines the time (in ms) when a warning indicator is flashed to show that a watchdog reset it imminent.

    reset_delay_ms
Defines the time (in ms) that the reset line is held low.
