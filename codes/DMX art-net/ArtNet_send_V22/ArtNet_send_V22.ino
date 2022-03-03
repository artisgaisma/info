/* Mārtiņš Kļaviņš 03/2020
 * 25ch Art-Net/DMX512 kontrolieris
 * --------------------------------
 * 
 * DMX512 channel         6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25
 * port                   PA6,PA7,PC7,PC6,x   PD3,PD2,PD1,PD0,PA1 PJ1,PJ0,PH1,PH0,PA2 PE4,PE5,PG5,PE3,x
 * digitalPIN (pogas)     28  29  30  31  x  |18  19  20  21  23 |14  15  16  17  24 |2   3   4   5,  x
 * 
 * DMX512 channel         1  2  3  4  5
 * analogPIN (feideri)    A0,A1,A2,A3,A4
 * 
 * W5500 LAN Ethernet Module  => ArduinoMega
 *    gnd                     =>     GND
 *    +5                      =>     5V
 *    ss                      =>     10
 *    sck                     =>     52
 *    mosi                    =>     51
 *    miso                    =>     50
 *    
 * =====================================================================================================
 * Change Log:
 * 29.03.2020 - W5100,W5200,W5500 ethernet čipu atbalsts
 * 30.03.2020 - GitHub iesniegts bug report par <DMXSerial.h> DMX512 frame format.
 *              Jābūt SERIAL_8N2 (ar diviem stop bitiem)
 *              Vai nu manuāli jālabo biblitēka vai jaizmanto v1.5.1 (30.03.2020)
 *              https://github.com/mathertel/DMXSerial/issues/39
 * 10.05.2020 - ArtPoll discovery
 * =====================================================================================================
 * Used libraries, datasheets etc:       
 *    https://github.com/Serasidis/Ethernet_STM
 *    https://github.com/mathertel/DMXSerial
 * =====================================================================================================
 * To-Do:
 *    # testēt ArtNet un DMX512 paralēlu darbību
 *    # pievienot divus nezināmos pogas pinus
 *    # ArtNet biblioteka - nolasīt MAC no W5500, izveidot W5100 un W5500 saderību
 *    # reply on own ArtPoll
 *    # upgrade ArtNet library ........
 *    
*/

  

#include <artnetDEVICEconfig.h>
#include <ArtNetMK.h>
#include <DMXSerial.h>



// global variables
// ----------------

  // for ArtNet
  uint16_t packet;
  uint8_t discoverFlag = 0;

  // for DMX512
  uint8_t myDmxFrame[26];
  uint8_t lastFrame[26];
  uint8_t undefined = 13;
  uint8_t pinMap[20] = { 28, 29, 30, 31, undefined, 18, 19, 20, 21, 23, 14, 15, 16, 17, 24, 2, 3, 4, 5, undefined};


  // for time
  unsigned long sekunde = 1000;
  unsigned long currentMillis;
  unsigned long lastMillis1 = 0;             // sekundes taimeris
  unsigned long discover_time = 2500;
  unsigned long lastMillis2_5 = 0;           // 2.5sec taimeris



void setup(){


    DMXSerial.init(DMXController);
    DMXSerial.maxChannel(25);

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

    for ( uint8_t m = 0; m < 20; m++ ){
        pinMode(pinMap[m], INPUT_PULLUP);
    }
    
    ArtNet_beginETH();

    memset(myDmxFrame, 0, 26);
    memset(lastFrame, 0, 26);

    ArtNet_discover();                      // send ArtPoll on power up


}

void loop(){


    currentMillis = millis();
    packet = ArtNet_read();
    if ( packet == ART_POLL ){
        discoverFlag++;
    }
    memset(myDmxFrame, 0, 26);
 
    for ( uint8_t m = 0; m < 5; m++ ){
        myDmxFrame[m] = analogRead(m) >> 2;
    }
    for ( uint8_t m = 0; m < 20; m++ ){
        if ( !(digitalRead(pinMap[m])) ){
            myDmxFrame[5+m] = 255;
        }
    }


    int m = memcmp(lastFrame, myDmxFrame, 26);
    if ( m ){
        memcpy(lastFrame, myDmxFrame, 26);
        ArtNet_write(0, myDmxFrame);
        for ( uint8_t m = 0; m < 25; m++ ){
            DMXSerial.write(m+1, myDmxFrame[m]);
        }
    } 
    else if ( currentMillis - lastMillis1 >= sekunde ){
        lastMillis1 = currentMillis;
        ArtNet_write(0, myDmxFrame);
    }

    if ( currentMillis - lastMillis2_5 >= discover_time ){
        lastMillis2_5 = currentMillis;
        if ( discoverFlag == 0 ){
            ArtNet_discover();
        }
        discoverFlag = 0;
    }

    
}
