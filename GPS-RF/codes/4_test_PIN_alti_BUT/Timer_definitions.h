/*
2020-10-02
	Added:
		serial_print_info();
		led_blink_loop();
		buz_short();
		buz_long();
		buz_tone();
		timer_time(); with millis();
*/
#include "Pinmap.h"

unsigned long currentMillis;

#ifdef FFT_FIRST_REV_C
unsigned long sekunde       = 1000;

int TIMER_TIME;
bool RDT = false;

Servo stab_servo;
Servo wing_servo;

#endif

void serial_print_info(){
	Serial.println(BOARD);
	Serial.println(SOFT_VER);
	Serial.println("...");
  delay(100);
  Serial.println("..");
  delay(100);
  Serial.println(".");
  delay(100);
}

void i2c_scaner(){//TODO
	
}

void led_blink_loop(uint8_t PIN, int on_time, int off_time){
	
	digitalWrite(PIN, HIGH);
	Serial.println("HIGH");
	delay(on_time);
	
	digitalWrite(PIN, LOW);
	Serial.println("LOW");
	delay(off_time);
	
}

void buz_short(uint8_t PIN, uint8_t times){
	for(int i=0; i<times; i++){
		digitalWrite(PIN, HIGH);
		delay(30);
		digitalWrite(PIN, LOW);
		delay(200);
	}   
}

void buz_long(uint8_t PIN, uint8_t times){
	for(int i=0; i<times; i++){
		digitalWrite(PIN, HIGH);
		delay(1000);
		digitalWrite(PIN, LOW);
		delay(500);
	}   
}

void buz_tone(uint8_t PIN, int start_freq, int end_freq){
	for (int i = start_freq; i <= end_freq; i++){
		tone(PIN, i);
		delay(1);
	}
}

void servo_on(){
  digitalWrite(SERVO_ON, HIGH);
}

void servo_off(){
    digitalWrite(SERVO_ON, LOW);
}

void timer_time( float time ){
    currentMillis = millis();
    while ( millis() < (currentMillis + time) ){
        // your time window for various status checking
    }
}
