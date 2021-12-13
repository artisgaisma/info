/*
 * 2020-09-09
 * changed serial speed to 9600
 * tested module with fft V3 timer soft - 11-FFT_simple  - 2020_07_15 - V3-MK
 * Serial comunication between BLE and Rx Tx pins
 * RxTx pins should be connected to Arduino board
 * 
 * Instalation:
 * 1-board package - https://raw.githubusercontent.com/sparkfun/Arduino_Boards/nrf/IDE_Board_Manager/package_sparkfun_index.json
 * 2-install boards - SparkFun nRF52 Boards
 * 3-select board - SparkFun nRF52832 Breakout
 * 5-add Lib "BLEPeripheral" ->manage libraries...
 * 4-triger bootloader on ble module: 
     * 4.1 press down RST and 06 button
     * 4.2 release RST button
     * 4.3 led should start blinking
     * 4.4 release 06 button
*/
#include <SPI.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
BLESerial bleSerial;
void setup() {
  bleSerial.setLocalName("FFT First");//this shows in Scanner - for APP FFT needs to be included in name..FFT..FFT 1 FFT First etc
  bleSerial.setDeviceName("FFT");//could not find this anywhere - sho var nedefineet
  Serial.begin(9600);//sazinjai ar MCU
  bleSerial.begin();
}

void loop() {
  bleSerial.poll();
  forward();
}

void forward() {
  if (bleSerial && Serial) {
    int byte;
    while ((byte = bleSerial.read()) > 0) Serial.write((char)byte);
    while ((byte = Serial.read()) > 0) bleSerial.write((char)byte);
  }
}
