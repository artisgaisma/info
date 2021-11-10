#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

float pressure = 0.0;
float tempC = 0.0;
float altitude = 0.0;
void BmpSensorRead(float* pressure, float* tempC, float* altitude);

double baseline;

void setup(void)
{
  Serial.begin(9600);
  baseline = bmp.readAltitude();
}
void loop(void)
{
  BmpSensorRead(&pressure, &tempC, &altitude);
  delay(100);
}

void BmpSensorRead(float* pressure, float* tempC, float* altitude)
{
  *tempC = bmp.readTemperature();
  //Serial.print("T:");
  //Serial.println(*tempC);
  //Serial.println(" *C");

  *pressure = bmp.readPressure() / 100.0;
  
  *altitude = (bmp.readAltitude() - baseline);
  //Serial.print("A:");
  Serial.println(*altitude);

}
