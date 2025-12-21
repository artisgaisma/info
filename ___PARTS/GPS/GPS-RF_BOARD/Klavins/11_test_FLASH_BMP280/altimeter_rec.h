#ifndef alimeterREC_H
#define alimeterREC_H

//#include <Arduino.h>
#include <SPIMemory.h>
#include <EEPROM.h>
#include <BMP280.h>



// ---- EEPROM memory addresses, change if overlaps with other codes
#define IDaddr            0                         // stored uint8_t
#define packetCountAddr   1                         // stored uint32_t
// ---- FLASH memory
#define FLASHsize         0x7FFFFF //tas nav izmers, tā ir pēdejā adrese ..... izmērs 0x800000
#define packetSize        10                        // stored uint8_t + float + float
#define maxPackets        ( FLASHsize/packetSize )
#define dataAddr          0x1000                    // data storage starts with 1st FLASH sector ( not implemented yet )
// ---- barometric forecast ( irrelevant, because BOARD measures relative height from certain base level )
#define P0                1020.25                   // pressure in hPa
// ---- height measurements
#define samples           5                         // sample rate for base level measurements
#define samplesFIR        5                         // sample rate for altimeter reading


enum Status {     /* ---- status messages for update_STATUS() function --- */
    disable,      //      disable altimeter recording
    enable,       //      enable altimeter recording
    session,      //      start next session
};


/* setup functions (mandatory requirement) */
void begin_ALTIMETER();
void begin_FLASH();
/* functions for data management */
void memoryDUMP();
void read_FLASH( uint8_t sID, float start_time );
void erase_FLASH();
void altimeter_REC( unsigned long REC_step, float REC_threshold );
void read_ALTIMETER( unsigned long READ_step );
void update_STATUS( uint8_t Status );



#endif
