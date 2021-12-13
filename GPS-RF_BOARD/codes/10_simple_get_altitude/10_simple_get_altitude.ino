/*
2021-03-23
This is sample for FFT-First+ timer.
On 328p mcu it takes 16%
At the time, FFT code uses 75% of flash.
FFT has alread EEPROM write fuinctions.

for readings and writings use new millis
long altitude_timer = 0;
addr = 0; // on each setup aadr is relative 0 - starts afters params ~400
like -> if(millis() - altitude_timer > PAUSE_BETWEEN_ALIT_REC){
	if(addr<= LAST_EEPROM_ADDRES){
		EEPROM.write(addr, a); //a to Int!!!
		addr++;
		altitude_timer = millis(); // reset back timer, to countown again
	}
}
*/

#include <Wire.h>
#include <SFE_BMP180.h>

SFE_BMP180 pressure;
double baseline;
double getPressure();
int counter = 0;

void setup(){

  baseline = getPressure();
  delay(500);

}

void loop(){
  
  double a,P;
  
  P = getPressure();//presure
  a = pressure.altitude(P,baseline);//altitude in meters

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
      }
    } 
  }
}
