/*
arduino sends out parameters 1 second after power up
arduino does not read here commands form app

see void setup() - to add remowe parameters and groups to send out

does not fully works with app2

notes:
	app 1 -  in devices menu need to press once to open devices
	app 2 - in devices menu need to press twice on device to open settings
	graph works, if sent alone. sending with GPS, does no show graph
	GPS does not show phone location
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

void ble_serial_begin(){
    digitalWrite(BLE_SER_RX_ENABLE, LOW);//works
} 

void ble_app_config_enable(){//enables ble config from app
    digitalWrite(BLE_APP_CONFIG, HIGH);
}

/*OTHER_Functions*********************************************/


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
	
	Serial.print("$");//DATA_PREFIX
	Serial.print(F("{<c:0, SLIDE,0>}"));//send group 0
	
	Serial.print(F("{<a:0,0,Stab,0,255,122,1>}"));//send param to group 0
	Serial.print("#");//DATA_SUFFIX
}
void sample_parameters_2(){//mid
    Serial.print("$");
    Serial.print("{<c:0,TOW,0>}");
    Serial.print("{<a:0,0,stab,0,255,0.0,1.0>}{<a:0,1,wing,0,255,1.0,1.0>}{<a:0,2,time,0,255,2.0,1.0>}");
    Serial.print("{<c:1,Circle,0>}");
    Serial.print("{<a:1,0,name_0,0,255,0.0,1.0>}{<a:1,1,name_1,0,255,1.0,1.0>}{<a:1,2,name_2,0,255,2.0,0.1>}");
    Serial.print("{<c:2,Accelerate,0>}");
    Serial.print("{<a:2,0,name_0,0,255,0.0,1.0>}{<a:2,1,name_1,0,255,1.0,1.0>}{<a:2,2,name_2,0,255,2.0,0.01>}");
    Serial.print("#");
}
void sample_parameters_3(){//long
    Serial.print("$");
    Serial.print("{<c:0,TOW,0>}");
    Serial.print(F("{<a:0,0,stab,0,255,0.0,1.0>}{<a:0,1,wing,0,255,1.0,1.0>}{<a:0,2,time,0,255,2.0,1.0>}"));
	
    Serial.print("{<c:1,Circle,0>}");
    Serial.print(F("{<a:1,0,name_0,0,255,0.0,1.0>}{<a:1,1,name_1,0,255,1.0,1.0>}{<a:1,2,name_2,0,25,2.0,0.1>}"));
	
    Serial.print("{<c:2,Accelerate,0>}");
    Serial.print(F("{<a:2,0,name_0,0,255,0.0,1.0>}{<a:2,1,name_1,0,255,1.0,1.0>}{<a:2,2,name_2,0,2,2.0,0.01>}"));
	
    Serial.print("#");
}
void sample_parameters_4(){//long
    Serial.println("$");
	
    Serial.println("{<c:0,TOW,0>}");
	Serial.println("{<c:1,Circle,0>}");
	Serial.println("{<c:2,Accelerate,0>}");

	Serial.println(F("{<a:0,0,stab,0,200,0,10>}"));
	Serial.println(F("{<a:0,1,wing,0,20,1,1>}"));
	Serial.println(F("{<a:0,2,time,0,2,2,0.1>}"));
	Serial.println(F("{<a:0,4,time,0,2,2,0.01>}"));

    Serial.println(F("{<a:1,0,stab,0,2,0,1.0>}"));
	Serial.println(F("{<a:1,1,wing,0,2,1,1.0>}"));
	Serial.println(F("{<a:1,2,time,0,2,2,1.0>}"));

    Serial.println(F("{<a:2,0,stab,0,255,0,1.0>}"));
	Serial.println(F("{<a:2,1,wing,0,255,1,1.0>}"));

    Serial.println("#");
}
void sample_button(){
	Serial.print(F("$"));
    Serial.println("{<c:3,Settings,1>}");
    Serial.println(F("{<a:3,0,Live config,0,0,0,0>}"));
	Serial.println(F("{<a:3,1,LED,0,0,0,0>}"));
	Serial.println(F("{<a:3,2,GPS,0,0,0,0>}"));
    Serial.print(F("#"));	
}
void sample_input(){
	Serial.print(F("$"));
    Serial.println("{<c:0,Ble Settings,2>}");
    Serial.println(F("{<a:0,0,Model Name,0,0,type in text,0>}"));
	Serial.println(F("{<a:0,1,Password,0,0,1234,0>}"));
	Serial.println(F("{<a:0,2,DT Channel,0,0,34,0>}"));
    Serial.print(F("#"));	
}
void sample_text(){
	Serial.print(F("$"));
    Serial.println("{<c:0,Ble Settings,3>}");
    Serial.println(F("{<a:0,0,Model Name,0,0,read only text,0>}"));
	Serial.println(F("{<a:0,1,Password,0,0,1234,0>}"));
	Serial.println(F("{<a:0,2,DT Channel,0,0,5678,0>}"));
    Serial.print(F("#"));	
}
void sample_graph(){
	
	Serial.print("$");
    Serial.print("{<g:id_0,Acelerometer,0.0,4.3>}");
	Serial.print("{<g:id_0,Acelerometer,1.0,6.3>}");
	Serial.print("{<g:id_0,Acelerometer,2.0,8.3>}");
    Serial.print("{<g:id_1,Altitude,0.0,0.3>}");
	Serial.print("{<g:id_1,Altitude,1.0,10.3>}");
	Serial.print("{<g:id_1,Altitude,2.0,20.9>}");
    Serial.print("#");
}
void sample_gps(){
	Serial.print("$");
    Serial.print("{<m:Plane,56.96932,23.081715,177.0>}");
    Serial.print("{<m:Drone,56.96972,23.077515,127.0>}");
    Serial.print("#");
	
}
/*setup preset*********************************************/
void setup_preset_1(){
        Serial.begin(38400);        //115200 for nRF52xx module
        Serial.begin(38400);       //default 115200,..38400-sugested for 8MHz!!!
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
