		//Pro Mini/UNO
		//3.3V 8MHz
		//Programmer - ArduinoISP
		//Bootload programmer USB ASP
/*

LED - implenent millis();
DONE - create LED blink function
BMP - find adress
BMP - receive data
BMP - filter data to meters
BMP - set zero meters, with serial or when turned ON

FLASH - store data in memory
FLASH - read data from memory

GYRO - find adress
GYRO - receive data
GYRO - filter angle, acceleration

GPS - receive full data
GPS - filter to lat lon

RF - send data
RF - receive data-write in serial
RF - send diferent signal powers
RF - receive DT, move servo
RF - check transceive distance
RF - send FM tone

EEPROM - store board settings
APP - change parameteres thru serial

==possible PRODUCTS==
GPS standalone
RDT 
Alti Logger
RF Beacon
Timer

==possible MODES==
Timer RF BUNT mode
Timer RF FLY mode
Timer Gyro FLY mode
Timer Gyro BUNT mode
Timer Gyro RF info mode

		 BOARD			"GPS REV C"
		 STAB
		 BUTON		-	push button on board
		 LED 		-	LED on board
		 INT_GYRO	-	accelerometer INT pin
		 RF_NSS		-	RFM9x NSS PIN
		 DATA_RF	-	RFM9x data 1
		 DATA_RF_2	-	RFM9x data 2
		 RF_FM		-	send FM tone broadcast, should be DIO2 on RFM9x??
		 MEM_CS		-	Flash memory chip select - used for altimeter data
		 GPS_RX
		 GPS_TX

	serial_print_info();
	i2c_scaner();
	led_blink_loop(int pin, int on_time, int Off_time);
	buz_short(PIN, times); //active buz
	buz_tone(PIN, start_freq, end_freq);//passive buz
*/

#define GPS_RF_C
#define SOFT_VER "20201002"
#include "Pinmap.h"
#include "Timer_definitions.h"

void setup(){
	Serial.begin(9600);
	serial_print_info();
	delay(1000);
	pinMode(LED, OUTPUT); 
}

void loop(){
	led_blink_loop(LED, 10, 1000);
}
