/*
app sample code for Software serial arduino
main code added from mk3_3.ino
2021-01-15
app v1  - works fine
app v2  - does not read alla data
BLE INFO:
GATT services
Service UUID      0000fff0-0000-1000-8000-00805f9b34fb
Read characterictics UUID 0000fff1-0000-1000-8000-00805f9b34fb
Write characteriscics UUID  0000fff2-0000-1000-8000-00805f9b34fb
Bluetooth comunication UUID   0000fff3-0000-1000-8000-00805f9b34fb
*/
#define LED                 6
#define BLE_TX              9
#define BLE_RX              8
#define BLE_SER_RX_ENABLE   7   
#define BLE_APP_CONFIG      10 

#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial bleSerial(BLE_TX, BLE_RX); //(RxTx) of MCU
void ble_serial_begin(){
    digitalWrite(BLE_SER_RX_ENABLE, LOW);//works
} 
void ble_app_config_enable(){//enables ble config from app
    digitalWrite(BLE_APP_CONFIG, HIGH);
}
/* ======================================== CONST ========================================*/

const String INPUT_RESET = "reset";       // Mask for resetting EEPROM to 0;
const String INPUT_SAVE = "save";         // Mask for saving data in EEPROM;
const String INPUT_GET = "get";           // Mask for reading data;
const String INPUT_GET_GPS = "get_gps";       // Mask for sending GPS data;
const String INPUT_GET_ALT = "get_alt";       // Mask for sending Altitude data;

const String INPUT_CREATE = "<c:";        // Mask for creating a group;
const String INPUT_UPDATE = "<u:";        // Mask for updating a parameter inside a group;
const String INPUT_SUFFIX = ">";          // Mask for input suffix;

const String DATA_PREFIX = "$";           // Mask for data beginning
const String DATA_SUFFIX = "#";           // Mask for data ending

const byte START_MARKER = 0x7b;           // Translates to: {;
const byte END_MARKER = 0x7d;             // Translates to: };
const String MESSAGE_PREFIX = "{";        // Message beginning
const String MESSAGE_SUFFIX = "}";        // Message ending

const uint8_t groupCount = 10;
const uint8_t parameterCount = 4;
const char BOARD_NAME[] = "FFT-MICRO";
const uint8_t dataRegion = strlen(BOARD_NAME) + 1;        // +1 = NULL, bet es nezinu vai vajag, iespejmas var bez

/* ======================================== CONST ========================================*/

boolean isDataReceived    = false;          // Check if any new data is received
String stream;                                // Holds the received String
uint8_t index       = 0;                // Index of data beginning


enum group_types {
  SLD,  // 0 - slider
  BUT,  // 1 - button 
  INP,  // 2 - input text
  TXT   // 3 - text only
};
const uint8_t GROUP_TYPES[groupCount] = { SLD, SLD, SLD, SLD, SLD, SLD, SLD, SLD, SLD, SLD };

const String GROUP_NAME[groupCount] = { BOARD_NAME,"TOW","CIR","ACC","UP1","UP2","BUNT","FAST","FLY","DT" };

struct group_param {
    float     TOW[3];
    float     CIR[3];
    float     ACC[3];
    float     UP1[3];
    float     UP2[3];
    float     BUNT[3];
    float     FAST[3];
    uint8_t   FLY[3];
    uint8_t   DT[4];
} Parameters;

void toParameters( uint8_t gID, uint8_t pID, float value ){
  
    switch(gID){
        case 1:
            Parameters.TOW[pID] = value;
            break;
        case 2:
            Parameters.CIR[pID] = value;
            break;
        case 3:
            Parameters.ACC[pID] = value;
            break;
        case 4:
            Parameters.UP1[pID] = value;
            break;
        case 5:
            Parameters.UP2[pID] = value;
            break;
        case 6:
            Parameters.BUNT[pID] = value;
            break;
        case 7:
            Parameters.FAST[pID] = value;
            break;
        case 8:
            Parameters.FLY[pID] = (uint8_t)value;
            break;
        case 9:
            Parameters.DT[pID] = (uint8_t)value;
            break;
        default:
            Serial.print("{no such parameter}");
    }
    
}

