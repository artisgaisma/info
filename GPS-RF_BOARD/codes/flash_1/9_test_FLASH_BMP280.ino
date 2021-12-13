/* Things which is up to USER:
User has to determine when to start/stop altimeter recording
User has to update session ID with update_STATUS(session) if needed
*/

#include "altimeter_rec.h"


unsigned long frequency = 5000;           // recording freq in ms                   (now: once every 5 seconds)
unsigned long rate = 1000;                // reading rate in ms                     (now: once in second)
bool writing = true;                      // switch for recording enable/disable    (now: enabled by default)
float minHeight = 0.0;                    // threshold from which recording starts  (now: everything bigger than zero)


void setup() {


    Serial.begin(115200);
    begin_ALTIMETER();
    begin_FLASH();
    read_FLASH();
    
}

void loop() {


    read_ALTIMETER(rate);
    altimeter_REC(writing, frequency, minHeight);

    
}
