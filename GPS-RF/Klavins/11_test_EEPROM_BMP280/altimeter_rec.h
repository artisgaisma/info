#ifndef alimeterREC_H
#define alimeterREC_H

#include <EEPROM.h>
#include <BMP280.h>



// ---- EEPROM memory addresses, change if overlaps with other codes
#define startTimeAddr     0xBF                      // stored float
#define packetRateAddr    0xC3                      // stored float
#define packetCountAddr   0xC7                      // stored uint8_t
#define dataAddr          0xC8                      // altimeter data storage starts with 200th byte (data header from 191th byte)
#define EEPROMsize        0x3FF                     // 1023th address, size -> 1024 bytes = 0x400
#define packetSize        4                         // stored float
#define maxPackets        ( (EEPROMsize - dataAddr)/packetSize )
// ---- barometric forecast ( irrelevant, because BOARD measures relative height from certain base level )
#define P0                1020.25                   // pressure in hPa
// ---- height measurements
#define samples           5                         // sample rate for base level measurements
#define samplesFIR        5                         // sample rate for altimeter reading


enum Status {     /* ---- status messages for update_STATUS() function --- */
    disable,      //      disable altimeter recording
    enable,       //      enable altimeter recording
};


/* setup functions (mandatory requirement) */
void begin_ALTIMETER();
/* functions for data management */
void read_MEMORY( String id );
void altimeter_REC( unsigned long REC_step, float REC_threshold, unsigned long REC_duration );
void read_ALTIMETER( unsigned long READ_step );
void update_STATUS( uint8_t Status );



#endif
