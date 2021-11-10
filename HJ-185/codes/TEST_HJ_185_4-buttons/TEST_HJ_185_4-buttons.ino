//BootLoader - Pro Mini/UNO
//Board Pro mini 3v 8MHz
//Programmer - USB Tiny
/*
P1- LOW - Turn On, HIGH-sleep
P16 - HIGH - Config mode
P4 - High out - Connected as Slave
P14 - High out - Connected to slave as master
VCC - 1.7-3.6V
*/
#define ver 0.1

#define BUTON         4
#define BUTON_2       20//reset
#define LED           10
#define SRTOBE        5
#define VOLTAGE       A2    
#define POT           A3
#define STAB          3
#define BLE_TX        9
#define BLE_RX        8
#define BLE_WAKE_UP   7
#define BLE_CONFIG    6

#include <Servo.h>
Servo myservo;
int servo_pos = 0;
int pot_pos = 0;


static int BUTON_state = 0; //current state of button
static int BUTON_2_state = 0; //current state of button

void setup(){
        Serial.begin(9600);
        Serial.println("HJ-185 Module" );
        Serial.print(ver);
        Serial.print("\t");
        Serial.println();
        
        pinMode(LED, OUTPUT);
        on_short(LED, 2);
        
        myservo.attach(STAB);
        pinMode(BUTON, INPUT_PULLUP);
        pinMode(BUTON_2, INPUT_PULLUP);
        
            }
void loop(){
            //show_voltage(VOLTAGE);
            read_button_state();
            while(BUTON_state == LOW ){
                myservo.write(100);
                on_short(LED, 5);
                read_button_state();
            }
            servo_position();
            
}


void read_button_state(){
    BUTON_state = digitalRead(BUTON);
    BUTON_2_state = digitalRead(BUTON_2);
}
  
void servo_position(){
    pot_pos=analogRead(POT);
    Serial.print(pot_pos);
    Serial.print(" ");
    servo_pos = map(pot_pos, 0, 1023, 0, 255);
    myservo.write(servo_pos);
    Serial.println(servo_pos);
}
  
void on_short(uint8_t PIN, uint8_t times){
  for(int i=0; i<times; i++){
    digitalWrite(PIN, HIGH);
    delay(30);
    digitalWrite(PIN, LOW);
    delay(200);
  }   
}

void show_voltage( uint8_t PIN ){
  float R1=300000.0;  //300K
  float R2=10000.0; //10k
  float reference_v = 3.3;//3.3V or 5V
  float vin = 0.0;
  float vout = 0.0;
  int value = 0;
  
  value = analogRead(PIN);
  vout = (value*reference_v)/1024;
  vin = vout/(R2/(R1+R2));
  if(vin<0.09){vin=0.0;}
  Serial.print(vin);
}
