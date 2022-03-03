/* Mārtiņš Kļaviņš */
#ifndef SETUPmode_H
#define SETUPmode_H

#include <Arduino.h>
#include <EEPROM.h>




const String SAVE = "<save>";       // Mask for saving data in EEPROM;
const String INPUT_GET = "<get>";   // Mask for reading data;
const String INPUT_CREATE = "<c:";  // Mask for creating a group;
const String INPUT_UPDATE = "<u:";  // Mask for updating a parameter inside a group;
const String INPUT_SUFFIX = ">";    // Mask for input suffix;

const String DATA_PREFIX = "$";     // Mask for data beginning
const String DATA_SUFFIX = "#";     // Mask for data ending

const byte START_MARKER = 0x7b;     // Translates to: {;
const byte END_MARKER = 0x7d;       // Translates to: };
const String MESSAGE_PREFIX = "{";  // Message beginning
const String MESSAGE_SUFFIX = "}";  // Message ending

const uint8_t groupCount = 5;
const uint8_t parameterCount = 4;

enum group_types {
  SLD,  // 0 - slider
  BUT,  // 1 - button 
  INP,  // 2 - input 
  TXT   // 3 - text(not implemented in APP)
};
const uint8_t GROUP_TYPES[groupCount] = { SLD, SLD, SLD, SLD, SLD };

const String GROUP_NAME[groupCount] = {
    "Class-A network",
    "IP address",
    "destination IP",
    "ArtNet universe",
    "DMX offset"
};

struct group_param {                // ---! byte sized variables are defined as arrays to resolve param_addr[] bug
    uint8_t   netClass[1];          // ClassA IP (0 - disable, 1 - enable)
    uint8_t   ip[4];                // this device IP
    uint8_t   dest_ip[4];           // where to send ArtNet
    uint8_t   universe[1];          // ArtNet universe (for sake of eeprom range is up to 255)
};



bool connectApp();
void readApp();
void saveData();
void getData();
void updateParameter( String input );



#endif
