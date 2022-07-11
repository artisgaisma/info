constrain()
[Math] - Constrains a number to be within a range.
Syntax - constrain(x, a, b)

x: the number to constrain Allowed data types: all data types.
a: the lower end of the range. Allowed data types: all data types.
b: the upper end of the range. Allowed data types: all data types.

Returns
x: if x is between a and b.
a: if x is less than a.
b: if x is greater than b.

Example Code
The code limits the sensor values to between 10 to 150.

sensVal = constrain(sensVal, 10, 150);  // limits range of sensor values to between 10 and 150
Notes and Warnings
Because of the way the constrain() function is implemented, avoid using other functions inside the brackets, it may lead to incorrect results.

This code will yield incorrect results:
	int constrainedInput = constrain(Serial.parseInt(), minimumValue, maximumValue);   // avoid this
	Use this instead:
	int input = Serial.parseInt();  // keep other operations outside the constrain function
	int constrainedInput = constrain(input, minimumValue, maximumValue);

`digitalWrite(ledPin, !digitalRead(ledPin));`

/*=========================================================================================*/
Serial.write()	-	Writes binary data to the serial port.
			sent as a byte or series of bytes; 
Serial.print()	-	writes characters to serial output.
			sent as a digits
Serial Print
Serial.print(78) gives "78"
Serial.print(1.23456) gives "1.23"
Serial.print('N') gives "N"
Serial.print("Hello world.") gives "Hello world."
Serial.print(78, BIN) gives "1001110"
Serial.print(78, OCT) gives "116"
Serial.print(78, DEC) gives "78"
Serial.print(78, HEX) gives "4E"
Serial.println(1.23456, 0) gives "1"
Serial.println(1.23456, 2) gives "1.23"
Serial.println(1.23456, 4) gives "1.2346"
Syntax
Serial.print(val) 
Serial.print(val, format)

/*
	*	.h failā var definēt un izpildīt funkcijas(){};
	*	.h failā var #define PIN_BUZ 9 ar #ifdef BOARD_A.... #endif
	*	nav jālieto #ifndef HEADER_H #endif
	*	.c failā #define Number 7 pirms #include "xx.h" 
	*	Number 7 parādas .h failā, tas vēlreiz nav jāpiesauc
*/

/*RUN EVERY****************************************************************************/
  if (runEvery(1000)) { // repeat every 1000 millis

    String message = "HeLoRa World! ";
    message += "I'm a Node! ";
    message += millis();

    LoRa_sendMessage(message); // send a message
    Serial.println("Send Message!");
  }  

  
  boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
/*END****************************************************************************/



/*POWER SAVING****************************************************************************/
  //sample of how I'm shutting down the MCU for power-savings
 
  void setup() {
    //power on TH02
    pinMode(si7005Vcc, OUTPUT);
    digitalWrite(si7005Vcc, HIGH);
    //
    //disable heater and enable fast measurement (less time spent sampling)
    Si.disableHeater();
    Si.enableFastMeasurements();
  }

  void loop() {
  
    Serial.begin(SERIAL_BAUD);//Turn serial back on
    
    digitalWrite(si7005Vcc, HIGH);//turn TH02 back on
    //
    .....
    //
    //  shutdown devices and idle for ~60 seconds
    Serial.flush();
    Serial.end();
    radio.sleep();
    digitalWrite(si7005Vcc, LOW);
    //
    for (int count = 0; count < 7; count++) {
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
  }
/*END****************************************************************************/


/*Interupt PIN*******************************************************************/

attachInterrupt(digitalPinToInterrupt(PIN),Function,MODE); //pin 2 or 3
	//MODE - RISING,FALLING,CHANGE,LOW
	//Function - short as possible
void Function()          
{                   
   output = LOW;               
   lcd.setCursor(0,1);                                         
   lcd.print("Interrupt 1");
}
/*END****************************************************************************/


/*button states******************************************************************/
DT_state = digitalRead(RDT_PIN);
    
//change state on each button press
    if(RDT_state != last_RDT_State){
        if(RDT_state == 1){
            if(servoState == 0) servoState = 1;
            else 		servoState = 0;
        }
        last_RDT_State = RDT_state;
     }  

//write values to servo   
    	if(RDT_state == 0){	myservo.write(servo_1_value);
    	}else{			myservo.write(servo_2_value);
	}


//cits gadiijums, ar dazaadiem steitiem
		if(buttonstate == 1){
				stateNum++;
				if(stateNum>3) stateNum=0;
				//te var likt lielaaku ciparu, ja vairaaki steiti. un tad katram sava darbiba
		} //piemeeram taimera laiki
/*END****************************************************************************/


int led;
int timer;

void setup() {
  pinMode (8, OUTPUT);
}
//8Mhz xtal
void loop() {
	timer++;
	digitalWrite(8, led);
	if(timer%10000==0){led=!led;}
}
//16Mhz
void loop() {
	timer++;
	digitalWrite(8, led);
	if(timer%20000==0){led=!led;}
}

/*END****************************************************************************/

//MILLIS
unsigned long time;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  time = millis();
  Serial.println(time); //prints time since program started
  delay(1000);          // wait a second so as not to send massive amounts of data
}
/*END****************************************************************************/


