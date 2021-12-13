/*

DONE -  Flash memory functions
DONE -  Altimeter sensor BMP280
DONE -  Write Altimeter data in Flash (ID,Xi,Yi)
DONE -  Read flash push out to Serial

UZMANIBU -  sis kods aiznem jau 47%/42% no atmega328 atminas
UZMANIBU -  ja vajag precizu altimetru, tad var konstanti/nepartraukti nolasit datus
            un meginat izmantot running average (FIR filter), kas butiba ir low pass filter un anules straujas izmainas
            ====================== example ================
            uint8_t index = 0;
            const uint8_t samples = 5;
            double matrica[samples];
            double rAverage = 0;

            double FIR( double merijums ){
                matrica[index] = merijums;
                rAverage = 0;
                for ( uint8_t i = 0; i < samples; i++ ){
                    rAverage = rAverage + matrica[i];
                }
                index++;
                if ( index == samples ) index = 0;
    
                return rAverage/(float)samples;
            }
            ==============================================
UZMANIBU -  varbut isti neko nedod situacija, kur ik pec sekundes veic 5 merijumus pec kartas un izvelk videjo.
            Jo altimetra dati nepartraukti "pulse" 1m robezas gan 100ms intervalos, gan 1s.
            Videjas vertibas stradatu, ja interesetu fiksets augstums (sekundi nekustina alimetru un izvelk videjo no >50 merijumiem).
            Ja svarigs viegls/atrs kods un netrauce 1m kluda, tad noteikti vajag atteikties no videjosanas!
UZMANIBU -  ieviest EEPROM prieks session ID ( read Change Log )
UZMANIBU -  pie parametriem ( samples = 3, setOversampling(4) ) augstuma iegusana un ierakstisana FLASH aiznem 150ms

 * =====================================================================================================
 * Used libraries, datasheets etc:
 *    https://github.com/Marzogh/SPIMemory 
 *    https://github.com/mahfuz195/BMP280-arduino-library ( memory usage - 26%/25% with test code)
 *    https://github.com/adafruit/Adafruit_BMP280_Library ( memory usage - 39%/25% with test code)
 * =====================================================================================================
 * Change Log:
 * 04.12.2020 - added EEPROM for ID and packet count tracking, because in FLASH memory it would take 8Kb
 * 05.12.2020 - added altimeter
 * 06.12.2020 - added base level for height, attempt to stabilise altimeter readouts
 * =====================================================================================================
 * To-Do:
 *    # can Xi overflow? does failed altimeter read get's discarded?
 * NB:
 *    ! erasing FLASH memory will take at least 20 seconds
 *    ! every FLASH memory cell can only be written once, for updating cell please erase it first
 *    ! its not possible to erase only single FLASH memory cell, only whole sector
 *    ! for faster FLASH memory writing - flash.writeByte(ID_ADD, sessionID, NOERRCHK);
*/


#include <SPIMemory.h>
#include <EEPROM.h>
#include <BMP280.h>


// ---- EEPROM memory addresses, change if overlaps with other codes
#define IDaddr            0                 // stored uint8_t
#define packetCountAddr   1                 // stored uint16_t
// ---- FLASH memory packet size ( uint8_t + float + float )
#define packetSize        10
// ---- barometric forecast ( irrelevant, because FFT measures relative height from certain base level )
#define P0                1020.25           // pressure in hPa
// ---- sample rate for height measurements
#define samples           3


SPIFlash flash(A0);
BMP280 bmp;


  // EEPROM memory
  uint8_t sessionID;
  uint16_t packetCount;

  // time
  unsigned long currentMillis;
  unsigned long frekvence = 10000;
  unsigned long lastMillis = 0;             // sekundes vai sekundes desmitdaļas taimeris

  // device
  bool writing = true;
  float Xi;                                 // time
  float Yi;                                 // height
  float baseLevel;




String read_FLASH( uint16_t packetNr ){

    uint32_t address = packetNr * packetSize;
    uint8_t id = flash.readByte(address);
    float Xi = flash.readFloat(address+1);
    float Yi = flash.readFloat(address+5);
    
    return "{alt:lidojums" + String(id) + "," + String(Xi, 1) + "," + String(Yi, 1) + "}";
}

void write_FLASH( uint8_t id, float Xi, float Yi ){

    uint32_t address = packetCount * packetSize;
    flash.writeByte(address, id);
    flash.writeFloat(address+1, Xi);
    flash.writeFloat(address+5, Yi);
    packetCount++;
    EEPROM.put(packetCountAddr, packetCount);

}

void erase_FLASH(){

    flash.eraseChip();                        // will write 255 in all cells
    EEPROM.write(IDaddr, 0);
    packetCount = 0;
    EEPROM.put(packetCountAddr, packetCount);
    
}

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




void setup() {


    Serial.begin(115200);

    if ( bmp.begin() ){
        bmp.setOversampling(4);
        float total = 0.0;
        uint8_t counter = 0;
        for( uint8_t i = 0; i < samples; i++ ){
            float readout = get_altitude();
            if ( readout == readout ){
                total += readout;// - baseLevel;
                counter++;
            }
        }
        baseLevel = total/(float)counter;
    }
    else {
        Serial.println("BMP280 I2C error");
    }
    
    flash.begin();
    /* find out session ID from EEPROM */
    uint8_t lastID = EEPROM.read(IDaddr);
    sessionID = lastID+1;
    EEPROM.write(IDaddr, sessionID);
    /* find out packet count from EEPROM */
    EEPROM.get(packetCountAddr, packetCount);

    uint16_t remainingPackets = packetCount;
    uint16_t packetNr = 0;
    while( remainingPackets > 0 ){
        Serial.println(read_FLASH(packetNr));
        packetNr++;
        remainingPackets--;
    }

    
}

void loop() {

    
    currentMillis = millis();
    /* save data in memory at certain pace, if 'writing' activated */
    if ( (currentMillis - lastMillis >= frekvence) && writing ){
        lastMillis = currentMillis;

        float total = 0.0;
        uint8_t counter = 0;
        for( uint8_t i = 0; i < samples; i++ ){
            float readout = get_altitude();
            if ( readout == readout ){
                total += readout - baseLevel;
                counter++;
            }
        }
        Yi = total/(float)counter;
        Xi = (millis()/100)/10.0;
        write_FLASH(sessionID, Xi, Yi);
    }
    /* if 'writing' disabled, session is over */
    if ( !writing ){
        sessionID++;
        EEPROM.write(IDaddr, sessionID);
    }

    
}