void sample_parameters_1(){//short 1 group
	
	//bleSerial.print("$");//DATA_PREFIX
	//bleSerial.print(F("{<c:0, SLIDE,0>}"));//send group 0
	
	bleSerial.print(F("{<a:0,0,Stab,0,255,122,1>}"));//send param to group 0
	//bleSerial.print("#");//DATA_SUFFIX
}
void sample_parameters_2(){//mid
    //bleSerial.print("$");
    //bleSerial.print("{<c:0,TOW,0>}");
    bleSerial.print("{<a:0,0,stab,0,255,0.0,1.0>}{<a:0,1,wing,0,255,1.0,1.0>}{<a:0,2,time,0,255,2.0,1.0>}");
    //bleSerial.print("{<c:1,Circle,0>}");
    bleSerial.print("{<a:1,0,name_0,0,255,0.0,1.0>}{<a:1,1,name_1,0,255,1.0,1.0>}{<a:1,2,name_2,0,255,2.0,0.1>}");
    //bleSerial.print("{<c:2,Accelerate,0>}");
    bleSerial.print("{<a:2,0,name_0,0,255,0.0,1.0>}{<a:2,1,name_1,0,255,1.0,1.0>}{<a:2,2,name_2,0,255,2.0,0.01>}");
    //bleSerial.print("#");
}
void sample_parameters_3(){//long
    //bleSerial.print("$");
    //bleSerial.print("{<c:0,TOW,0>}");
    bleSerial.print(F("{<a:0,0,stab,0,255,0.0,1.0>}{<a:0,1,wing,0,255,1.0,1.0>}{<a:0,2,time,0,255,2.0,1.0>}"));
	
    //bleSerial.print("{<c:1,Circle,0>}");
    bleSerial.print(F("{<a:1,0,name_0,0,255,0.0,1.0>}{<a:1,1,name_1,0,255,1.0,1.0>}{<a:1,2,name_2,0,25,2.0,0.1>}"));
	
    //bleSerial.print("{<c:2,Accelerate,0>}");
    bleSerial.print(F("{<a:2,0,name_0,0,255,0.0,1.0>}{<a:2,1,name_1,0,255,1.0,1.0>}{<a:2,2,name_2,0,2,2.0,0.01>}"));
	
    //bleSerial.print("#");
}
void sample_parameters_4(){//long
    	//bleSerial.println("$");
	
    	//bleSerial.println("{<c:0,TOW,0>}");
	//bleSerial.println("{<c:1,Circle,0>}");
	//bleSerial.println("{<c:2,Accelerate,0>}");

	bleSerial.println(F("{<a:0,0,stab,0,200,0,10>}"));
	bleSerial.println(F("{<a:0,1,wing,0,20,1,1>}"));
	bleSerial.println(F("{<a:0,2,time,0,2,2,0.1>}"));
	bleSerial.println(F("{<a:0,4,time,0,2,2,0.01>}"));

	bleSerial.println(F("{<a:1,0,stab,0,2,0,1.0>}"));
	bleSerial.println(F("{<a:1,1,wing,0,2,1,1.0>}"));
	bleSerial.println(F("{<a:1,2,time,0,2,2,1.0>}"));

    bleSerial.println(F("{<a:2,0,stab,0,255,0,1.0>}"));
	bleSerial.println(F("{<a:2,1,wing,0,255,1,1.0>}"));

    	//bleSerial.println("#");
}
void sample_parameters_5(){
	        /* this format almost follows 20byte recommendation */
				
		bleSerial.print(F("{<a:1,0,Stab,0,180,"));     bleSerial.print(Parameters.TOW[0]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:1,1,Wing,0,180,"));     bleSerial.print(Parameters.TOW[1]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:1,2,Duration,0,10,"));  bleSerial.print(Parameters.TOW[2]);  bleSerial.print(F(",0.1>}"));
        // CIR
        bleSerial.print(F("{<a:2,0,Stab,0,180,"));     bleSerial.print(Parameters.CIR[0]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:2,1,Wing,0,180,"));     bleSerial.print(Parameters.CIR[1]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:2,2,Duration,0,10,"));  bleSerial.print(Parameters.CIR[2]);  bleSerial.print(F(",0.1>}"));
        // ACC
        bleSerial.print(F("{<a:3,0,Stab,0,180,"));     bleSerial.print(Parameters.ACC[0]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:3,1,Wing,0,180,"));     bleSerial.print(Parameters.ACC[1]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:3,2,Duration,0,10,"));  bleSerial.print(Parameters.ACC[2]);  bleSerial.print(F(",0.1>}"));
        // UP1
        bleSerial.print(F("{<a:4,0,Stab,0,180,"));     bleSerial.print(Parameters.UP1[0]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:4,1,Wing,0,180,"));     bleSerial.print(Parameters.UP1[1]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:4,2,Duration,0,10,"));  bleSerial.print(Parameters.UP1[2]);  bleSerial.print(F(",0.1>}"));
        delay(50); // TODO : is it needed??
        // UP2
        bleSerial.print(F("{<a:5,0,Stab,0,180,"));     bleSerial.print(Parameters.UP2[0]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:5,1,Wing,0,180,"));     bleSerial.print(Parameters.UP2[1]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:5,2,Duration,0,10,"));  bleSerial.print(Parameters.UP2[2]);  bleSerial.print(F(",0.1>}"));
        // BUNT
        bleSerial.print(F("{<a:6,0,Stab,0,180,"));     bleSerial.print(Parameters.BUNT[0]); bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:6,1,Wing,0,180,"));     bleSerial.print(Parameters.BUNT[1]); bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:6,2,Duration,0,10,"));  bleSerial.print(Parameters.BUNT[2]); bleSerial.print(F(",0.1>}"));
        // FLY - 1
        bleSerial.print(F("{<a:7,0,Stab,0,180,"));     bleSerial.print(Parameters.FAST[0]); bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:7,1,Wing,0,180,"));     bleSerial.print(Parameters.FAST[1]); bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:7,2,Duration,0,10,"));  bleSerial.print(Parameters.FAST[2]); bleSerial.print(F(",0.1>}"));
        // FLY - 2
        bleSerial.print(F("{<a:8,0,Stab,0,180,"));     bleSerial.print(Parameters.FLY[0]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:8,1,Wing,0,180,"));     bleSerial.print(Parameters.FLY[1]);  bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:8,2,Wing,0,180,"));     bleSerial.print(Parameters.FLY[2]);  bleSerial.print(F(",1>}"));
        // DT
        bleSerial.print(F("{<a:9,0,Stab,0,180,"));     bleSerial.print(Parameters.DT[0]);   bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:9,1,Wing,0,180,"));     bleSerial.print(Parameters.DT[1]);   bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:9,2,Fly Time,0,60,"));  bleSerial.print(Parameters.DT[2]);   bleSerial.print(F(",1>}"));
        bleSerial.print(F("{<a:9,3,Test Time,0,60,")); bleSerial.print(Parameters.DT[3]);   bleSerial.print(F(",1>}"));
		
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

