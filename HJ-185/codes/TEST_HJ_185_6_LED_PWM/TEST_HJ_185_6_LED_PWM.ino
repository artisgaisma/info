//BootLoader - Pro Mini/UNO
//Board Pro mini 3v 8MHz
//Programmer - USB Tiny
/*
P1- LOW - Turn On, HIGH-sleep
P16 - HIGH - Config mode
P4 - High out - Connected as Slave
P14 - High out - Connected to slave as master
VCC - 1.7-3.6V

LED PWM with analog write
*/
#define ver 0.4
#define BUTON         4
#define BUTON_2       20
#define LED           10
#define SRTOBE        5
#define VOLTAGE       A2    
#define POT           A3
#define STAB          3
#define BLE_TX        9
#define BLE_RX        8
#define BLE_WAKE_UP   7
#define BLE_CONFIG    6


int ledPin = LED;    // LED connected to digital pin 10
void setup(){
        Serial.begin(9600);
        Serial.println("HJ-185 Module" );
        Serial.print(ver);
        Serial.print("\t");
        Serial.println();
}

void loop(){
    //DIM UP
    led_fade_up(LED, 2, 255);
    //DIM UP
    led_fade_down(LED, 255, 2);
    
}



void led_fade_up(uint8_t PIN,uint8_t START, uint8_t END){
      for (int fadeValue = START ; fadeValue <= END; fadeValue += 5) {
          analogWrite(PIN, fadeValue);
          show_voltage(VOLTAGE);
          delay(30);
      }
}
void led_fade_down(uint8_t PIN,uint8_t START, uint8_t END){
      for (int fadeValue = START ; fadeValue >= END; fadeValue -= 5) {
          analogWrite(PIN, fadeValue);
          show_voltage(VOLTAGE);
          delay(30);
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
    Serial.print(" v ");
    Serial.println("");
}
