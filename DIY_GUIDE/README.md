[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate?hosted_button_id=WT735CX4UMZ9U)

# BEMANI PSX Adapter - DIY Adapter Tutorial

USB Adapter for Playstation IIDX and Pop'n Music Controllers.

Here's a step-by-step guide on how I used to make them

## Required materials

### CJMCU Beetle USB

![cjmcu](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/00_CJMCU.jpg)

This is the easiest atmega32u4 to work with thanks to these big USB pads one can solder directly onto

### PSX to USB or PSX to PS3 adapter

![psxusb](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/00_adapter.jpg)

While not the smallest and easiest to build with, this is the cheapest and most available adapter from Aliexpress etc..

Feel free to use a bigger adapter if you're worried about soldering with limited space available.

### Resistor

![resistor](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/00_resistor.jpg)

Not a lot of power to dissipate, so 0.25W will suffice.
1K is what's recommended, but 2K, 5K or 10K will work as well.

Basically take any cheap resistor between 1K and 10K that you can find, it should work

### Wires

You need 6 pieces of wire, I recommend using 22 or 24 awg

### Soldering iron and hot air station

If available, use hot air and kapton tape to desolder the existing PCB.

Otherwise you can also use your soldering iron and a desoldering pump or wick.

### Cutting pliers and wire stripper

![cutting](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/00_cutting.jpg)
![stripper](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/00_stripper.jpg)

This will be used to prepare wires.

## Step by step tutorial

### Step 1: Preparing the adapter

#### Open the adapter

Disassemble the adapter by inserting a flat screwdriver above the controller plug and gently twist to separate both halves, exposing the PCB soldered on the connector and USB cable.

#### Remove USB wires

Use cutting pliers to remove the 4 usb wires from the PCB.

#### Remove PCB

![01a](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/01a_adapter_remove_pcb.jpg)

Protect the plastic with Kapton tape and use hot air to separate the PCB from the controller plug.

![01b](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/01b_adapter_pcb_removed.jpg)

#### Clean pins

![01c](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/01c_adapter_clean_pins.jpg)

Use your soldering iron to clean solder off the pins

![01d](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/01d_adapter_pins_cleaned.jpg)

**Congrats, the controller plug is now ready to be used**, now let's take care of the MCU

### Step 2: Preparing the MCU

Note: I suggest flashing the code **BEFORE** soldering anything, but if you forgot to flash it, you can always do it later using the usb cable soldered on it

#### Resistor and wires

![02a](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/02a_mcu_resistor.jpg)

Start with the resistor, place it between pins MI and 5V (top and bottom pins from right side of the MCU), but **DO NOT SOLDER YET**, we need to insert wires into these holes as well:

- Take your 6 wires, strip one end and insert one of each into **MI**, **MO**, **SCK**, **GND**, **5V** and **A2** pin-holes.

You can now solder everything on MCU side

![02b](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/02b_mcu_wires.jpg)
![02c](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/02c_mcu_wires_front.jpg)

#### USB

Apply tin to the 4 usb pads

![02d](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/02d_mcu_usb_solder.jpg)

Then solder the 4 USB wires on them, carefully following the BLACK-GREEN-WHITE-RED order

![02e](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/02e_mcu_usb_solder_2.jpg)

When in doubt, use a multimeter to check which pad is in continuity with GND pin, and which pad is in continuity with 5V pin

### Step 3: Assembly

Find a way to place both the cable and MCU inside the adapter, with the controller plug set back into place as well

![03a](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/03a_assembly_position.jpg)

Then it's time to cut wires to a shorter length (you want the minimum length so that you can close the adapter normally)

![03b](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/03b_assembly_cut_wires.jpg)

Finally, following the pinout, you can solder everything following the pinout

```
  1  2  3   4  5  6   7  8  9
-------------------------------
| o  o  o | o  o  o | o  o  o |  (at the Controller)
\_____________________________/

1 : DAT        -> MI (the pin which also has the pullup resistor on it)
2 : CMD        -> MO
4 : GND        -> GND
5 : 3.3V       -> 5V (yes, spec is 3.3v but I've been using 5v without issue)
6 : Attention  -> A2
7 : SCK        -> SCK

 ```

**/!\ Pin 1 is on top of the pic** (since this is what the leftmost pin of controller cable will plug into)

![03c](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/03c_assembly_solder.jpg)

#### Close the adapter box

With everything soldered you can now close the adapter by just pressing both sides together it should snap into place

![04](https://github.com/CrazyRedMachine/BEMANI_PSX_Adapter/blob/main/DIY_GUIDE/resources/04_done.jpg)

I like adding some tape in order not to confuse a modded adapter with a regular one :)

## Closing words

Congrats on making your Bemani adapter :)

Note that you can also flash my LUFAHybridFightstick firmware on it if you take the "PSX" branch and it will serve as a low latency PSX to USB adapter, but I think that's a bit overkill, and doing that without exposing the RST pin will make it hard to flash another firmware on it later on (PROTIP: you can either mod a reset button or solder that RST pin to an unused pin from the PSX plug).

## Donation

If this project helps you and you want to give back, you can help me with my future projects.

While not necessary, donations are much appreciated and will only go towards funding future github projects (arcade hardware ain't cheap :( ).

Of course you'll also receive my gratitude and I'll remember you if you post a feature request ;)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=WT735CX4UMZ9U)
