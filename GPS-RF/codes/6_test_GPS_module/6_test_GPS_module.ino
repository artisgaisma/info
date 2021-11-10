		//Pro Mini/UNO
		//3.3V 8MHz
		//Programmer - ArduinoISP
		//Bootload programmer USB ASP
/*
    pinMode(BUTON, INPUT_PULLUP);
    pinMode(BUZ, OUTPUT);
=====================================
LED - implenent millis();
DONE - create LED blink function
DONE - BMP - find adress
DONE - BMP - receive data
DONE - BMP - filter data to meters
DONE - BMP - set zero meters, with serial or when turned ON

      FLASH - store data in memory
      FLASH - read data from memory
      
      GYRO - find adress
      GYRO - receive data
      GYRO - filter angle, acceleration

DONE - GPS - receive full data
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
		 GPS_RX   - 2
		 GPS_TX   - 3


working functions:
	serial_print_info();
	i2c_scaner();
	led_blink_loop(int pin, int on_time, int Off_time);
	buz_short(PIN, times); //active buz
	buz_tone(PIN, start_freq, end_freq);//passive buz
*/
              /*
              BMP180 - I2C device found at address #define BMP180_ADR		0x77
KEYWORDS:
              begin  KEYWORD2
              startTemperature  KEYWORD2
              getTemperature  KEYWORD2
              startPressure KEYWORD2
              getPressure KEYWORD2
              sealevel  KEYWORD2
              altitude  KEYWORD2

              */

#define GPS_RF_C
#define SOFT_VER "20201123"

#include "Pinmap.h"
#include "Timer_definitions.h"

#include <SoftwareSerial.h>
SoftwareSerial GPS(GPS_TX, GPS_RX);//tMCU RX,TX



void read_button();
int counter = 0;
bool but_state = true;

void setup(){
	Serial.begin(9600);
	GPS.begin(9600);
  
	pinMode(LED, OUTPUT); 
	pinMode(BUTON, INPUT_PULLUP);
 
	serial_print_info();
	buz_long(LED, 1);//turns on LED pin
 
	delay(500);
}

void loop(){
    //read_button();
    while (GPS.available() > 0){
        byte gpsData = GPS.read();
        Serial.write(gpsData);
    }
    delay(2000);
    
}


/*
void read_button(){
  	if(but_state == true ){             
      	//Serial.println("OFF, state: ");
  	}else if(but_state == false ) {
        //buz_short(LED, 3);
        counter = counter+1;
      }
}
*/