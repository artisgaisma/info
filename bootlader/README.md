# MiniCore
- This core gives you two extra IO pins if you're using the internal oscillator! PB6 and PB7 is mapped to Arduino pin 20 and 21.
# Optiboot
` https://github.com/Optiboot/optiboot

```
Optiboot is an easy to install upgrade to the Arduino bootloader within Arduino boards. It provides the following features:

Allows larger sketches. Optiboot is only 512 bytes, freeing 1.5k of extra code space compared to older bootloaders.
Makes your sketches upload faster. Optiboot operates at higher baud rates and has streamlined programming.
Adaboot performance improvements. Optiboot implements "fastboot" that starts sketches immediate after power-on.
Compatible with ATmega8, ATmega168, and ATmega328p Arduinos and derivatives including Lilypad, Pro, Nano, and many derivatives.
Works with MANY additional Atmel AVR chips - almost anything that supports bootloads or "flash self-programming." This includes chips from ATtiny 8pin chips through the 100pin ATmega2560 used on Arduino Mega.
Supports alternate serial ports, CPU frequencies and baud rates.
```
### Fuses
``` 
programmed with arduino using programmer. all was vell. 
Fuses: 
Low Fuse		0xFF
High Fuse		0xDA
Extended Fuse		0xFD
before any "get", do a RESET !!!, otherwise - soft freezes

Arduino default Pro/Pro mini 3.3v, 8Mhz:
Low Fuse		0xFF
High Fuse		0xDA // DE for Optiboot - 0.5KB bootloader, not 2 or 4KB
Extended Fuse	0x05 - BOD 2.7V
Arduino UNO has Optiboot - try this.
```

```
1.The FLASH security is based on two Memory Lock bits, LB2 and LB1 for the application 
2.section and BLB12 and BLB11 for the boot loader section, if your chip has a boot loader section.
3.When LB1 is programmed, all write accesses to the application section are denied. 
4.When both LB2 and LB1 are programmed, all write and external read accesses to the 
5.application section are denied. The similiar settings for the BLB12 and BLB11 bit 
6.applies to the boot loader section. When none of the Lock bits are programmed, 
7.there are no memory lock features enabled.(Please note that “1” means unprogrammed and “0” means programmed.)
3.You can secure your firmware against external inspection by programming the Lock Bits.
4.You need read the datasheet in the 'Memory Programming' section for you specific chips for more specific information.
```

```
I found the whole section in the data sheet on lock bits and especially the 
Application and Boot section locks bits to be less than clear.
I have 99.1% of flash consumed and I have no boot section. I found that when 
I set the lock bits, suddenly the code was corrupted. Upon trial and error I 
found that the BLB1 fuse if set to "LPM prohibited in the Boot section" was 
the source of the issue. I assume that this setting carved out a boot section
and prevented reads... Why would you ever want to prevent reads (LPM)?
Writes (SPM) I get

To protect the code from prying eyes and to avoid potential power event corruption, I had to set the bits to:
LB: Further Programming and verification disabled
BLB0: LPM and SPM prohibited in App section
BLB1: SPM prohibited in Boot Section.

Does this seem right given no boot section?
If I set BLB1 to: SPM and LPM prohibited in boot section, code fails.
```

# Bootloader - BLE HEX


### Test

```
- Tested on FIRST-H 2021-03-17
		FLASH	MAX	GLOBAL		LOCAL bytes leaved
Mega 2560	9%	253952	12%		7185
Mega 1280	20%	126976	12%		7185
arduino UNO	74%	32256	42%		1179
Pro Pro Mini	78%	30720	42%		1179
arduino Mini	83%	28672	42%		1179


```
# Wiring
<img src="./burn bootloader.png">
<img src="./arduino nano as bootloader.jpg">


# USB ASP 
## Install - win7 win10
- run `zadig-2.3.exe` - https://zadig.akeo.ie/
- choose USBasp from list
- install as `libusbK(v3.0.7.0)`
- 
### source
- https://rlogiacco.wordpress.com/2016/09/01/usbasp-windows-10/
- https://www.instructables.com/USBASP-Installation-in-Windows-10/