long DELAY = 0;
unsigned long previousMillis = 0; 
void loop() {
  unsigned long currentMillis = millis();  
  if(currentMillis - previousMillis >= 100){
      previousMillis += 100;
      if(DELAY > 800 && DELAY < 1200)
      {
       Serial.println("Button1");
      }
      if(DELAY > 1300 && DELAY < 1700)
      {
       Serial.println("Button2");
      }
      if(DELAY > 1800 && DELAY < 2200)
      {
       Serial.println("Button3");
      }
      if(DELAY > 2300 && DELAY < 2700)
      {
       Serial.println("Button4");
      }
  }
/*END****************************************************************************/

//same functions
n++;
n = n + 1;
n += 1;

/*END****************************************************************************/

/////////////BUTON/////////////
while (status_button == HIGH){
 	button_delay++;
 	delay(100);
 	status_button = digitalRead(inputPin);
 
if(button_delay == 10){ //no need to wait for user to release
         B
         break;       
}else if(button_delay < 10) {//short press
          A
}

/*END****************************************************************************/

  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY == RF69_433MHZ ? 433 : FREQUENCY == RF69_868MHZ ? 868 : 915);
  Serial.println(buff);  

/*END****************************************************************************/

unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON) / 1000;
  printInt(distanceKmToLondon, gps.location.isValid(), 9);

/*END****************************************************************************/

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));


/*END****************************************************************************/

      if (input == 'e')
      {
        Serial.print("Erasing Flash chip ... ");
        flash.chipErase();
        while (flash.busy());
        Serial.println("DONE");
      }

/*END****************************************************************************/
//store values and send data

  int currPeriod = millis() // TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    //fill in the struct with new values
  theData.nodeId = Satellites;
  theData.uptime = Satellites; // working
  theData.temp = hdop ; //hdop works
  theData.AAltitude = altitude;
  theData.LLat = latitude;
  theData.LLon = longitude;
  theData.TTemperature = ftemp;
  CCounter = ++CCounter  ;
  theData.SendCount = CCounter;
      if (radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData)))
      Blink(LED, 3);
      lastPeriod = currPeriod;
  }

/*END****************************************************************************/


// Constants for hardware
const byte SWITCH_PIN[] = {2,3,4};
const byte LED_PIN[] = {5,6,7};

const byte maxDataLength = 20;
char receivedChars[21] ;
boolean newData = false;

/*END****************************************************************************/

const byte ROWS = 10;
const byte COLS = 3;
ms = 1000;
int dataArray[ROWS][COLS] = 
{
	{100, 100},//TOW - 0
	{100, 150},//CIR - 1
	{102, 100},//ACC - 2
	{90, 100, 0.4},//UP - 3
	{85, 100, 0.6},//UP2 - 4
	{55, 50, 0.5},//BNT - 5
	{98, 100, 1},//RUN - 6
	{100, 110, 180},//FLY - 7
	{200, 150}//DT - 8
};
serwowrite.STAB = dataArray[0][0];
serwowrite.WING = dataArray[0][1];
TIMER = dataArray[3][2];

