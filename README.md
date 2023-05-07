# Other
- CDM -> diskpart -> list disk

# Restore
```
Solution #1: Restore Deleted Files from USB Drive Using CMD
1. Connect the USB flash drive to your computer.
2. Open the Start menu, type "cmd", and hit Enter on your keyboard.
3. Type "chkdsk X: /f" in the Command Prompt window and hit Enter. Just make sure to replace “X” with the letter assigned to your USB flash drive.recover deleted files from flash drive without software
4. Finally, type "ATTRIB -H -R -S /S /D X:*.*" and hit the Enter key (Again, replace “X” with the drive letter of the USB drive).recover deleted files from flash drive
5. Wait for the process to be completed.

Solution #2: Use the Restore Previous Versions Feature
1. Connect the USB flash drive to your computer.
2. Open the Start menu, type "This pc", and hit Enter on your keyboard.
3. Right-click on your USB flash drive and select "Properties".
4. Go to the "Previous Versions" tab and look at the list of available recovery options.
5. Select a previous version and click "Restore".

Solution #3: Use a Data Recovery Software 
Today we will be using the latest version of Disk Drill to recover data. It’s is one of the most popular and well-established data recovery software on the market.

Steps to recover deleted files from a USB flash drive:
1. Go to the following address and download Disk Drill: https://www.cleverfiles.com/
2. With your pen drive connected, launch Disk Drill 
3. Click the "Search for lost data" button next to it.
4. Select which files you want Disk Drill to recover and click the "Recover" button.
```

# MCU

`8MHZ - 8,000,000 cycles per second`

```int 4bytes -2`147`483`648	to 2`147`483`648 or uint32_t 4 bytes [0-4294967295]```

`EEPROM`	 write cycle typically takes 	`3.3 m`s to complete

## TI
		
|	|	I/O|		FLASH|	RAM|	BAND|
|----|----|----|----|----|
|CC1301	| 	10-30	|	32-128|	16/20|	Sub 1GHz, Wi-Fi|
|CC1350	|	10-33	|	128KB|	20KB|	Sub-1GHz, Wi-Fi, BLE|
|CC1352R|	28	|	352KB|	80KB|	Sub-1GHz AND 2.4GHz - DUAL BAND|
|CC1352P|	26	|	352KB|	80KB|	Sub-1GHz AND 2.4GHz - integrated amplifier|
|CC2640R2|	10-31	|	128KB|	20KB|	BLE 5, 2.4GHz|
|||||
||||
|CC110L| transmit|		<1.5e|	300-928MHz|
|CC115L| transmit|		<1e|	300-928MHz|
|CC1150| transmit|		<2e |	300-900MHz|
|CC1200| transceiv|		<2.5e| 	136-960MHz|
|CC1175| trannmit|		<2.5e |	136-960MHz| - 10pcs ordere - 25eur|

***CC1310 vs CC1350: Can be replacable each other.
The main advantage of the CC1352 over the CC1350 is the increased memory. 
With the increased memory you can run multiple stacks (TI15.4 and BLE). 
CC1350 can only run one stack at a time or one full stack and partially 
another for example, you can run the full TI 15.4 Stack with limited 
BLE functionality (only beaconing ad not connections)

### Atmel
```
		I/O 	FREQ	FLASH	EEPORM	RAM	PWM	USARTS
AVR128DA32		1-24	128KB	512B	16KB		3	has WatchDogTimer
Atmega 4808	27	1-20	48KB	256B	6KB	9	3	1.5
Atmega 4809	41	1-20	48KB	256B	6KB	11	4	1.5
Atmega 328p	23	1-20	32KB	1KB	2KB	6	1	1.5EUR
Atmega 32U4	26	8/16	32KB	1KB	2.5	5?	1
Atmega 1281 	54	8/16	128KB	4KB	8KB	6	2	6EUR
Atmega 2561	54	8/16	256KB	4KB	8KB	6	2
Atmega 1280	86	8/16	128KB	4KB	8KB	12	4
Atmega 2560	86	8/16	256KB	4KB	8KB	12	4
Atxmega256a3u 	64p	32	256	4KB	16KB	22	7	8eur	Allard QuadAnt taimeris

STM32F103x8	32	8-78	64KB	---	20KB	13	3	Have sample Board

SAMD21			48MHz	256KB	NO	32KB 12	

nRF52832
```
# Atmege BOD levels
- 4.3V - 100
- 2.7 - 101
- 1.8 - 110
- disabled -111
### Atmega4808/9
- MEgaCoreX - https://github.com/MCUdude/MegaCoreX
### BOD option
- Brownout detection or BOD for short lets the microcontroller sense the input voltage and shut down if the voltage goes below the brownout setting. Below is a table that shows the available BOD options:

