#include "altimeter_rec.h"



  // create objects and call the constructors
  SPIFlash flash(A0);
  BMP280 bmp;

  // EEPROM memory
  uint8_t sessionID;
  uint32_t packetCount;
  bool startSession = false;

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
//Serial.print("A = \t");Serial.print(A, 1); Serial.println(" m");

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

/* function - setting up W25Q64 and data management
 * ================================================
 * ( data management is facilitated with help from EEPROM (faster for frequent data updates))
 */
void begin_FLASH(){
  
    flash.begin();
    /* find out session ID from EEPROM */
    uint8_t lastID = EEPROM.read(IDaddr);
    if ( lastID == 255 ){
        /* new device (in new AVR MCU eeprom default is 255) */
        EEPROM.write(IDaddr, 0);
        EEPROM.put(packetCountAddr, (uint32_t)0);
    }
    sessionID = lastID+1;
    /* find out packet count from EEPROM */
    EEPROM.get(packetCountAddr, packetCount);
    
}

/* function - for W25Q64 erasing and resetting data management
 * ===========================================================
 * ( data management is facilitated with help from EEPROM (faster for frequent data updates))
 */
void erase_FLASH(){

    EEPROM.get(packetCountAddr, packetCount);
    flash.eraseSection(0, packetCount * packetSize);
    sessionID = 0; EEPROM.write(IDaddr, sessionID);               // necessary to restet variable as well
    packetCount = 0; EEPROM.put(packetCountAddr, packetCount);    // necessary to restet variable as well
    
}

/* function - reads W25Q64 memory and sends to Serial port
 * =======================================================
 */
void memoryDUMP(){

    uint16_t remainingPackets = packetCount;
    uint16_t packetNr = 0;
    
    while( remainingPackets > 0 ){
        uint32_t address = packetNr * packetSize;
        uint8_t id = flash.readByte(address);
        float Xi = flash.readFloat(address+1);
        float Yi = flash.readFloat(address+5);
        Serial.println("{alt:lidojums_" + String(id) + "," + String(Xi, 1) + "," + String(Yi, 1) + "}");
        
        packetNr++;
        remainingPackets--;
    }

}

/* function - reads certain flight data from W25Q64 memory and sends to Serial port
 * ================================================================================
 * @param ID          - session ID
 * @param start_time  - timestamp of the beginnig of certain flight
 */
void read_FLASH( uint8_t sID, float start_time ){

    if ( (sID >= 1) && (sID < sessionID) ){
        uint8_t id = 0;
        uint32_t packetNr = 0;
        uint32_t offset, address;
        while( (id < sID) && (packetNr <= packetCount) ){
            /* go through memory to find beginning of certain session */
            address = packetNr * packetSize;
            id = flash.readByte(address);
            
            packetNr++;
        }
        offset = address;
        packetNr = 0;

        while( id == sID ){
            /* print out certain session from specified starting time */
            address = (packetNr * packetSize) + offset;
            id = flash.readByte(address);
            if ( id != sID ){
                break;
            }
            float Xi = flash.readFloat(address+1);
            float Yi = flash.readFloat(address+5);
            if ( Xi >= start_time ){
                Serial.println("{alt:lidojums_" + String(id) + "," + String(Xi, 1) + "," + String(Yi, 1) + "}");
            }
            
            packetNr++;
        }
    }
    else {
        //Serial.println("invalid ID");
    }

}

/* function - writes data in W25Q64
 * ================================
 * @param id  - session ID
 * @param Xi  - measurement time
 * @param Yi  - measured height
 */
void write_FLASH( uint8_t id, float Xi, float Yi ){

    uint32_t address = packetCount * packetSize;
    flash.writeByte(address, id);
    flash.writeFloat(address+1, Xi);
    flash.writeFloat(address+5, Yi);
    packetCount++;
    EEPROM.put(packetCountAddr, packetCount);

}

/* function - data recording in memory
 * ===================================
 * ( using FIR filter for height measurements )
 * --------------------------------------------
 * @param REC_step      - time between recordings (in ms)
 * @param REC_threshold - minimum height for recording (in meters)
 */
void altimeter_REC( unsigned long REC_step, float REC_threshold ){

    currentMillis = millis();
    /* save data in memory at certain pace, if enabled */
    if ( (currentMillis - lastMillis >= REC_step) && record ){
        lastMillis = currentMillis;

        float rAverage = 0;
        for ( uint8_t i = 0; i < samplesFIR; i++ ){
            rAverage = rAverage + matrica[i];
        }
        Yi = rAverage/(float)samplesFIR;
        if ( Yi >= REC_threshold || apply ){
            if ( !apply ) apply = true;
            Xi = (millis()/100)/10.0;
            if ( (uint32_t)packetCount < maxPackets ){
                if ( startSession == false){
                    startSession = true;
                    EEPROM.write(IDaddr, sessionID);
                }
                write_FLASH(sessionID, Xi, Yi);
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
        case 1: /* enable recording */
            record = true;
            break;
        case 2: /* start next session */
            startSession = false;
            sessionID++;
            break;
        default:
            return;
    }
    
}
