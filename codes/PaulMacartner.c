//arduino interupts
//Leson 28
//blink led

//Library - TimerOne r11.zip - make folder on libraries and add files there


#include <TimerOne.h>
String LEDStatus="OFF";
int YellowLED=10;
int RedLED=9;

void setup(){
	pinMode(YellowLED, OUTPUT);
	pinMode(RedLED, OUTPUT);
	Timer1.initialize(1000000);//microseconds not miliseconds
	Timer1.attachInterrupt(BlinkYellow);
}
void loop() {
	digitalWrite(RedLED, HIGH);
	delay(250);
	digitalWrite(RedLED, LOW);
	delay(250);
}

void BlinkYellow(){
	if(LEDStatus=="ON"){
		digitalWrite(YellowLED,LOW);
		LEDStatus="OFF";
		return;	
	}
	if(LEDStatus=="OFF"){
		digitalWrite(YellowLED,HIGH);
		LEDStatus="ON";
	}
}

//Leson 29
// dont use delay in interupts voids
//dont print in interupts

//Leson 30
//better interrupt library - https://github.com/PaulStoffregen/TimerOne
#include<TimerOne.h>
#include<SoftwareSerial.h>
int flag=0;
int RedLed=9;
long tmr;
String NMEA="";
char c;
String UPDATE_10_sec="$PMTK220,10000*2F r n";
sTRING GPRMC_ONLY= "GARA PENTERE";
SoftwareSerial GPSSerial(2,3);

void setup(){
	
	pinMode(RedLed,OUTPUT);
	GPSSerial.begin(9600);
	Serial.begin(115200);
	delay(100);
	GPSSerial.print(GPRMC_ONLY);
	delay(100);
	GPSSerial.print(UPDATE_10_sec);
	Timer1.initialize(1000);
	Timer1.attachInterrupt(readGPS);
	
}

void loop(){
	digitalWrite(RedLED,HIGH);
	delay(1000);
	digitalWrite(RedLED,LOW;)
	delay(1000);
	if (flag==1){
		Timer1.stop();
		NMEA.trim();
		Serial.println(NMEA);
		NMEA="";
		Timer1.restart();
		
	}
	
}
readGPS(){
	if(GPSSerial.available()>0){
		c=GPSSerial.read();
		NMEA.concat(c);
	}
	if(c=='r''){}//vajag preteejo / ziimi
	flag=1;
	
}




//LESON 31
float grades[25];
int numGrades;
float av;
int i;
float sumGrades=0;

void setup(){
	Serial.begin(9600);
	
	
}

void loop(){
	Serial.println("How many grades?");
	while(Serial.available()==0){
	}
	numGrades=Serial.parseInt();
	
	for(i=0; i<numGrades; i++){
		Serial.println("input grade");
		while (Serial.available()==0){
		}
	grades[i]=Serial.parseFloat();		
	}
	
	
	for(i=0; i<numGrades, i++){
		sumGrades=sumGrades+grades[i];
	}
	av=sumGrades/numGrades;
	Serial.print("average grades is");
	Serial.println(av);
	sumGrades=0;
}





//Lesson 32
int numGrades;
float grades[15];
int j;
float av;
float bucket=0;


void setup(){
Serial.begin(9600);
	
}

void loop(){
	/*
	Serial.println("how many grades");
	while(Serial.available()==0){
	}	
	numGrades=Serial.parseInt();
	
	for(j=1; j<numGrades; j++){
		Serial.println("input grade");
		while(Serial.available()==0){
		}
		grades[j]=Serial.parseFloat();
	}
	
	**************************
	for(j=1; j<numGrades; j++){
		bucket=bucket+grades[j];
	}
	av=bucket/numGrades;
	bucket=0;
	**************************
	Serial.println(av);
	Serial.println("your grades was");
	for(j=1; j<numGrades; j++){
		Serial.println(grades[j]);
	}
	*/
	
	inputGrades();
	avGrades();
	printGrades();
	
}

void inputGrades(){
	Serial.println("how many grades");
	while(Serial.available()==0){
	}	
	numGrades=Serial.parseInt();
	
	for(j=1; j<numGrades; j++){
		Serial.println("input grade");
		while(Serial.available()==0){
		}
		grades[j]=Serial.parseFloat();
	}
}

void avGrades(){
	for(j=1; j<numGrades; j++){
		bucket=bucket+grades[j];
	}
	av=bucket/numGrades;
	bucket=0;
	
}
void printGrades(){
Serial.println(av);
	Serial.println("your grades was");
	for(j=1; j<numGrades; j++){
		Serial.println(grades[j]);
	}	
	
}

//Lesson 33 - Local and Global Variables

int x=7;
void setup(){
	Serial.begin(9600);
	delay(100);
	Serial.print("you are in void setup");
	Serial.print(x);
	delay(4000);
	
}

void loop(){
	Serial.print("you are in void loop");
	Serial.print(x);
	hello();
	delay(2000);
	
}
void hello(){
	Serial.println("hello func has x=");
	Serial.print(x);
}


//Leson 34
//functions
//****************Part 1*************************copied from Leson 32
int numGrades;
float grades[15];
int j;
float bucket=0;

void setup(){
Serial.begin(9600);
	
}

void loop(){
	float av;
	inputGrades();
	av=avGrades();
	Serial.print("our average is");
	Serial.println(av);
	//printGrades();
}
void inputGrades(){
	Serial.println("how many grades");
	while(Serial.available()==0){
	}	
	numGrades=Serial.parseInt();
	
	for(j=1; j<numGrades; j++){
		Serial.println("input grade");
		while(Serial.available()==0){
		}
		grades[j]=Serial.parseFloat();
	}
}
void avGrades(){
	float average;
	for(j=1; j<numGrades; j++){
		bucket=bucket+grades[j];
	}
	average=bucket/numGrades;
	return average;
	bucket=0;
}
void printGrades(){
Serial.println(av);
	Serial.println("your grades was");
	for(j=1; j<numGrades; j++){
		Serial.println(grades[j]);
	}	
	
}

//****************Part 2*************************
void setup(){
	Serial.begin(9600);
	
}
void loop(){
	float x;
	float x2;
	Serial.println("input x");
	while(Serial.available==0){
		//do nothing
	}
	x=Serial.parseFloat();
	x2=x_squred(x);//aizsuuta x uz funkciju
	Serial.println(x);
	Serial.print(x2);
	
}
float x_squred(float x){ //sanjem x un darbina funkciju
	float answer;
	answer=x*x;
	return answer;
}


//Leson 35



void setup(){
	Serial.begin(9600);
}

void loop(){
	float a;
	float b;
	float c;
	float d;
	
	Serial.println("enter first number");
	while(Serial.available==0){}
	a=Serial.parseFloat();
	Serial.println("enter second number");
	while(Serial.available==0){}
	b=Serial.parseFloat();
	
	tally(a,b,c,d);//aizsper uz fuknciju veertiibas-atpakalj atnaaks c un d, jo lejaa tiem ir & ziime
	
	Serial.print("Some of your number is");
	Seril.print(c);
	Serial.print("difference is");
	Seril.print(d);
	
}

//void tally(float firsrNum, float secondNum, float sum, float difference){//te laikam vinus deklaree, njem veertiibas un atsuutiitaa
//pieliekot & suuta arii atpakalj
void tally(float firsrNum, float secondNum, float &sum, float &difference){
	sum=firsrNum-secondNum;
	//c=b+a;
	//d=a-b;
	Serial.println(sum);
	Serial.print(difference);
	return
	
}