### BOD threshold
4.3 V
4.0 V
3.7 V
3.3 V
2.9 V
2.6 V (default option)
2.1 V
1.8 V
Disabled
 
### LPC11U3x
32-bit ARM Cortex-M0 
up to 128 kB flash; 
upto 12 kB SRAM 
4 kB EEPROM
USB device
USART


## Ferrite Bead
```
A ferrite bead is a passive device that filters high frequency noise energy 
over a broad frequency range. It becomes resistive over its intended frequency
range and dissipates the noise energy in the form of heat. ... This forms
a low-pass filter network, further reducing the high frequency power supply noise.
```
![ferrite bead](https://user-images.githubusercontent.com/51158344/149995203-f96739c8-cf2c-4181-9f2d-f02e2d633dca.PNG)


## RT9193 - LDO with loadswitch

![RT9193](https://user-images.githubusercontent.com/51158344/138400954-dc9b8b29-33b9-41eb-a0b5-8a2bf2fd1008.JPG)



# CODE

### Data Types
```
	"32-bit CPU"
	char		1byte	-128				to 	127
	short		2bytes	-32`768 			to	32`768
	int		4bytes	-2`147`483`648			to	2`147`483`648
	long		4bytes	-2`147`483`648			to	2`147`483`648
	long long 	8bytes	-9`223`327`036`854`775`807 	to 	9`223`327`036`854`775`807
	float		4bytes	3.4E+/--38
	double		8bytes	1.7E+/--308
	long double	16bytes

	uint8_t		1 byte	[0-255]
	uint16_t	2 bytes	[0-65535]
	uint32_t	4 bytes	[0-4294967295]
	uint64_t	8 bytes	[0-18446744073709551615]
	
```


### Comparison
```
== (equal to)
!= (not equal to)
< (less than)
> (greater than)
<= (less than or equal to)
>= (greater than or equal to)
```
### Boolean Operators
```
! (logical not)
&& (logical and)
|| (logical or)
```

### Arithmetic
```
% (remainder)
* (multiplication)
+ (addition)
- (subtraction)
/ (division)
= (assignment operator)

y += x;	y = y + x;
x -= 5;	x = x - 5;
x /= y;	x = x / y;
```

### Compound Operators
```
&= (compound bitwise and)
*= (compound multiplication)
++ (increment)
+= (compound addition)
-- (decrement)
-= (compound subtraction)
/= (compound division)
^= (compound bitwise xor)
|= (compound bitwise or)
```

### Array
```
	array_1[] = array_2[] 	// tikai pieliidzina vienu elementu
	struct 1 = struct 2 	//pieliidzina visus elementus	
```
### Bitwise Operators
```
& (bitwise and)
<< (bitshift left)
>> (bitshift right)
^ (bitwise xor)
| (bitwise or)
~ (bitwise not)


	int a = 5;        // binary: 0000000000000101
	int b = a << 3;   // binary: 0000000000101000, or 40 in decimal
	int c = b >> 3;   // binary: 0000000000000101, or back to 5 like we started with

   	1 <<  0  ==    1
    	1 <<  1  ==    2
    	1 <<  2  ==    4
    	1 <<  3  ==    8
	
	int x = -16;     		// binary: 1111111111110000
 	int y = x >> 3;  		// binary: 1111111111111110
	int y = (unsigned int)x >> 3;	// binary: 0001111111111110

```
# Pointers
### print
```
void print(char* C){
	int i=0;		//var bez
	while(*(C+i)!= '/0'){ 	//var vnk C++
		printf("%c",C[i]);
		i++;//var C++
	}
	printf("/n");
}
int main(){
	char C[20] = "Hello";
	print(C);
}
```
### to Servos

```
107. rinda	#include <Servo.h>
			Servo STAB_SERVO;
			Servo WING_SERVO;
			Servo RUDD_SERVO;
			Servo HOOK_SERVO;
		/* MK edit - izveidojam poiteru matricu uz klasēm "ptr_Servo", kura glabajas objektu adreses */
		Servo* ptr_Servo[] = [ &STAB_SERVO, &WING_SERVO, &RUDD_SERVO, &HOOK_SERVO ];


203. rinda	void toParameters( uint8_t gID, uint8_t pID, float value ){
  
    			switch(gID){
        			case 1:
            				Parameters.FLY_TIME[pID] = value;
		/* MK edit - darbina servo */
		if ( pID > 4) ptr_Servo[PID]->write(value);

            				break;
				case 2:
            				Parameters.CIR[pID] = (uint8_t)value;
            				break;
```
