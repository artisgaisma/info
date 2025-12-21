		//Pro Mini/UNO
		//3.3V 8MHz
		//Programmer - ArduinoISP
		//Bootload programmer USB ASP
/*
    pinMode(BUTON, INPUT_PULLUP);
    pinMode(BUZ, OUTPUT);
=====================================
      - LED implenent millis();
DONE -  LED blink function
      - LED PWM - 2020-11-23

DONE - BMP - find adress
DONE - BMP - receive data
DONE - BMP - filter data to meters
DONE - BMP - set zero meters, with serial or when turned ON

      FLASH - store data in memory
      FLASH - read data from memory
      
      GYRO - find adress
      GYRO - receive data
      GYRO - filter angle, acceleration

DONE - GPS - receive full data  - 2020-11-23
DONE - GPS - filter to lat lon  - 2020-11-23

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
		 GPS_RX   - 2!!!
		 GPS_TX   - 3!!!


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
/*
check coordinates in google maps
57.2330876, 021.3307052

2020-11-23 - works fine
atrada loti tuvu-1 kvartaals ventspilii
tieshaa tuvumaa bija 5staavu maaja
*/
#define GPS_RF_C
#define SOFT_VER "20201123_3"

#include "Pinmap.h"
#include "Timer_definitions.h"

              //GPS Library
              #include <TinyGPS++.h>
              TinyGPSPlus gps;
              #include <SoftwareSerial.h>
              SoftwareSerial GPS(GPS_TX, GPS_RX);//tMCU RX,TX
              double Lat;
              double Long;
              int day, month, year;
              int hour, minute, second;
              typedef struct {
              float LLat;//alt
              float LLon;
              float TTemperature;
              float Altitude;
            }
            Payload;
            Payload theData;


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
      Get_GPS();

      Serial.print("Lattitude [");
      //Serial.print(theData.LLat,6);
      Serial.print(Lat,6);
      Serial.println("]");
      Serial.print("Longitude [");
      //Serial.print(theData.LLon,6);
      Serial.print(Long,6);
      Serial.println("]");
}



static void smartDelay(unsigned long ms)
{
      unsigned long start = millis();
      do
      {
        while (GPS.available())
        gps.encode(GPS.read());
      } while (millis() - start < ms);
}

void Get_GPS()
{
      if (gps.location.isValid() == 1) {
          Lat = gps.location.lat();
          Serial.println(Lat);
          Long = gps.location.lng();
          Serial.println(Long);
          theData.LLat = Lat;
          theData.LLon = Long;
      }
      smartDelay(1000);
      if (millis() > 5000 && gps.charsProcessed() < 10)
      {
         Serial.println(F("No GPS detected: check wiring."));
      }
}
