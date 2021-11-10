//Board Pro mini 3v 8MHz
//Programmer - USB Tiny

/*
**********************************************************************
channels:
FFF0 - srvices
FFF1 - Reads from MCU - transparent
FFF2 - Sends to MCU - transparent
FFF3 - RX/TX chip to APP for configs

change settings in Bluetooth Monitor:
	Custom settings
	1-Service UUID - only one will be shows
		FFF0
	2-Read characteristics 
		FFF1 - Trasnparent
		FFF3 - direct from BLE chip
	3-Write charasteristics:
		FFF2 - Trasnparent
		FFF3 - to send to BLE direct-inbuilt comands etc...
		
		///man iisti nesanaaca piesleegties ar ble serial mon. betr straadaaja ar NRF conncet
		TEMPLATE - APP -> BLE <-
			FFF0
			FFF3
			FFF3
			
		TEMPLATE - APP -> BLE -> Transparant
			FFF0
			FFF1
			FFF2
recont BLE afters setting change!!!!!!!!!!!		


**********************************************************************

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


**********BAUD RATES*****************
forum: 
1	-	Using software serial at rates over 38400 is a recipe for disaster.
2	-	Even when using hardware serial, I understand the 3.3v Pro Mini struggles at 115200. 
		I assume this is because it runs at 8MHz.

SoftwareSerial:
		8MHz  up  to  57600
		16MHz up  to  115200
		
		38400 - suggestet from forums!
*/

/*2021-1-8
OK	-	1-test print/write - same output on com monitor
OK	-	2-turn on ble module, open module with NRF connect. write <RD_BAUD>. works with Nrf Connect
OK	-	3-set baudrate fro, MCU	<ST_BAUD=38400>
			set softserial baud the same you will programm to BLE - does not work!!!!!!!
					begin existing baud rate
					write set baud comand
					end softser
					begin new baud
			
			57600 - aatri suutot datus radaas kluuads loti aatri	serial.write
			38400 - ir kluudas, bet mazaak. bet ir taapat :/ 		serial.write
			ble receives good from ble. ble receives bad from MCU!!!
			nu itkaa no BLE naak daudz mazaak datu nekaa no mcu uz ble. naak tik izmainjas
			
			serial nomainiiju no 9600 uz 38400. taapat ir kluudas. arii leenaam rakstot
			tas laikam saitiits ar to 17 bitu suutiishanu/daliishanu pakaas!!
		
	-	Baud rate can be set from app any time, only if BLE_APP_CONFIG is enabled!!!!!
*/
/*2021-1-8 - Time to connect to FFT and add some settings!!!

YES, it works - with SoftwareSerial thru Bluetooth module.

*/
#define TEST-HJ-185-REV-B
#define ver 0.10

#define BUTON         		4
#define LED           		6
#define STROBE        		5
#define VOLTAGE       		A2    
#define STAB          		3
#define BLE_TX        		9
#define BLE_RX        		8
#define BLE_SER_RX_ENABLE   7		
#define BLE_APP_CONFIG    	10		

/*BLUETOOTH*********************************************/
#include <SoftwareSerial.h>
SoftwareSerial bleSerial(BLE_TX, BLE_RX); //(RxTx) of MCU

void setup(){
        Serial.begin(38400);				//maybe change to 115200-no
        bleSerial.begin(38400); 			//default 115200,..38400-sugested for 8MHz!!!
/*define I/O*/
        pinMode(LED, OUTPUT);
        pinMode(BUTON, INPUT_PULLUP);
        pinMode(BLE_SER_RX_ENABLE, OUTPUT);
        pinMode(BLE_APP_CONFIG, OUTPUT);
/*do some stuff here*/
		delay(50);

		Serial.println(ver);
        Serial.println("Serial:38400" );
        Serial.print("BLE:38400" );

        on_short(LED, 3);
		ble_serial_begin();//enabless RX on BLE module
		ble_app_config_enable();//enables ble config from app
		delay(50);
		
		manifest();//prints group and param to FFT app once

}

void loop(){
    //forward();//sends data between serial and softserial///write
	forward2();//print wroks the same as write
	//write_print();

}

void manifest(){//YES - it works :)
	bleSerial.print("$");//DATA_PREFIX
	bleSerial.print(F("{<c:0, SLIDE,0>}"));//send group 0
	bleSerial.print(F("{<a:0,0,Stab,0,255,122,1>}"));//send param to group 0
	bleSerial.print("#");//DATA_SUFFIX
}



/*dummy TEST functions*/
void write_print(){
	Serial.write("write");
	Serial.println("");
	delay(1000);
	Serial.println("print");
	delay(1000);
	
}
/*BLE_Functions*********************************************/
void ble_serial_begin(){
    digitalWrite(BLE_SER_RX_ENABLE, LOW);//works
} 
void ble_serial_end(){
    digitalWrite(BLE_SER_RX_ENABLE, HIGH);//works
}
void ble_app_config_enable(){//enables ble config from app
    digitalWrite(BLE_APP_CONFIG, HIGH);
}
void ble_app_config_off(){
    digitalWrite(BLE_APP_CONFIG, LOW);
}
void set_name(){
    bleSerial.write("<ST_NAME=FFT-MOD-1>");//works
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
void forward() {  //no NRF - works
  if (bleSerial && Serial) {
    int byte;
    while ((byte = bleSerial.read()) > 0) Serial.write((char)byte);//write writes bytes
    while ((byte = Serial.read()) > 0) bleSerial.write((char)byte);//does "print" works the same???
  }
}
void forward2() {  //works the same!!! all ok
  if (bleSerial && Serial) {
    int byte;
    while ((byte = bleSerial.read()) > 0) Serial.print((char)byte);//write writes bytes
    while ((byte = Serial.read()) > 0) bleSerial.print((char)byte);//does "print" works the same???
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







