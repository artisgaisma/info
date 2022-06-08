/* Mārtiņš Kļaviņš 02/2020
 * 24ch Art-Net kontrolieris
 * -------------------------
 * 
 *                        1  2  3  4  5  6  7  8  | 9  10 11 12 13 14 15 16 | 17 18 19 20
 * pogas nolases secībā - 22,23,24,25,26,27,28,29 | 37,36,35,34,33,32,31,30 | 41,40,39,4
 * -
 * - nav vajadzīgi pull-up rezistori
 * 
 * 
 *                          1  2  3  4  5
 * feideri lolases secībā - A0,A1,A2,A3,A4
 * 
 * W5100 LAN Ethernet Module  => ArduinoMega
 *    gnd                     =>     GND
 *    +5                      =>     5V
 *    ss                      =>     10
 *    sck                     =>     52
 *    mosi                    =>     51
 *    miso                    =>     50
 *    
*/


#include "artnetDEVICEconfig.h"
#include "ArtNetMK.h"


uint8_t myDmxFrame[24];
uint8_t lastFrame[24];
uint8_t bArray1 = 0, bArray2 = 0, bArray3 = 0;

unsigned long sekunde = 1000;
unsigned long currentMillis;
unsigned long lastMillis = 0;




void setup(){

/*
    // for class_C network
    // ------------------- 
    set_IP(192, 168, 1, 151);
    set_destIP(192, 168, 1, 255);
    set_Broadcast(192, 168, 1, 255);
    set_MAC(0x04, 0xE9, 0xE5, 0x56, 0x78, 0x9A);
    // -------------------
*/
    // for class_A network
    // -------------------
    set_MAC(0x04, 0xE9, 0xE5, 0x56, 0x78, 0x9A);
    set_destIP(2, 255, 255, 255);
    ClassA_network();
    // -------------------

    DDRA = 0b00000000;
    DDRC = 0b00000000;
    DDRG = DDRG & 0b11011000;

    PORTA = 0b11111111;
    PORTC = 0b11111111;
    PORTG = PORTG | 0b00100111;
    
    ArtNet_beginETH();
    memset(myDmxFrame, 0, 24);
    memset(lastFrame, 0, 24);

    
}

void loop(){


    currentMillis = millis();
    ArtNet_read();
    memset(myDmxFrame, 0, 24);
    
    for ( uint8_t m = 0; m < 5; m++ ){
        myDmxFrame[m] = analogRead(m) >> 2; // .. /4
    }

    bArray1 = PINA;
    bArray2 = PINC;
    bArray3 = PING;
    
    for ( uint8_t m = 0; m < 8; m++ ){
        if ( !(bArray1 & ( 1 << m )) ){
            myDmxFrame[5+m] = 255;
        }
    }
    for ( uint8_t m = 0; m < 8; m++ ){
        if ( !(bArray2 & ( 1 << m )) ){
            myDmxFrame[13+m] = 255;
        }
    }
    for ( uint8_t m = 0; m < 3; m++ ){
        if ( !(bArray3 & ( 1 << m )) ){
            myDmxFrame[21+m] = 255;
        }
    }
    if ( !(bArray3 & ( 1 << 5 )) ){
        myDmxFrame[23] = 255;
    }

    int m = memcmp(lastFrame, myDmxFrame, 24);
    if ( m ){
        memcpy(lastFrame, myDmxFrame, 24);
        ArtNet_write(0, myDmxFrame);
    } 
    else if ( currentMillis - lastMillis >= sekunde ){
        lastMillis = currentMillis;
        ArtNet_write(0, myDmxFrame);
    }
    
    
}
