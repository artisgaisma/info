// zoomkat 11-22-10 serial servo (2) test
// for writeMicroseconds, use a value like 1500
// for IDE 0019 and later
// Powering a servo from the arduino usually DOES NOT WORK.
// two servo setup with two servo commands
// send eight character string like 15001500 or 14501550

#include <Servo.h> 
String readString, servo1, servo2;
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;

void setup() {
  Serial.begin(9600);
  myservo1.attach(6);  //the pin for the servo control 
  myservo2.attach(7);
  Serial.println("servo-test-21"); // so I can keep track of what is loaded
}

void loop() {

  while (Serial.available()) {
    delay(10);  
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }

  if (readString.length() >0) {
      Serial.println(readString); //see what was received
      
      // expect a string like 07002100 containing the two servo positions      
      servo1 = readString.substring(0, 4); //get the first four characters
      servo2 = readString.substring(4, 8); //get the next four characters 
      
      Serial.println(servo1);  //print ot serial monitor to see results
      Serial.println(servo2);
      
      int n1; //declare as number  
      int n2;
      
      char carray1[6]; //magic needed to convert string to a number 
      servo1.toCharArray(carray1, sizeof(carray1));
      n1 = atoi(carray1); 
      
      char carray2[6];
      servo2.toCharArray(carray2, sizeof(carray2));
      n2 = atoi(carray2); 
      
      myservo1.writeMicroseconds(n1); //set servo position 
      myservo2.writeMicroseconds(n2);
    readString="";
  } 
}