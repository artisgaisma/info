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

Atmega 32U4	26	8/16	32KB	1KB	2.5	5?	1
Atmega 328p	23	1/20	32KB	1KB	2KB	6	1	1.5EUR
Atmega 1281 	54	8/16	128KB	4KB	8KB	6	2	6EUR
Atmega 2561	54	8/16	256KB	4KB	8KB	6	2
Atmega 1280	86	8/16	128KB	4KB	8KB	12	4
Atmega 2560	86	8/16	256KB	4KB	8KB	12	4
Atxmega256a3u 	64p	32	256	4KB	16KB	22	7	8eur	Allard QuadAnt taimeris

STM32F103x8	32	8-78	64KB	---	20KB	13	3	Have sample Board

SAMD21			48MHz	256KB	NO	32KB 12	

nRF52832
```
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