////IR ielikts
for(int t=0; t<TIMER; t++){
	delay(ms);
}

/*END****************************************************************************/

#include <EEPROM.h>

const byte ROWS = 2;
const byte COLS = 8;
const int ADDRESS_OFFSET = 50;

int my2dArray[ROWS][COLS] = 
{
  {0x8888, 0x9999, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF},
  {0x9999, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF, 0x8888}
};

void setup(){
  Serial.begin(9600);

  int address;

  Serial.println ("Writing data.....");

  for(int j=0; j<ROWS; j++){

    Serial.println();

    for(int i=0; i<COLS; i++){

      EEPROMWriteInt(address= ((ROWS*i)+(j * COLS * sizeof (my2dArray [0][0]))+ ADDRESS_OFFSET), my2dArray[j][i]);
      Serial.println(address);
      
    }
  }
  Serial.println();
  Serial.println();
  Serial.println ("Reading data.....");

  for(int j=0; j<ROWS; j++){

    Serial.println();

    for(int i=0; i<COLS; i++){

      unsigned int value= EEPROMReadInt(address=(ROWS*i)+(j*COLS*sizeof (my2dArray [0][0]))+ADDRESS_OFFSET);
      Serial.print(address);
      Serial.print('\t'); 
      Serial.println(value,HEX);

    }
  }
}

void loop(){
}

//integer read/write functions found at http://forum.arduino.cc/index.php/topic,37470.0.html
//This function will write a 2 byte integer to the eeprom at the specified address and address + 1

void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
}

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address); 
  byte highByte = EEPROM.read(p_address + 1);
  return ((lowByte << 0) & 0xFF) + ((highByte << 8)& 0xFF00);

}

////////////////////////////////////////////////
void recvWithStartEndMarkers()
{
     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '[';
     char endMarker = ']';
     char rc;
 
     if (BTserial.available() > 0) 
     {
          rc = BTserial.read();
          if (recvInProgress == true) 
          {
               if (rc != endMarker) 
               {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx > maxDataLength) { ndx = maxDataLength; }
               }
               else 
               {
                     receivedChars[ndx] = '\0'; // terminate the string
                     recvInProgress = false;
                     ndx = 0;
                     newData = true;
               }
          }
          else if (rc == startMarker) { recvInProgress = true; }
     }
}

/*END****************************************************************************/

void processCommand()
{
     Serial.print("receivedChars = ");   Serial.println(receivedChars);
    if (receivedChars[0] == 'L')      // do we have a LED command?
    {
        // we know the LED command has a fixed length "L10"
        // and the value at pos 1 is the LED and the value at pos 2 is 0 or 1 (on/off). 
        // 0 and 1 is the same as LOW and HIGH so we can use 0/1 instead of LOW/HIGH
 
        byte LEDnum = receivedChars[1] - 48;          // convert ascii to value by subtracting 48
        boolean LEDstatus = receivedChars[2] - 48;
 
        digitalWrite(LED_PIN[LEDnum-1],LEDstatus);
        LED_State[LEDnum-1] = LEDstatus;
    }
    if (receivedChars[0] == 'S')      // do we have a slider command?
    {
        // we know the slider command has a fixed length "S123"
        // and the value at pos 1,2, and 3 is the value. 
        // We need to convert the value from 3 ascii characters to a single value

        byte hundreds = (receivedChars[1]-48) * 100;
        byte tens = (receivedChars[2]-48) * 10;
        byte units = receivedChars[3]-48;
        byte value = hundreds + tens + units;
        
        analogWrite(LED_PIN[0],value);
    }
    receivedChars[0] = '\0';
    newData = false;
}

/*END****************************************************************************/

/***************STAB WING TIME********/
int TOW[]     = {81, 60, 0};
int CIRCLE[]  = {81, 130, 0};
int ACCEL[]   = {74, 60, 0};
int START[]   = {67, 70, 0.2};
int START2[]  = {64, 90, 0.6};
int KLIV[]    = {40, 120, 0.7};
int SPEED[]   = {77, 100, 3};
int FLY[]     = {78, 100, 0};
int STOP[]    = {150, 150, 0};
  
  
/*END****************************************************************************/


