#include "altimeter_rec.h"



  // create objects and call the constructors
  BMP280 bmp;

  // EEPROM memory
  uint8_t packetCount;
  float packetRate, startTime;              // ---- sie var but lokali mainigie

  // time
  unsigned long currentMillis;
  unsigned long lastMillis = 0;             // altimeter recording timer
  unsigned long currentMillis1;
  unsigned long lastMillis1 = 0;            // altimeter reading timer

  // device
  float Xi;                                 // time
  float Yi;                                 // height
  float baseLevel;                          // relative zero level
  bool record = false;                      // recording state
  bool apply = false;                       // flag for the beginning of recording

  // FIR
  uint8_t index = 0;
  double matrica[samplesFIR];




/* function - read BMP280 sensor
 * =============================
 * @return  - height in meters or 'not a number' if measurement fails
 */
double get_altitude(){
  
    double T, P;
    double A = NAN;                           // value that will be returned if measurement fails
    char result = bmp.startMeasurment();
    if ( result != 0 ){
        delay(result);                        // this comes from oversampling ( 1-4 results in 8-45ms )
        result = bmp.getTemperatureAndPressure(T,P);
        if ( result != 0 ) A = bmp.altitude(P,P0);
    }

    return A;
}

/* function - setting up BMP280 and relative zero level
 * ====================================================
 * ( for eliminating error, zero level is read multiple times and averaged )
 */
void begin_ALTIMETER(){

    if ( bmp.begin() ){
        bmp.setOversampling(4);
        float total = 0.0;
        uint8_t counter = 0;
        for( uint8_t i = 0; i < samples; i++ ){
            float readout = get_altitude();
            if ( readout == readout ){
                total += readout;
                counter++;
            }
        }
        baseLevel = total/(float)counter;
    }
    else {
        //Serial.println("BMP280 I2C error");
    }
    
}

/* function - reads EEPROM memory and sends to Serial port
 * =======================================================
 * @param id          - session ID
 */
void read_MEMORY( String id ){

    /* find out packet header from EEPROM */
    packetCount = EEPROM.read(packetCountAddr);
    if ( packetCountAddr == 255 ){
        /* new device (in new AVR MCU eeprom default is 255) */
        packetCount = 0; EEPROM.write(packetCountAddr, packetCount);    // necessary to restet variable as well
        packetRate = 0; EEPROM.put(packetRateAddr, packetRate);
        startTime = 0; EEPROM.put(startTimeAddr, startTime);
    }
    else {
        EEPROM.get(packetRateAddr, packetRate);
        EEPROM.get(startTimeAddr, startTime);
    }
    uint16_t remainingPackets = packetCount;
    uint16_t packetNr = 0;
    
    while( remainingPackets > 0 ){
        uint32_t address = dataAddr + (packetNr * packetSize);
        float Xi, Yi;
        Xi = startTime + (packetNr * packetRate);
        EEPROM.get(address, Yi);
        Serial.println("{alt:" + id + "," + String(Xi, 1) + "," + String(Yi, 1) + "}");
        
        packetNr++;
        remainingPackets--;
    }

}

/* function - writes data in EEPROM
 * ================================
 * @param Yi  - measured height
 */
void write_EEPROM( float Yi ){

    uint32_t address = dataAddr + (packetCount * packetSize);
    EEPROM.put(address, Yi);
    packetCount++;
    EEPROM.put(packetCountAddr, packetCount);

}

/* function - data recording in memory
 * ===================================
 * ( using FIR filter for height measurements )
 * --------------------------------------------
 * @param REC_step      - time between recordings (in ms)
 * @param REC_threshold - minimum height for recording (in meters)
 * @param REC_duration  - duration of recording (in ms)
 */
void altimeter_REC( unsigned long REC_step, float REC_threshold, unsigned long REC_duration ){

    currentMillis = millis();
    if ( currentMillis > REC_duration ) return;
    
    /* save data in memory at certain pace, if enabled */
    if ( (currentMillis - lastMillis >= REC_step) && record ){
        lastMillis = currentMillis;

        float rAverage = 0;
        for ( uint8_t i = 0; i < samplesFIR; i++ ){
            rAverage = rAverage + matrica[i];
        }
        Yi = rAverage/(float)samplesFIR;
        if ( Yi >= REC_threshold || apply ){
            if ( !apply ){
                Xi = (millis()/100)/10.0;
                EEPROM.put(startTimeAddr, Xi);
                packetRate = (REC_step/100)/10.0;
                EEPROM.put(packetRateAddr, packetRate);
                apply = true;
            }
            
            if ( (uint16_t)packetCount < maxPackets ){
                write_EEPROM(Yi);
            }
            else ;//Serial.println("full memory");
        }
        else ;//Serial.println("zero level");
    }

}

/* function - height sampling
 * ==========================
 * ( discards any failed attempt )
 * -------------------------------
 * @param READ_step - time between samplings (in ms)
 */
void read_ALTIMETER( unsigned long READ_step ){

    currentMillis1 = millis();
    if ( currentMillis1 - lastMillis1 >= READ_step ){
        lastMillis1 = currentMillis1;
        float readout = get_altitude();
        if ( readout == readout ){
            matrica[index] = readout - baseLevel;
            index++;
        }
        if ( index == samplesFIR ) index = 0;
    }

}

/* function - update various parameters
 * ====================================
 * @param Status  - parameter name (assigned to integer with enum)
 */
void update_STATUS( uint8_t Status ){

    switch(Status){
        case 0: /* disable recording */
            record = false;
            apply = false;
            break;
        case 1: /* enable recording, overwrite previous data */
            record = true;
            packetCount = 0;
            break;
        default:
            return;
    }
    
}
