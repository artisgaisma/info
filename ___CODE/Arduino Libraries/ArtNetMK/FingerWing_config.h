/* Mārtiņš Kļaviņš */

#ifndef FINGERWINGconfig_H
#define FINGERWINGconfig_H



// 	definitions, preprocessors ----- MCU related ---------------------
/*	uncomment your choice for device								*/

#define _kind_of_ARDUINO


//	specific paramaters ------------ Art-Net related -----------------
/*	correct for your device											*/

// ---- device ID
#define SHORT_NAME    	"FingerWing"								// device name max 17 bytes
#define LONG_NAME   	"FingerWing-ArtNet 25ch comander" // device description max 63 bytes
#define ESTA      		0x7fff //0x5053								// ESTA code ("PS")
#define VERSION			2											// device firmware version
#define OEM				0xffff										// OEM code for ArtNet product (OEMunknown=0x00FF)
// ---- device configuration
#define DMX_CHANNELS	26											// dmx512 channel count in one frame
#define en_read_DMX		false										// enable to receive dmx512
#define en_reply_msg	true										// enable to replay to discovery messages
#define PAYLOAD			8											// NZS channel count
#define STATUS1			0xd0										// 11-01-0-0-0-0 = 0xd0 (indicators normalMode - set by fronPanel - 0 - normalBoot - RDMon - UBEAoff)
#define PORTSc			0x0001										// port count
#define PORT_1          0xC5										// 1-1-000101 = c5 (out_yes - in_yes - ArtNet data)
//#define PORT_2          0xC5
//#define PORT_3          0xC5
//#define PORT_4          0xC5
#define STYLE			1											// 0 - node, 1 - controller
#define STATUS2			0x08										// 000-0-1-0-0-0 = 0x08 (not squawking - no ACN - ArtNet4 - noDHCP - IP set manualy - no web config)
#define discoveryCONFIG	0											// 000-0-0-0-0-0 = 0x00	(unused - ignore VLC - (diag_msg broadcast)- no diag_msg - artPollReply after artPoll - uused)



#endif