/*****************************MILLIS************************************************/
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds
const byte ledPin = 13;    //using the built in LED

void setup()
{
  Serial.begin(115200);  //start Serial in case we need to print debugging info
  pinMode(ledPin, OUTPUT);
  startMillis = millis();  //initial start time
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}

/*same*/

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10;  //the value is a number of milliseconds between changes of brightness
const byte ledPin = 10;    //using an LED on a PWM pin.
byte brightness = 0;       //initial brightness
byte increment = 1;        //amount to change the brightness at the end of each period.

void setup()
{
  Serial.begin(115200);  //start Serial in case we need to print debugging info
  pinMode(ledPin, OUTPUT);
  startMillis = millis();  //initial start time
}

void loop()
{
  currentMillis = millis();  //get the current time
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    analogWrite(ledPin, brightness);    //set the brightness of the LED
    brightness += increment;    //will wrap round from 255 to 0 because brightness is an unsigned data type
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED brightness
  }
}

/*same*/

unsigned long blinkStartMillis;
unsigned long fadeStartMillis;
unsigned long currentMillis;
const unsigned long blinkPeriod = 1000;  //blink period
const unsigned long fadePeriod = 10;  //fade period
const byte blinkLedPin = 13;    //this LED will blink
const byte fadeLedPin = 10;    //this LED will fade
byte brightness = 0;  //initial brightness of LED
byte increment = 1;  //amount to change PWM value at each change

void setup()
{
  Serial.begin(115200);  //start Serial in case we need to print debugging info
  pinMode(blinkLedPin, OUTPUT);
  blinkStartMillis = millis();  //start time of blinking LED
  fadeStartMillis = millis();  //start time of fading LED
}

void loop()
{
  currentMillis = millis();  //get the current time
  blink();
  fade();
}

void blink()  //function to blink an LED if the blink period has ended
{
  if (currentMillis - blinkStartMillis >= blinkPeriod)  //test whether the period has elapsed
  {
    digitalWrite(blinkLedPin, !digitalRead(blinkLedPin));  //if so, change the state of the LED
    blinkStartMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}

void fade()    //function to fade an LED
{
  if (currentMillis - fadeStartMillis >= fadePeriod)  //test whether the period has elapsed
  {
    analogWrite(fadeLedPin, brightness);    //set the brightness of the LED
    brightness += increment;    //will wrap round because brightness is an unsigned data type
    fadeStartMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}
/*END****************************************/


/*BUTTOn COUNT*/

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  //period during which button input  is valid
const byte buttonPin1 = A1;    //button on pin A1
byte currentButtonState;
byte previousButtonState;
int count = 0;
boolean counting)

void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin1, INPUT_PULLUP);
  Serial.println("Press the button as many times as possible in 5 seconds");
  counting = true;    //turn on the counting indicator
  startMillis = millis();  //save the start time
}

void loop()
{
  currentMillis = millis();
  previousButtonState = currentButtonState;
  if (currentMillis - startMillis <= period)  //true until the period elapses.  Note that this is the reverse of BWOD
  {
    currentButtonState = digitalRead(buttonPin1);
    if (currentButtonState == LOW and previousButtonState == HIGH)  //the button has become pressed
    {
      count++;    //increment count
      Serial.println(count);
    }
  }
  else  //period has ended
  {
    if (counting == true)  //if we were counting
    {
      Serial.print("Time is up");
      counting = false;    //prevent the message being displayed again
    }
  }
}
/*END****************************************/

/*Aproximate DATA****************************************/
int AproxData()
{
	int buffer;
	int dataReadTimes = 3;
	byte i;
	dataOut = 0;
	
	for (i = 0; i<dataReadTimes; i++)
	{
		buffer = analogRead(PIN_1);//can be anything here to read
		
		dataOut = dataOut + buffer;
		delay(1);
	}
	dataOut=dataOut/dataReadTimes;//add multiplier because of voltage refference
	return dataOut;
}
/*END****************************************/


