[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate?hosted_button_id=WT735CX4UMZ9U)

# BEMANI PSX Adapter

USB Adapter for Playstation IIDX and Pop'n Music Controllers

Generic Playstation to USB adapters are not working correctly with Bemani controllers :

- IIDX controllers have a digital TT instead of an analog axis, and lack the correct descriptors for Infinitas or Ultimate Mobile compatibility

- Pop'n Music controllers hold left+down+right constantly and the right yellow button is mapped to up, which confuses most adapters into making the right yellow button a normally closed "down" input.

This code solves these issues and auto-detects whether a Pop'n Music controller is plugged.

You can buy a cheap "Dualshock extension cable" or a dualshock breakout board to build an external adapter box, or you can buy a pre-made adapter from me to support my work :)

## Demo

https://www.instagram.com/p/CHVLleylqFl/

## Acknowledgments

The PSX pad read code is using [PsxNewLib by SukkoPera](https://github.com/SukkoPera/PsxNewLib/).

## Features

Pop'n Music and IIDX controller modes are auto detected (you must plug the PSX controller to the adapter before plugging the USB)

### Pop'n Music mode

- Compatible with Pop'n Music Lively natively (wired, **not bluetooth**)

- All buttons are recognized as simple buttons from 1 to 9 then select and start.

- It should be compatible with Lively without any remapping, but also with UltimatePopnController's ezusb.dll for other purposes

### IIDX mode

- Compatible with Infinitas and Ultimate Mobile natively (wired, **not bluetooth**)

- Buttons 1 to 7 are mapped to the first 7 buttons, then E1-E4 are mapped to buttons 9-12.

- The TT is mapped to the X analog axis.

- Still includes a HAT switch to retrieve the digital TT signals (useful for MAME)

- Select acts as E1, Start as E2.

- Double (resp. triple) press Select acts as E3 (resp. E1+E3), double (resp. triple) press Start acts as E4 (resp. E2+E4)

- **Note**: digital TT signal is lingering a bit too much, which causes inertia on the analog axis. This is a **hardware limitation** from the controller itself. In order to make song wheel selection less painful, holding SELECT allows for very slow increments. This also means that it's important to keep alternating direction when playing a rapid succession of scratch notes to avoid dropped inputs.

## Building Instructions

- Download Arduino IDE, 
- Install greiman's DigitalIO library from library manager
- Build and Flash for your ATMEGA32U4 Board
- Have Fun

## Pinout

When looking at the plug of the *controller cable* :
```
  1  2  3   4  5  6   7  8  9
-------------------------------
| o  o  o | o  o  o | o  o  o |  (at the Controller)
\_____________________________/

1 : DAT        -> to arduino MISO (on ICSP header) (with 1k ohm pullup resistor*)
2 : CMD        -> to arduino MOSI (on ICSP header)
4 : GND        -> to arduino GND
5 : 3.3V       -> to arduino 3.3V or 5V (tested on Pop'n KASC, minicon, and IIDX JKOC)
6 : Attention  -> to arduino analog pin A2
7 : SCK        -> to arduino SCK (on ICSP header)

* Note: 1k ohm pullup resistor to 5V means you also have to put a 1k ohm resistor between 5V and MISO.
See the demo pic.

 ```
 
## Donation

If this project helps you and you want to give back, you can help me with my future projects.

While not necessary, donations are much appreciated and will only go towards funding future github projects (arcade hardware ain't cheap :( ).

Of course you'll also receive my gratitude and I'll remember you if you post a feature request ;)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=WT735CX4UMZ9U)
