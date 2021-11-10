//MULTI-LINE LED CONTROLLER using PWM
//T.K.Hareendran
//www.electroschematics.com
// Analog inputs connected to the variable resistors
const int knobPin1 = 1; //Red LED control 
const int knobPin2 = 2; //Green LED control 
const int knobPin3 = 3; //Blue LED control 
// PWM outputs connected to LED driver circuits
const int drivePin1 = 9;//Red LED drive
const int drivePin2 = 10;//Green LED drive
const int drivePin3 = 11;//Blue LED drive
// initial value for the variable resistors
int knobValue1 = 0;
int knobValue2 = 0;
int knobValue3 = 0;
void setup() {
   // set the drive pins as output:
   pinMode(drivePin1, OUTPUT);
   pinMode(drivePin2, OUTPUT);
   pinMode(drivePin3, OUTPUT);
}
void loop() {
   // read the variable resistors, convert it to 0 - 255
   knobValue1 = analogRead(knobPin1) / 4;
   knobValue2 = analogRead(knobPin2) / 4;
   knobValue3 = analogRead(knobPin3) / 4;
   // use the data to control the drive:
   analogWrite(9, knobValue1);
   analogWrite(10, knobValue2);
   analogWrite(11, knobValue3);
}