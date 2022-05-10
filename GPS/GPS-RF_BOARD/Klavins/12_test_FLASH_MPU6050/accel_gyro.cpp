#include "accel_gyro.h"



  // create objects and call the constructors
  SPIFlash flash(A0);
  MPU6050lib mpu;

  // EEPROM memory
  uint8_t sessionID;
  uint32_t packetCount;
  bool startSession = false;

  // time
  unsigned long currentMillis;
  unsigned long lastMillis = 0;             // Accel-Gyro recording timer

  // device
  int16_t accelCount[3];                    // Stores the 16-bit signed accelerometer sensor output
  float Axyz[3];                            // Stores the real accel value in g's
  int16_t gyroCount[3];                     // Stores the 16-bit signed gyro sensor output
  float Gxyz[3];                            // Stores the real gyro value in degrees per seconds
  float gyroBias[3], accelBias[3];          // Bias corrections for gyro and accelerometer
  float SelfTest[6];                        // Gyro and accelerometer self-test sensor output
  float aRes, gRes;                         // scale resolutions per LSB for the sensors
  float TIME;                               // time
  bool record = false;                      // recording state




/* function - read MPU6050 sensor
 * ==============================
 * @return  - state of data availability
 */
uint8_t get_data(){
  
    uint8_t state = 0;                                      // value that will be returned if measurement fails
    /* If data ready bit set, all data registers have new data */
    if ( mpu.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01 ){
      
        mpu.readAccelData(accelCount);                      // Read the x/y/z adc values
        aRes = mpu.getAres();
        mpu.readGyroData(gyroCount);                        // Read the x/y/z adc values
        gRes = mpu.getGres();

        /* Now we'll calculate the accleration value into actual g's */
        Axyz[0] = (float)accelCount[0]*aRes - accelBias[0]; // get actual g value, this depends on scale being set
        Axyz[1] = (float)accelCount[1]*aRes - accelBias[1];   
        Axyz[2] = (float)accelCount[2]*aRes - accelBias[2]; 
        /* Calculate the gyro value into actual degrees per second */
        Gxyz[0] = (float)gyroCount[0]*gRes - gyroBias[0];   // get actual gyro value, this depends on scale being set
        Gxyz[1] = (float)gyroCount[1]*gRes - gyroBias[1];  
        Gxyz[2] = (float)gyroCount[2]*gRes - gyroBias[2];  

        state = 1;
        
    }

    return state;
}

/* function - setting up MPU6050
 * =============================
 */
void begin_ACCEL_GYRO(){

    Wire.begin();                                                   // I2C address  for MPU6050 board is 0x68
    /* read the WHO_AM_I register for board test */
    uint8_t c = mpu.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);

    if (c == 0x68){
        mpu.MPU6050SelfTest(SelfTest);                              // Start by performing self test and reporting values
        if (  SelfTest[0] < 1.0f &&
              SelfTest[1] < 1.0f &&
              SelfTest[2] < 1.0f &&
              SelfTest[3] < 1.0f &&
              SelfTest[4] < 1.0f &&
              SelfTest[5] < 1.0f ){
            mpu.calibrateMPU6050(gyroBias, accelBias);              // Calibrate gyro and accelerometers, load biases in bias registers 
            mpu.initMPU6050();                                      // Initialize device for active mode read of acclerometer, gyroscope
        }
        else {
            Serial.println("error");
        }
    }
    else {
        Serial.println("error");
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
        float ax = flash.readFloat(address+1);
        float ay = flash.readFloat(address+5);
        float az = flash.readFloat(address+9);
        float gx = flash.readFloat(address+13);
        float gy = flash.readFloat(address+17);
        float gz = flash.readFloat(address+21);
        float Time = flash.readFloat(address+25);
        
        Serial.println("{accel-gyro:lidojums_" + String(id) + "," +  String(Time, 1) + "," +
                                                                    String(ax) + "," + 
                                                                    String(ay) + "," +
                                                                    String(az) + "," +
                                                                    String(gx, 1) + "," +
                                                                    String(gy, 1) + "," +
                                                                    String(gz, 1) + "}");
                                                        
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
            float ax = flash.readFloat(address+1);
            float ay = flash.readFloat(address+5);
            float az = flash.readFloat(address+9);
            float gx = flash.readFloat(address+13);
            float gy = flash.readFloat(address+17);
            float gz = flash.readFloat(address+21);
            float Time = flash.readFloat(address+25);
            if ( Time >= start_time ){
                Serial.println("{accel-gyro:lidojums_" + String(id) + "," + String(Time, 1) + "," +
                                                                          String(ax) + "," +
                                                                          String(ay) + "," +
                                                                          String(az) + "," +
                                                                          String(gx, 1) + "," +
                                                                          String(gy, 1) + "," +
                                                                          String(gz, 1) + "}");
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
 * @param id    - session ID
 * @param AXYZ  - acceleration
 * @param GXYZ  - gyro rate
 * @param Time  - recording time
 */
void write_FLASH( uint8_t id, float* AXYZ, float* GXYZ, float Time ){

    uint32_t address = packetCount * packetSize;
    flash.writeByte(address, id);
    flash.writeFloat(address+1, *AXYZ);
    flash.writeFloat(address+5, *(AXYZ+1));
    flash.writeFloat(address+9, *(AXYZ+2));
    flash.writeFloat(address+13, *GXYZ);
    flash.writeFloat(address+17, *(GXYZ+1));
    flash.writeFloat(address+21, *(GXYZ+2));
    flash.writeFloat(address+25, Time);
    packetCount++;
    EEPROM.put(packetCountAddr, packetCount);

}

/* function - data recording in memory
 * ===================================
 * ( discards any failed attempt )
 * -------------------------------
 * @param REC_step  - time between recordings (in ms)
 */
void ACCEL_GYRO_REC( unsigned long REC_step ){

    currentMillis = millis();
    /* save data in memory at certain pace, if enabled */
    if ( (currentMillis - lastMillis >= REC_step) && record ){
        lastMillis = currentMillis;

        if ( (uint32_t)packetCount < maxPackets ){
            if ( startSession == false){
                startSession = true;
                EEPROM.write(IDaddr, sessionID);
            }
            TIME = (millis()/100)/10.0;
            write_FLASH(sessionID, Axyz, Gxyz, TIME );
            Serial.println("rec");
        }
        else ;//Serial.println("full memory");
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
