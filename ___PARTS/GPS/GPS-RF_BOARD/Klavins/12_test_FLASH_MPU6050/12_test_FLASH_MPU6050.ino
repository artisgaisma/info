/* Things which is up to USER:
User has to determine when to start/stop accel-gyro recording with update_STATUS(enable/disable)
User has to update session ID with update_STATUS(session) if needed
User has to decide when to erase memory
*/

#include "accel_gyro.h"


unsigned long frequency = 5000;           // recording freq in ms                   (now: once every 5 seconds)


void setup() {


    Serial.begin(115200);
    begin_ACCEL_GYRO();
    begin_FLASH();
    //erase_FLASH();
    memoryDUMP();
    update_STATUS(enable);
    

}

void loop() {

    get_data();
    ACCEL_GYRO_REC(frequency);

    
}
