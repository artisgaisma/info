/* Mārtiņš Kļaviņš */
#include "setup_mode.h"

struct group_param Parameters;
struct group_param* ptr_to_param = &Parameters;

uint8_t* param_addr[] = {
    ptr_to_param->netClass,
    ptr_to_param->ip,
    ptr_to_param->dest_ip,
    ptr_to_param->universe
};


/*
 * function - print formated group data
 * ====================================
 */
void getData(){

    Serial.print(DATA_PREFIX);
    if ( groupCount > 0 ){
        for( uint8_t n = 0; n < groupCount; n++ ){
            Serial.print(MESSAGE_PREFIX);
            Serial.print( INPUT_CREATE + n + "," + GROUP_NAME[n] + "," + GROUP_TYPES[n] + INPUT_SUFFIX );
            Serial.print(MESSAGE_SUFFIX);
        }
    }
    if ( parameterCount > 0 ){
        // Class-A
        Serial.print(F("{<a:0,0,enable,0,1,"));     Serial.print(Parameters.netClass[0]); Serial.print(F(",1>}"));
        // IP address
        Serial.print(F("{<a:1,0,octet 1,0,255,"));  Serial.print(Parameters.ip[0]);       Serial.print(F(",1>}"));
        Serial.print(F("{<a:1,1,octet 2,0,255,"));  Serial.print(Parameters.ip[1]);       Serial.print(F(",1>}"));
        Serial.print(F("{<a:1,2,octet 3,0,255,"));  Serial.print(Parameters.ip[2]);       Serial.print(F(",1>}"));
        Serial.print(F("{<a:1,3,octet 4,0,255,"));  Serial.print(Parameters.ip[3]);       Serial.print(F(",1>}"));
        // destination address
        Serial.print(F("{<a:2,0,octet 1,0,255,"));  Serial.print(Parameters.dest_ip[0]);  Serial.print(F(",1>}"));
        Serial.print(F("{<a:2,1,octet 2,0,255,"));  Serial.print(Parameters.dest_ip[1]);  Serial.print(F(",1>}"));
        Serial.print(F("{<a:2,2,octet 3,0,255,"));  Serial.print(Parameters.dest_ip[2]);  Serial.print(F(",1>}"));
        Serial.print(F("{<a:2,3,octet 4,0,255,"));  Serial.print(Parameters.dest_ip[3]);  Serial.print(F(",1>}"));
        // ArtNet universe
        Serial.print(F("{<a:3,0,universe,0,255,")); Serial.print(Parameters.universe[0]); Serial.print(F(",1>}"));
        // DMX offset
    }
    Serial.print(DATA_SUFFIX);
    
}

/*
 * function - save data (changed paramaters)
 * =========================================
 */
void saveData(){

    EEPROM.update(0, 0);
    EEPROM.put(1, Parameters);

    Serial.print(MESSAGE_PREFIX);
    Serial.print("EEPROM is updated");
    Serial.print(MESSAGE_SUFFIX);

}

/*
 * function - update a group parameter if found
 * =========================================================
 * @param data - the raw parameter String: <u:gId,pId,value>
 */
void updateParameter( String data ){
  
    if ( parameterCount > 0 ){   
        data.replace(INPUT_UPDATE, "");                                   // Clean input
        data.replace(INPUT_SUFFIX, "");
        String groupId = data.substring(0, data.indexOf(','));            // Get group ID
        data = data.substring(data.indexOf(',') + 1, data.length());
        String parameterId = data.substring(0, data.indexOf(','));        // Get parameter ID
        data = data.substring(data.indexOf(',') + 1, data.length());
        String value = data;                                              // Get parameter value

        uint8_t gID = groupId.toInt();
        uint8_t pID = parameterId.toInt();
        *(param_addr[gID]+pID) = (uint8_t)value.toInt();

        Serial.print(MESSAGE_PREFIX);
        Serial.print("Settings update - ");
        Serial.print( groupId + parameterId + " " + value );
        Serial.print(MESSAGE_SUFFIX);
    }

}






bool connectApp(){

    Serial.begin(9600);
    while(!Serial);                     // wait for specified Serial port to be ready
    Serial.print(MESSAGE_PREFIX);
    Serial.println(" ~ PIXELED.FingerWing ~ ");
    Serial.println("   v1.0 /26.07.2020/    ");
    Serial.print(MESSAGE_SUFFIX);

    EEPROM.get(1, Parameters);
    getData();

    return true;
}

void readApp(){

    uint8_t index = 0;
    String stream;
    if ( Serial.available() > 0 ){
        String current = Serial.readStringUntil((char)END_MARKER);
        index = current.indexOf((char)START_MARKER);
        stream = current.substring(index+1);

        if ( SAVE == stream ) saveData();
        else if ( INPUT_GET == stream ) getData();
        else if ( stream.startsWith(INPUT_UPDATE) ) updateParameter(stream);
        else Serial.print("{Unknown command}");

        index = 0;
    }
    
}