void getGps(){
	bleSerial.print("$");
    bleSerial.print("{<m:Plane,56.96932,23.081715,177.0>}");
    bleSerial.print("{<m:Drone,56.96972,23.077515,127.0>}");
    bleSerial.print("#");	
}
	
void getAlt(){
	bleSerial.print("$");
    bleSerial.print("{<g:id_0,Acelerometer,0.0,4.3>}");
	bleSerial.print("{<g:id_0,Acelerometer,1.0,6.3>}");
	bleSerial.print("{<g:id_0,Acelerometer,2.0,8.3>}");
    bleSerial.print("{<g:id_1,Altitude,0.0,0.3>}");
	bleSerial.print("{<g:id_1,Altitude,1.0,10.3>}");
	bleSerial.print("{<g:id_1,Altitude,2.0,20.9>}");
    bleSerial.print("#");
}

void getData(){

/* ---30.07.2020--- EEPROM first memory cell will distinguish if there is something written from app
                  * in new AVR MCU eeprom default is 255
                  * saveData() will put 0 in first memory cell
*/
    if ( !(EEPROM.read(0)) ){
        EEPROM.get(dataRegion+1, Parameters);   // load memory
    }
    else {
        Parameters = {0};                       // use default values
    }
    
    bleSerial.print(DATA_PREFIX);
    if ( groupCount > 0 ){
        for( uint8_t n = 0; n < groupCount; n++ ){
            bleSerial.print(MESSAGE_PREFIX);
            bleSerial.print( INPUT_CREATE + n + "," + GROUP_NAME[n] + "," + GROUP_TYPES[n] + INPUT_SUFFIX );
            bleSerial.print(MESSAGE_SUFFIX);
        }
        delay(50);
    }
    if ( parameterCount > 0 ){
    
    sample_parameters_5();
    
    }
    bleSerial.print(DATA_SUFFIX);

}

