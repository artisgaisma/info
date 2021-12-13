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
#define SOFT_VER "20201007"
#include "Pinmap.h"
#include "Timer_definitions.h"

#include <Wire.h>
#include <SFE_BMP180.h>
SFE_BMP180 pressure;
double baseline;
double getPressure();
void setup(){
	Serial.begin(9600);
	serial_print_info();
	pinMode(LED, OUTPUT); 
	pinMode(BUTON, INPUT_PULLUP);
  buz_short(LED, 3);
  
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }
  baseline = getPressure();
  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb"); 

  
  delay(1000);
}

void loop(){

  double a,P;
  P = getPressure();
  a = pressure.altitude(P,baseline);
  
  if (a >= 0.0) Serial.print(" "); // add a space for positive numbers
  Serial.print(a,1);
  Serial.println(" meters");
  
  buz_short(LED, 1);
  delay(100);
}
double getPressure()
{
  char status;
  double T,P,p0,a;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
