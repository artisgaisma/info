# GPS
# #1 External
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

### PINS
| PIN  |NAME| description | default |def2|
| -----|------- | ------------- | -----------|---|
|7|EINT|IN. LOW - go sleep, HI-wake UP|keep Open|100nF to GND|
|23| NRES  |IN, Atcive LOW |keep Open|100nF to GND|
|30|WAKE|OUT, indicates to host, that module is active|keep Open|100nF to GND|

### TESTED:[2022-05-21]
- Cold Start - 2-4min to location
- Ho Start - 2-10sec to location
- ON - 25mAh, more than 35mAh pair times on found satelites
- SLEEP - ~0mAh
- `Tested in roome with GPS antenna amplfier for rooms`
- Schematic below used

![sim33ela](https://user-images.githubusercontent.com/51158344/137916027-474b5422-38fb-4456-961f-10396157c853.JPG)
![sim33 sch](https://user-images.githubusercontent.com/51158344/138419999-65183f27-9668-4ec8-950d-526db0a5e6d1.JPG)
![sim33 sch2](https://user-images.githubusercontent.com/51158344/138420013-870b1c21-981b-46c4-920d-e1e039ae7edf.JPG)

### L96
- 14×9.6×2.5
- 20mW tracking
- Acuracy 2.5m
- 8.00 EUR Aliexpress
- GPS, GLONASS, BeiDou, Galileo and QZSS 
- Built-in LNA for better sensitivity 
- https://forum.arduino.cc/t/active-gps-antenna-is-not-working/620041

![L96](https://user-images.githubusercontent.com/51158344/149634546-8bd748f0-82f6-4e4c-9641-865eb0845785.PNG)
![L96 board](https://user-images.githubusercontent.com/51158344/149634602-32388d08-a926-4cc8-801b-91ae201e9ae0.PNG)

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

![ATGM336H](https://user-images.githubusercontent.com/51158344/167366777-f677d3a8-8a06-43ee-b311-722940f16458.PNG)

## NEO-7M
- module
- 5eur 

![NEO7M](https://user-images.githubusercontent.com/51158344/138400514-3a82dfb9-b207-4987-85d8-9d5166c36129.JPG)

# MAX2769
- Universal GPS Receiver IC
- No External IF SAW or Discrete Filters Required

![MA2769 gps](https://user-images.githubusercontent.com/51158344/167366211-fdd045ac-1295-4621-9812-2966289f7967.PNG)

## RT9193 - LDO with loadswitch

![RT9193](https://user-images.githubusercontent.com/51158344/138400954-dc9b8b29-33b9-41eb-a0b5-8a2bf2fd1008.JPG)

## AT2659 
- GPS LNA Amplifier
- Similar to MAX2659



- 
## MAX2659ELT - Amplifier
- 0.80 Eur
![MAX2659](https://user-images.githubusercontent.com/51158344/138034729-88f1791f-ce43-4ffa-9191-df2d7859400a.JPG)

![AT6558 MAX2659](https://user-images.githubusercontent.com/51158344/138398950-6af3a6af-cdb8-4e70-bff7-b37c2414e89a.JPG)

![MAX2659](https://user-images.githubusercontent.com/51158344/138034729-88f1791f-ce43-4ffa-9191-df2d7859400a.JPG)

![AT6558 MAX2659](https://user-images.githubusercontent.com/51158344/138398950-6af3a6af-cdb8-4e70-bff7-b37c2414e89a.JPG)

# NMEA SENTENCES

![GPS NMEA ](https://user-images.githubusercontent.com/51158344/172376577-d6ba6987-87f7-40fb-8199-f9814604a374.png)

