/* Things which is up to USER:
User has to determine when to start/stop altimeter recording with update_STATUS(enable/disable)
 # when recording starts, previous data are overwritten due to EEPROM size limits
 # update_STATUS() gives option to start board without overwriting memory
*/

#include "altimeter_rec.h"


unsigned long rec_rate = 1000;            // recording rate in ms                   (now: once in second)
unsigned long rate = 200;                 // altitude reading rate in ms            (now: once in 0.2 seconds)
float minHeight = 0.5;                    // threshold from which recording starts  (now: everything higher than 30cm)
unsigned long duration = 10000;           // duration of recording in ms            (now: 10 seconds)


void setup() {


    Serial.begin(115200);
    begin_ALTIMETER();
    read_MEMORY("nosaukums");             // your name for saved flight
    update_STATUS(enable);
    

}

void loop() {


    read_ALTIMETER(rate);
    altimeter_REC(rec_rate, minHeight, duration);

    
}
