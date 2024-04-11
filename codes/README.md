# 328PB bootloader:
```
Prepare your ATmega328PB chip: Make sure your ATmega328PB chip is properly connected to power (VCC and GND) and has a clock source (internal or external crystal). Also, ensure that the reset pin is connected properly.

Install USBtiny support in Arduino IDE:

Open the Arduino IDE.
Go to File > Preferences.
In the "Additional Board Manager URLs" field, add the URL: https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
Click "OK" to close the Preferences window.
Go to Tools > Board > Boards Manager...
Search for "attiny" and install "ATTinyCore" by David A. Mellis.
Select the proper board: Go to Tools > Board and select "ATmega328PB".

Select the programmer: Go to Tools > Programmer and select "USBtinyISP".

Write your code: Write your code in the Arduino IDE as you normally would.

Upload the code:

Connect your USBtiny programmer to your computer via USB.
Connect the USBtiny programmer to the ICSP pins (MISO, MOSI, SCK, RST, VCC, GND) of your ATmega328PB chip.
Make sure your ATmega328PB chip is powered.
Click the Upload button in the Arduino IDE (the right arrow icon).
The IDE should compile your code and attempt to upload it to your ATmega328PB chip using the USBtiny programmer.
Verify: After the upload process is complete, you should see a "Done uploading" message in the Arduino IDE. Verify that your code is running correctly on the ATmega328PB chip.
```
# for(;;); // Don't proceed, loop forever

# Header Files
- #include `<file>` - system Header Files
- #include `"file"` - in same directory your owm header files 
 
```
#ifndef HEADER_FILE
#define HEADER_FILE
    the entire header file file
#endif
```
```
#if SYSTEM_1
   # include "system_1.h"
#elif SYSTEM_2
   # include "system_2.h"
#elif SYSTEM_3
   ...
#endif
```

# define
``` 
 #ifdef TEST
   do_something_here
 #endif
 ```
 ```
  #ifdef DEBUG
   do_something_here
 #else
   do_something_else
 #endif
 ```
 ```
  #ifdef DEFINED
   do_something_here
 #elif TEST2
   do_something_else
 #endif
```
```
#if defined COCONUT || defined APPLE
#endif
```
 
# optimized group, param prints
```
char *groups[] = {"Fly", "TOW", "CIR", "Accelerate", "Release", "Bunt" };
enum groups    = {FLY, TOW, CIR...};
void setup() {}
void loop() {
  for (int i = 0; i < 6; i++) {Serial.println(groups[i]);}
}
```
# Arduno Libraries
```
http://drazzy.com/package_drazzy.com_index.json
http://www.technoblogy.com/package_technoblogy_index.json
http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json
https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json
https://raw.githubusercontent.com/digistump/arduino-boards-index/master/package_digistump_index.json
https://raw.githubusercontent.com/sparkfun/Arduino_Boards/nrf5/IDE_Board_Manager/package_sparkfun_index.json
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json



```
# Precise Servo
```
#include <Servo.h>
Servo servo;

void setup() {
  // initialize serial:
  Serial.begin(9600); //set serial monitor baud rate to match
  servo.write(90);
  servo.attach(9);
  prntIt();
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int pos = Serial.parseInt();
    pos = constrain(pos, 0, 2400);
    servo.write(pos);
    prntIt();
  }
}
void prntIt()
{
  Serial.print("  degrees = ");
  Serial.print(servo.read());
  Serial.print("\t");
  Serial.print("microseconds =  ");
  Serial.println(servo.readMicroseconds());
}
```


# constrain
`Constrains a number to be within a range`
```
constrain(x, a, b)
Parameters
x: the number to constrain Allowed data types: all data types.
a: the lower end of the range. Allowed data types: all data types.
b: the upper end of the range. Allowed data types: all data types.

Returns
x: if x is between a and b.
a: if x is less than a.
b: if x is greater than b.
```
```
This code will yield incorrect results:

int constrainedInput = constrain(Serial.parseInt(), minimumValue, maximumValue);   // avoid this
Use this instead:

int input = Serial.parseInt();  // keep other operations outside the constrain function
int constrainedInput = constrain(input, minimumValue, maximumValue);
```
