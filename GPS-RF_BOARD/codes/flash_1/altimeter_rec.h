#ifndef alimeterREC_H
#define alimeterREC_H

#include <Arduino.h>
#include <SPIMemory.h>
#include <EEPROM.h>
#include <BMP280.h>



// ---- EEPROM memory addresses, change if overlaps with other codes
#define IDaddr            0                         // stored uint8_t
#define packetCountAddr   1                         // stored uint32_t
#define timeAddr          5                         // stored float
// ---- FLASH memory
#define FLASHsize         0x7FFFFF
#define packetSize        10                        // stored uint8_t + float + float
#define maxPackets        ( FLASHsize/packetSize )
#define dataAddr          0x1000                    // data storage starts with 1st FLASH sector ( not implemented yet )
// ---- barometric forecast ( irrelevant, because BOARD measures relative height from certain base level )
#define P0                1020.25                   // pressure in hPa
// ---- height measurements
#define samples           5                         // sample rate for base level measurements
#define samplesFIR        5                         // sample rate for altimeter reading


enum Status { session, startTime };

/* setup functions (mandatory requirement) */
void begin_ALTIMETER();
void begin_FLASH();
/* functions for data management */
uint8_t read_FLASH();
uint8_t erase_FLASH();
void altimeter_REC( bool enable, unsigned long REC_step, float REC_threshold );
void read_ALTIMETER( unsigned long READ_step );
void update_STATUS( uint8_t Status );
void update_STATUS( uint8_t Status, float Time );



#endif