void updateParameter( String data ){
  
    if ( parameterCount > 0 ){   
        data.replace(INPUT_UPDATE, "");                                   // Clean input
        data.replace(INPUT_SUFFIX, "");                                   // Clean ending
        String groupId = data.substring(0, data.indexOf(','));            // Get group ID
        data = data.substring(data.indexOf(',')+1, data.length());
        String parameterId = data.substring(0, data.indexOf(','));        // Get parameter ID
        data = data.substring(data.indexOf(',')+1, data.length());
        String value = data;                                              // Get parameter value

    /* 
    app to MCU in all times sends value as a float 
    {gID, pID, value}
    */
        toParameters( groupId.toInt(), parameterId.toInt(), value.toFloat() );
        Serial.print(MESSAGE_PREFIX);
        Serial.print( groupId + parameterId );
        Serial.print(" parameter updated " + value);
        Serial.print(MESSAGE_SUFFIX);
    }
    
}

void readApp(){
    
    if ( (Serial.available() > 0) && !isDataReceived ) {
        String current = Serial.readStringUntil((char)END_MARKER);
        index = current.indexOf((char)START_MARKER);
        stream = current.substring(index+1);
        isDataReceived = true;
    }
    
}

void handleInput(){
  //
    if ( isDataReceived ){
        if ( INPUT_SAVE == stream ){                    /* - Update EEPROM - */
            EEPROM.update(0, 0);                        // activate exploitation of memory
            EEPROM.put(dataRegion+1, Parameters);
            Serial.print( MESSAGE_PREFIX + "EEPROM done" + MESSAGE_SUFFIX );
        }
        else if ( INPUT_RESET == stream ){              /* - Switch to default values - */
            EEPROM.update(0, 255);                      // disable exploitation of memory
            Serial.print( MESSAGE_PREFIX + "Use default values" + MESSAGE_SUFFIX ) ;
        }
        else if ( INPUT_GET == stream ){                /* - Sends data to APP - */
            getData();
        }
        else if ( INPUT_GET_GPS == stream ){            /* - Sends GPS data to APP - */
            getGps();
        }
        else if ( INPUT_GET_ALT == stream ){            /* - Sends Altitude data to APP - */
            getAlt();
        }
        else if ( stream.startsWith(INPUT_UPDATE) ){    /* - Receives parameter values from APP - */
            updateParameter(stream);
        }
        else {                                          /* - Received UNKNOWN command - */
            Serial.print( MESSAGE_PREFIX + "Unknown command" + MESSAGE_SUFFIX );
        }
        isDataReceived = false;
        index = 0;
    }
    
}

void setup(){
  
    if ( EEPROM.read(0) ){
        EEPROM.put(1, BOARD_NAME);
    }

	Serial.begin(38400);          
	bleSerial.begin(38400);     //default 115200,..38400-sugested for 8MHz!!!
	pinMode(LED, OUTPUT);   //defines LED pin 

    while(!Serial);       // wait for specified Serial port to be ready (need for atmega32u4)
  
	Serial.print( MESSAGE_PREFIX + "app sample params" + MESSAGE_SUFFIX );
    Serial.print( MESSAGE_PREFIX + "2021-01-18" + MESSAGE_SUFFIX );
    Serial.print( MESSAGE_PREFIX + "comands: " );
    Serial.print( "get, save, reset" + MESSAGE_SUFFIX );
	Serial.println("BLE/Serial Baud : 38400");
	bleSerial.println("BLE/Serial Baud : 38400");
  
	ble_serial_begin();     //enabless RX on BLE module
	ble_app_config_enable();  //enables ble config from app
	on_short(LED, 3);     //blink LED 3 times
  
    //getData();          //sends out all groups and parameters
  
    delay(500);   
    
}

void loop(){

    readApp();
    handleInput();

}

void on_short(uint8_t PIN, uint8_t times){
      for(int i=0; i<times; i++){
          digitalWrite(PIN, HIGH);
          delay(30);
          digitalWrite(PIN, LOW);
          delay(200);
      }   
}

