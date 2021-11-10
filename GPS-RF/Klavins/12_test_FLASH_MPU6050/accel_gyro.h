#ifndef AccelGyro_H
#define AccelGyro_H

#include <SPIMemory.h>
#include <EEPROM.h>
#include <MPU6050.h>



// ---- EEPROM memory addresses, change if overlaps with other codes
#define IDaddr            0                         // stored uint8_t
#define packetCountAddr   1                         // stored uint32_t
// ---- FLASH memory
#define FLASHsize         0x7FFFFF //tas nav izmers, tā ir pēdejā adrese ..... izmērs 0x800000
#define packetSize        30                        // stored uint8_t + 7x floats
#define maxPackets        ( FLASHsize/packetSize )


enum Status {     /* ---- status messages for update_STATUS() function --- */
    disable,      //      disable Accel-Gyro recording
    enable,       //      enable Accel-Gyro recording
    session,      //      start next session
};


/* setup functions (mandatory requirement) */
void begin_ACCEL_GYRO();
void begin_FLASH();
/* functions for data management */
void memoryDUMP();
void read_FLASH( uint8_t sID, float start_time );
void erase_FLASH();
void ACCEL_GYRO_REC( unsigned long REC_step );
void update_STATUS( uint8_t Status );
uint8_t get_data();



#endif
