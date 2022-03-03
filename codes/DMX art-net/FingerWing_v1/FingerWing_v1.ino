/* Mārtiņš Kļaviņš
 * 25ch Art-Net/DMX512 kontrolieris   / PIXELED.FingerWing /
 * ---------------------------------------------------------
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
 * 23.07.2020 - Setup Mode with FFT app, network class detection and auto-configuration
 *              Changed DMX port in favor for USB communication:
 *              * can't use Serial object if DMXSerial.h is included
 *              * it would be hard to implement bidirectional DMXSerial mode for Serial mimic
 * 27.07.2020 - finished load_settings(), ArtNetMK upgrade
 *              param_addr[] and config_mode() bug fix, corrupted digital pin fix
 * =====================================================================================================
 * Used libraries, datasheets etc:
 *    https://github.com/Serasidis/Ethernet_STM
 *    https://github.com/mathertel/DMXSerial
 * =====================================================================================================
 * To-Do:
 *    # testēt ArtNet un DMX512 paralēlu darbību
 *    # pievienot divus nezināmos pogas pinus
 *    # reply on own ArtPoll - need to redesign library
 *    # finish config_mode() - select two buttons
*/


#include "DMXSerial.h"
#include "setup_mode.h"
#include <FingerWing_config.h>
#include <ArtNetMK.h>


#define DEBUG_BOARD                           // uncomment for use of Serial monitor and test controller
#define DEBUG_OUTPUT                          // uncomment to disable noisy floating pins

  // for ArtNet
  uint16_t packet;
  uint8_t discoverFlag = 0;
  uint8_t mem_field[10];

  // for DMX512
  uint8_t myDmxFrame[26];
  uint8_t lastFrame[26];
  uint8_t undefined = 13;
  uint8_t corrupted = 6;                      // was 18, which is TX1 pin
  uint8_t pinMap[20] = { 28, 29, 30, 31, undefined, corrupted, 19, 20, 21, 23, 14, 15, 16, 17, 24, 2, 3, 4, 5, undefined};

  // for time
  unsigned long sekunde = 1000;
  unsigned long currentMillis;
  unsigned long lastMillis1 = 0;              // sekundes taimeris
  unsigned long discover_time = 2500;
  unsigned long lastMillis2_5 = 0;            // 2.5sec taimeris


bool config_mode(){

    bool READY = connectApp();
    uint8_t i = 0;
    while ( i < 255 ){
        if ( 1 /*divas pogas nospiestas*/ && READY ){
            return true;  
        }
        i++;
    }
    
    return false;
}

void load_settings(){

/* ---26.07.2020--- EEPROM first memory cell will distinguish if there is something written from app
                  * in new AVR MCU eeprom default is 255
                  * saveData() will put 0 in first memory cell
*/
    uint8_t array_length = sizeof(mem_field)/sizeof(mem_field[0]);
    if ( !(EEPROM.read(0)) ){
        for( uint8_t n = 0; n < array_length; n++ ){
            mem_field[n] = EEPROM.read(n+1);
        }
    }
    else {
        //                         | ClassA | IP address     | destination IP  | uni
        uint8_t default_values[] = { 0,      192, 168, 1, 150, 192, 168, 1, 255, 0 };
        for( uint8_t n = 0; n < array_length; n++ ){
            mem_field[n] = default_values[n];
        }
    }

}





void setup(){


    if ( config_mode() ){
        #if defined(DEBUG_BOARD)
            readApp();
        #else
            while(1){
                readApp();
            }
        #endif
    }

    DMXSerial.init(DMXController);
    DMXSerial.maxChannel(25);

    set_MAC(0x04, 0xE9, 0xE5, 0x56, 0x78, 0x9A);
    if ( mem_field[0] == 1 ){
        set_destIP(2, 255, 255, 255);
        ClassA_network();
    }
    else {
        load_settings();
        set_IP( mem_field[1], mem_field[2], mem_field[3], mem_field[4] );
        set_destIP( mem_field[5], mem_field[6], mem_field[7], mem_field[8] );
        
        uint8_t netID = mem_field[1];
        if ( (128 <= netID) && (netID <= 191) ) set_Broadcast( mem_field[1], mem_field[2], 255, 255 );
        if ( (192 <= netID) && (netID <= 233) ) set_Broadcast( mem_field[1], mem_field[2], mem_field[3], 255 );
    }

    for ( uint8_t m = 0; m < 20; m++ ){
        pinMode(pinMap[m], INPUT_PULLUP);
    }

    ArtNet_beginETH();
    memset(myDmxFrame, 0, 26);
    memset(lastFrame, 0, 26);
    Port_Address(mem_field[9]);             // Universe
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
        #if defined(DEBUG_OUTPUT)
            break;
        #else
            myDmxFrame[m] = analogRead(m) >> 2;
        #endif
    }
    for ( uint8_t m = 0; m < 20; m++ ){
        if ( !(digitalRead(pinMap[m])) ){
            myDmxFrame[5+m] = 255;
        }
    }


    int m = memcmp(lastFrame, myDmxFrame, 26);
    if ( m ){
        memcpy(lastFrame, myDmxFrame, 26);
        ArtNet_write(myDmxFrame);
        for ( uint8_t m = 0; m < 25; m++ ){
            DMXSerial.write(m+1, myDmxFrame[m]);
        }
    } 
    else if ( currentMillis - lastMillis1 >= sekunde ){
        lastMillis1 = currentMillis;
        ArtNet_write(myDmxFrame);
    }

    if ( currentMillis - lastMillis2_5 >= discover_time ){
        lastMillis2_5 = currentMillis;
        if ( discoverFlag == 0 ){
            ArtNet_discover();
        }
        discoverFlag = 0;
    }

    
}
