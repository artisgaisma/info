/*
does not fully works with app2

notes:
	app1 -  in devices menu need to press once to open devices
	app 2 - in devices menu need to press twice on device to open setting
	graph works, if sent alone. sending with GPS, does no show graph
*/
#define TEST_HJ_185_REV_B

#define BUTON             4
#define LED               6
#define STROBE            5
#define VOLTAGE           A2    
#define STAB              3
#define BLE_TX            9
#define BLE_RX            8
#define BLE_SER_RX_ENABLE   7   
#define BLE_APP_CONFIG      10    

/*BLUETOOTH*********************************************/
#include <SoftwareSerial.h>
SoftwareSerial bleSerial(BLE_TX, BLE_RX); //(RxTx) of MCU

void ble_serial_begin(){
    digitalWrite(BLE_SER_RX_ENABLE, LOW);//works
} 

void ble_app_config_enable(){//enables ble config from app
    digitalWrite(BLE_APP_CONFIG, HIGH);
}

/*OTHER_Functions*********************************************/
void forward() {  //no NRF - works
  if (bleSerial && Serial) {
    int byte;
    while ((byte = bleSerial.read()) > 0) Serial.write((char)byte);//write writes bytes
    while ((byte = Serial.read()) > 0) bleSerial.write((char)byte);//does "print" works the same???
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

/*Sample parameters*********************************************/
void sample_parameters_1(){//short 1 group
	
	bleSerial.print("$");//DATA_PREFIX
	bleSerial.print(F("{<c:0, SLIDE,0>}"));//send group 0
	
	bleSerial.print(F("{<a:0,0,Stab,0,255,122,1>}"));//send param to group 0
	bleSerial.print("#");//DATA_SUFFIX
}
void sample_parameters_2(){//mid
    bleSerial.print("$");
    bleSerial.print("{<c:0,TOW,0>}");
    bleSerial.print("{<a:0,0,stab,0,255,0.0,1.0>}{<a:0,1,wing,0,255,1.0,1.0>}{<a:0,2,time,0,255,2.0,1.0>}");
    bleSerial.print("{<c:1,Circle,0>}");
    bleSerial.print("{<a:1,0,name_0,0,255,0.0,1.0>}{<a:1,1,name_1,0,255,1.0,1.0>}{<a:1,2,name_2,0,255,2.0,0.1>}");
    bleSerial.print("{<c:2,Accelerate,0>}");
    bleSerial.print("{<a:2,0,name_0,0,255,0.0,1.0>}{<a:2,1,name_1,0,255,1.0,1.0>}{<a:2,2,name_2,0,255,2.0,0.01>}");
    bleSerial.print("#");
}
void sample_parameters_3(){//long
    bleSerial.print("$");
    bleSerial.print("{<c:0,TOW,0>}");
    bleSerial.print(F("{<a:0,0,stab,0,255,0.0,1.0>}{<a:0,1,wing,0,255,1.0,1.0>}{<a:0,2,time,0,255,2.0,1.0>}"));
	
    bleSerial.print("{<c:1,Circle,0>}");
    bleSerial.print(F("{<a:1,0,name_0,0,255,0.0,1.0>}{<a:1,1,name_1,0,255,1.0,1.0>}{<a:1,2,name_2,0,25,2.0,0.1>}"));
	
    bleSerial.print("{<c:2,Accelerate,0>}");
    bleSerial.print(F("{<a:2,0,name_0,0,255,0.0,1.0>}{<a:2,1,name_1,0,255,1.0,1.0>}{<a:2,2,name_2,0,2,2.0,0.01>}"));
	
    bleSerial.print("#");
}
void sample_parameters_4(){//long
    bleSerial.println("$");
	
    bleSerial.println("{<c:0,TOW,0>}");
	bleSerial.println("{<c:1,Circle,0>}");
	bleSerial.println("{<c:2,Accelerate,0>}");

	bleSerial.println(F("{<a:0,0,stab,0,200,0,10>}"));
	bleSerial.println(F("{<a:0,1,wing,0,20,1,1>}"));
	bleSerial.println(F("{<a:0,2,time,0,2,2,0.1>}"));
	bleSerial.println(F("{<a:0,4,time,0,2,2,0.01>}"));

    bleSerial.println(F("{<a:1,0,stab,0,2,0,1.0>}"));
	bleSerial.println(F("{<a:1,1,wing,0,2,1,1.0>}"));
	bleSerial.println(F("{<a:1,2,time,0,2,2,1.0>}"));

    bleSerial.println(F("{<a:2,0,stab,0,255,0,1.0>}"));
	bleSerial.println(F("{<a:2,1,wing,0,255,1,1.0>}"));

    bleSerial.println("#");
}
void sample_button(){
	bleSerial.print(F("$"));
    bleSerial.println("{<c:3,Settings,1>}");
    bleSerial.println(F("{<a:3,0,Live config,0,0,0,0>}"));
	bleSerial.println(F("{<a:3,1,LED,0,0,0,0>}"));
	bleSerial.println(F("{<a:3,2,GPS,0,0,0,0>}"));
    bleSerial.print(F("#"));	
}
void sample_input(){
	bleSerial.print(F("$"));
    bleSerial.println("{<c:0,Ble Settings,2>}");
    bleSerial.println(F("{<a:0,0,Model Name,0,0,type in text,0>}"));
	bleSerial.println(F("{<a:0,1,Password,0,0,1234,0>}"));
	bleSerial.println(F("{<a:0,2,DT Channel,0,0,34,0>}"));
    bleSerial.print(F("#"));	
}
void sample_text(){
	bleSerial.print(F("$"));
    bleSerial.println("{<c:0,Ble Settings,3>}");
    bleSerial.println(F("{<a:0,0,Model Name,0,0,read only text,0>}"));
	bleSerial.println(F("{<a:0,1,Password,0,0,1234,0>}"));
	bleSerial.println(F("{<a:0,2,DT Channel,0,0,5678,0>}"));
    bleSerial.print(F("#"));	
}
void sample_graph(){
	
	bleSerial.print("$");
    bleSerial.print("{<g:id_0,Acelerometer,0.0,4.3>}");
	bleSerial.print("{<g:id_0,Acelerometer,1.0,6.3>}");
	bleSerial.print("{<g:id_0,Acelerometer,2.0,8.3>}");
    bleSerial.print("{<g:id_1,Altitude,0.0,0.3>}");
	bleSerial.print("{<g:id_1,Altitude,1.0,10.3>}");
	bleSerial.print("{<g:id_1,Altitude,2.0,20.9>}");
    bleSerial.print("#");
}
void sample_gps(){
	bleSerial.print("$");
    bleSerial.print("{<m:Plane,56.96932,23.081715,177.0>}");
    bleSerial.print("{<m:Drone,56.96972,23.077515,127.0>}");
    bleSerial.print("#");
	
}
/*setup preset*********************************************/
void setup_preset_1(){
        Serial.begin(38400);        //maybe change to 115200-no
        bleSerial.begin(38400);       //default 115200,..38400-sugested for 8MHz!!!


/*define I/O*/

        pinMode(LED, OUTPUT);
        pinMode(BUTON, INPUT_PULLUP);
        pinMode(BLE_SER_RX_ENABLE, OUTPUT);
        pinMode(BLE_APP_CONFIG, OUTPUT);
				
		ble_serial_begin();			//enabless RX on BLE module
		ble_app_config_enable();	//enables ble config from app

		Serial.println("Serial:38400" );
		Serial.print("BLE:38400" );

		on_short(LED, 3);			//blinks LED 3 times
		
		delay(1000);
}

/*MCU work starts here*********************************************/
void setup(){
	setup_preset_1();
	delay(50);
	//sample_parameters_1();//works
	//sample_parameters_2();//app received only last group with params
	delay(50);
	sample_parameters_4();//receives only last parameters
	//sample_button();//works
	delay(50);
	//sample_input();//works puurfekt
	//sample_text();
	//sample_graph();//graph works, if sent alone. sending with GPS, does no show anything.
	delay(50);
	//sample_gps();//works 2021-01-18
}

void loop(){

	//forward2();

}
