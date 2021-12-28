# MCU`s:

`8MHZ - 8,000,000 cycles per second `int 4bytes -2`147`483`648	to 2`147`483`648 or uint32_t 4 bytes [0-4294967295]`

`EEPROM write cycle typically takes 3.3 ms to complete`

## TI
```		
		I/O		FLASH	RAM	BAND
CC1301 		10-30		32-128	16/20	Sub 1GHz, Wi-Fi
CC1350		10-33		128KB	20KB	Sub-1GHz, Wi-Fi, BLE	-	Launchpad ordered
CC1352R		28		352KB	80KB	Sub-1GHz AND 2.4GHz - DUAL BAND
CC1352P		26		352KB	80KB	Sub-1GHz AND 2.4GHz - integrated amplifier
CC2640R2	10-31		128KB	20KB	BLE 5, 2.4GHz

cc110L - transmit		<1.5e	300-928MHz
cc115L - transmit		<1e	300-928MHz
cc1150 - transmit		<2e 	300-900MHz
cc1200 - transceiv		<2.5e 	136-960MHz
cc1175 - trannmit		<2.5e 	136-960MHz - 10pcs ordere - 25eur

***CC1310 vs CC1350: Can be replacable each other.
The main advantage of the CC1352 over the CC1350 is the increased memory. 
With the increased memory you can run multiple stacks (TI15.4 and BLE). 
CC1350 can only run one stack at a time or one full stack and partially 
another for example, you can run the full TI 15.4 Stack with limited 
BLE functionality (only beaconing ad not connections)
```
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

# RF
|Model|mHZ|info|Price|
|---|---|---|---|
|RFM22B|433| stabilaaka frekvence|4.00|
|RFM69|433-915|LOS 500m|2.00|
|RFM95|868-915|LOS 2km-20km with yagi|4.00|
|RFM96|315-433|alternative DORJI|4.40|
|RFM98|433-470|LOS 40KM, Better than RFM22b, |4.00|
|SX1272|860-1000|same as RFM9x|4.00|


# MIC RF

| DIRECTION     | MODEL     | F 				  | MODE 		|PIN|PRICE|
| ------------- | ----------|-------			|-------	|--|--|
|TRANSMIT		    | MICRF 112	| 300-450 10dbm		| ASK/FSK	|10|0.75|
|				        | MICRF 113	| 300-450 10dbm		| ASK		|6|0.76|
|				        | MICRF 114 | 285-445 10dbm		| OOK		|6|0.66|
|				        | MICRF405	| 290-980 10dbm		| ASK/FSK|24|1.70|
|				        | SYN113/5	| 300-450 10dbm		| ASK		|6|0.45|
|	-			        | 		-	    | 		-			      |	-		  |-|   |
|RECEIVER		    | MICRF 211 | 380-450 -110dbm	| ASK OOK	|16||
|				| MICRF 213 | 300-350 -110dbm	| ASK OOK	|16||
| have sample	| MICRF 220 | 300-450 -110dbm	| ASK OOK	|16|1.2eur|
|				| MICRF 221 | 850-950 -109dbm	| ASK OOK	|16||





## TRANSMITER MICRF 1 XX
## RECEIVER MICRF 2 XX
## TRANSCEIVER  MICRF 5 XX

## ATA series:
![ATA series](https://user-images.githubusercontent.com/51158344/138142765-1cde7a77-1b08-47a2-b8ec-ad92109866c1.PNG)

### ATA8510/ATA8515
- UHF ASK/FSK Transceiver
- Low-band 310MHz to 318MHz, 418MHz to 477MHz
- High-band 836MHz to 956MHz



# GPS
## External
### SIM33ELA
- 14x9.6x2.15
- 0.5g
- Acuracy 2.5m
- 2.8-4.3V
- 20mA Trancking @3.3V
- 14uA Backup @3.3V
- Antenna SMD
- Similar to CAM-M8Q
- 14EUR - 2021

![sim33ela](https://user-images.githubusercontent.com/51158344/137916027-474b5422-38fb-4456-961f-10396157c853.JPG)
![sim33 sch](https://user-images.githubusercontent.com/51158344/138419999-65183f27-9668-4ec8-950d-526db0a5e6d1.JPG)
![sim33 sch2](https://user-images.githubusercontent.com/51158344/138420013-870b1c21-981b-46c4-920d-e1e039ae7edf.JPG)


### CAM-M8 Q/C
- 24eEUR
- 14x9.6x2mm
- Q - 2.7 V – 3.6 V
- C - 1.65 V – 3.6 V
- SMD Antenna
- GPS, Galileo, GLONASS, BeiDou
- no iCare

![CAM-M8](https://user-images.githubusercontent.com/51158344/138409612-94f1a5ce-24ca-4498-b1ba-26d95639bb6c.JPG)

# #2 on Timer
### MAX M8Q
- GPS Module
- NO need for external EEPROM chip
- 8.00EUR Ali
 
![max-m8q](https://user-images.githubusercontent.com/51158344/137916210-21ed5eb8-864c-41fd-a550-f15e447ca24a.JPG)

### ATGM336H
- GPS Module
- same footprint as MAX-M8
- 4.00EUR Ali
 
![ATGM336H](https://user-images.githubusercontent.com/51158344/138034519-4984bc9c-f3f6-4be6-b0d3-c1f13d73af36.JPG)

## NEO-7M
- module
- 5eur 

![NEO7M](https://user-images.githubusercontent.com/51158344/138400514-3a82dfb9-b207-4987-85d8-9d5166c36129.JPG)

## RT9193 - LDO with loadswitch

![RT9193](https://user-images.githubusercontent.com/51158344/138400954-dc9b8b29-33b9-41eb-a0b5-8a2bf2fd1008.JPG)

## AT2659 - Amplifier
- GPS LNA Amplifier
- Similar to MAX2659

## MAX2659ELT - Amplifier
- 0.80 Eur

![MAX2659](https://user-images.githubusercontent.com/51158344/138034729-88f1791f-ce43-4ffa-9191-df2d7859400a.JPG)

![AT6558 MAX2659](https://user-images.githubusercontent.com/51158344/138398950-6af3a6af-cdb8-4e70-bff7-b37c2414e89a.JPG)

# Charger

|part No|Manufact|Price|info|
|--|--|--|--|
|BQ25170|TI|0.39usd|Pick&Place|

# Boost
|part No|Manufact|Price|info|
|--|--|--|--|
|TPS61023DRLT|TI|0.37|Pick&Place|

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
