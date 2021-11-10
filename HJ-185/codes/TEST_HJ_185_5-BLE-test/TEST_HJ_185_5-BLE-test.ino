//BootLoader - Pro Mini/UNO
//Board Pro mini 3v 8MHz
//Programmer - USB Tiny
/*
channels:
FFF1 - APP Reads from MCU
FFF2 - APP Sends to MCU
FFF3 - RX/TX chip to APP for configs

PIN14/P0.16 - HIGH - Config mode Enable.HIGH-Enable, LOW-Disable
PIN9/P0.02 - Ble Serial RX Enable. LOW-enable, HIGH-Disable
VCC - 1.7-3.6V
<RD_NAME> <ST_NAME=xxx>
<RD_ADV_ONOFF> <ST_ADV_ONOFF=X> X=1:BroadCast ON, X=0 BroadCast Off
<RD_ADV_DATA> <ST_ADV_DATA=xx..xx> Set/Read Data about Broadcasting-HEX
<RD_ADV_GAP> <ST_ADV_GAP=xx..xx> Broadcast gab 20-10`000ms
<RD_CLIENT_LINK> <ST_CLIENT_LINK=0> Disconnect from Host. Reads MAC
<RD_CON_MIN_GAP> <ST_CON_MIN_GAP=xx..xx> Min connection gab 7.5-4000ms
<RD_CON_MAX_GAP> <ST_CON_MAX_GAP=yy..yy> Max connection Gap 7.5-4000ms
<RD_CON_TIMEOUT> <ST_CON_TIMEOUT=xx..xx> Timeout of slave connection 500-8000ms
<RD_SECRET> <ST_SECRET=xx..xx> <ST_CLEAR_SECRET=1> Pasword. Up to 8 chars
<RD_SECRET_TIMEOUT> <ST_SECRET_TIMEOUT=xx..xx> Pasword timeout 1-255s
<RD_SOFT_VERSION>
<RD_TX_POWER> <ST_TX_POWER=+4> -40/-20/-16/-12/-8/-4/0+3/+4dBm
<ST_RESET_BLE> Resest BLE after 500ms
<RD_BAUD> <ST_BAUD=xx..xx> 1200/2400/4800/9600/19200/38400/57600/115200/230400/460800/921600 bps
<RD_BLE_MAC>
<ST_OWN_MAC=xxxxxxxxxxxx> Set User-Defined MAC address
<ST_OWN_MAC=0> Cancel User-Defined MAC address
<RD_T1> <ST_T1=x> 1-255ms T1 value – App receiving indicator PIN10
<RD_UART_EVEN> <ST_UART_EVEN=x> 1-enables even parity. 0-disabled
<RD_AD_WAIT> <ST_AD_WAIT=x> Set/Red PIN10 puldown delay 1-255ms
<RD_UART_PULL> <ST_UART_PULL=x> BLE RX pullup – 1-Enable, 0-Disable Internal Pull-up - PIN5
<ST_FACTORY=1> RESTORE FACTORY SETTINGS
<RD_WAKEUP_HIGH> <ST_WAKEUP_HIGH=x> 1-Active HIGH 0-Active LOW(default) PIN9

    // SoftSerial: 115200 on 16Mhz and 57600 on 8Mhz.

*/
#define ver 0.3

#define BUTON         4
#define LED           10
#define SRTOBE        5
#define VOLTAGE       A2    
#define POT           A3
#define STAB          3
#define BLE_TX        9
#define BLE_RX        8
#define BLE_SER_RX_ENABLE   7             //BLE PIN9 - LOW-enable, HIGH-Disable
#define BLE_APP_CONFIG    6               //BLE PIN14 - HIGH-Enable, LOW-Disable

/*BLUETOOTH*********************************************/
#include <SoftwareSerial.h>
SoftwareSerial bleSerial(BLE_TX, BLE_RX); //(RxTx) of MCU
/*BLUETOOTH*********************************************/

void ble_serial_begin();
void set_name();

void setup(){
        Serial.begin(9600);//maybe change to 115200
        bleSerial.begin(57600); //default 115200(on fresh ble mcu??)
/*define I/O*/
        pinMode(LED, OUTPUT);
        pinMode(BUTON, INPUT_PULLUP);
        pinMode(BLE_SER_RX_ENABLE, OUTPUT);
        pinMode(BLE_APP_CONFIG, OUTPUT);
/*do some stuff here*/
        Serial.println("HJ-185 Module" );
        Serial.println("Serial:115200" );
        Serial.println("BLE:115200" );
        Serial.print(ver);
        Serial.print("\t");
        Serial.println();
        on_short(LED, 2);
		
        Serial.println("turning on BLE module" );
		digitalWrite(BLE_SER_RX_ENABLE, LOW);
		//digitalWrite(BLE_APP_CONFIG, HIGH);
        delay(1000);
		
        Serial.println("Set new name" );
        set_name();
        delay(1000);
		
        Serial.println("loop started" );
}

void loop(){

    while(bleSerial.available() > 0)
	{
        Serial.write(bleSerial.read());
	} 
}



/*BLE_Functions*********************************************/
void ble_serial_begin(){
    digitalWrite(BLE_SER_RX_ENABLE, LOW);//works
} 
void ble_serial_end(){
    digitalWrite(BLE_SER_RX_ENABLE, HIGH);//works
}

void ble_app_config_enable(){
    digitalWrite(BLE_APP_CONFIG, HIGH);
}
void ble_app_config_off(){
    digitalWrite(BLE_APP_CONFIG, LOW);
}
void set_name(){
    bleSerial.write("<ST_NAME=FFT-KON2>");//works
}
void set_password_def(){
    bleSerial.write("<ST_SECRET=1234>");//up to 8 chars
}
void clr_def_password(){
    bleSerial.write("<ST_CLEAR_SECRET=1>");
}
void set_broadcast_on(){
    bleSerial.write("<ST_ADV_ONOFF=1>");
}
void set_broadcast_off(){
    bleSerial.write("<ST_ADV_ONOFF=0>");
}

/*OTHER_Functions*********************************************/
void ble_read(){//no GPS  - nestraadaa----------------
    while (bleSerial.available() > 0)
	{
        byte bleData = bleSerial.read();
        Serial.write(bleData);
    }
    delay(30);
}

void gps_read_2(){//no GPS  - works
    while(bleSerial.available() > 0)
        {
        Serial.write(bleSerial.read());
        }  
}

void forward() {  //no NRF - works
  if (bleSerial && Serial) {
    int byte;
    while ((byte = bleSerial.read()) > 0) Serial.write((char)byte);
    while ((byte = Serial.read()) > 0) bleSerial.write((char)byte);
  }
}



void on_short(uint8_t PIN, uint8_t times){//short LED Blinks
      for(int i=0; i<times; i++){
          digitalWrite(PIN, HIGH);
          delay(30);
          digitalWrite(PIN, LOW);
          delay(200);
      }   
}