/*LED Flasher from RC forum****************************************/
 
int A = 2; // A-B Left Pair
int B = 6;
int C = 13; // C-D Right Pair
int D = A3;

#define DEL 40
#define DEL_OFF 50

void setup()
{
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
}

void loop()
{
long l = pulseIn(0, HIGH, 3000); // Use RX as its next to GND,VCC,RX on ProMin END Connector
if ( l < 1500 && l > 0 )
  {
    t41();
    t41();
    delay(250);
    t42();
    t42();
    t42();
    t42();
    delay(250);
    t48();
    t48();
    delay(1000);
  }
}

void t48()
{
    tog();
    tog();
    tog();
    tog();
    tog();
    tog();
    tog();
    tog();
}
void t42()
{
  tog(A, C);
  tog(B, D);
  tog(A, C);
  tog(B, D);
  tog(A, C);
  tog(B, D);
  tog(A, C);
  tog(B, D);
}

void t41()
{
  tog(A);
  tog(B);
  tog(A);
  tog(B);
  tog(A);
  tog(B);
  tog(A);
  tog(B);

  tog(C);
  tog(D);
  tog(C);
  tog(D);
  tog(C);
  tog(D);
  tog(C);
  tog(D);
}

void tog()
{
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  delay(DEL);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  delay(DEL_OFF); 
  delay(DEL_OFF);
  delay(DEL_OFF);
}

void tog(int pin)
{
  digitalWrite(pin, HIGH);
  delay(DEL);
  digitalWrite(pin, LOW);
  delay(DEL_OFF);
}
void tog(int pin1, int pin2)
{
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  delay(DEL);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(DEL_OFF);
}

}
//---------------------------------------------------------------------------------------
//				Structure SAMPLE
//---------------------------------------------------------------------------------------
struct Group {
	int number;
	char g_name[10];
};

Group customVar{0,"Circle"}; //jebshu->
//group name seciibu katram timer var savu, lai nav jaapisaas
for(int i=0; i<=GROUP_COUNT; i++){
	Group customVar{i,group_name[i]}; 
};


struct Parameter {//only for one param. time and servos has different steps and diapazones
	int g_number;
	int p_number;
	char p_name[10];// Duration, Stab, Wing, Time
	float value;
	int min;
	int max;
	float step;
};

//---------------------------------------------------------------------------------------
//				EEPROM SAMPLE
//---------------------------------------------------------------------------------------
#include <EEPROM.h>

struct MyObject {
  float field1;
  byte field2;
  char name[10];
};

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  float f = 123.456f;  //Variable to store in EEPROM.
  int eeAddress = 0;   //Location we want the data to be put.

  //One simple call, with the address first and the object second.
  EEPROM.put(eeAddress, f);
  Serial.println("Written float data type!");
  /** Put is designed for use with custom structures also. **/

  //Data to store.
  MyObject customVar = {
    3.14f,
    65,
    "Working!"
  };
  eeAddress += sizeof(float); //Move address to the next byte after float 'f'.
  //The "sizeof" operator returns the number of bytes in a variable type, or the number of bytes occupied by an array

  EEPROM.put(eeAddress, customVar);
  Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
}

/*--------------------------------------------------------------------------------------------------*/
#include <EEPROM.h>

struct MyObject{
  float field1;
  byte field2;
  char name[10];
};

void setup(){
  float f = 0.00f;   //Variable to store data read from EEPROM.
  int eeAddress = 0; //EEPROM address to start reading from
  Serial.begin( 9600 );
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print( "Read float from EEPROM: " );
  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get( eeAddress, f );
  Serial.println( f, 3 );  //This may print 'ovf, nan' if the data inside the EEPROM is not a valid float.
  // get() can be used with custom structures too.
  eeAddress = sizeof(float); //Move address to the next byte after float 'f'.
  
  MyObject customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get( eeAddress, customVar );
  Serial.println( customVar.field1 );
  Serial.println( customVar.field2 );
  Serial.println( customVar.name );
}

