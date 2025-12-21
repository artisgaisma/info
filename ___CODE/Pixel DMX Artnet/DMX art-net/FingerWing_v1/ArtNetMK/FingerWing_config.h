/*  ==================================================================
	Mārtiņš Kļaviņš
	==================================================================
	Change Log:
		07.2019		- start ...
		25.07.2020	- defines for device ID and configuration
	==================================================================
	Used libraries, datasheets etc:
	==================================================================
	To-Do:
		# read MAC from W5500, W5100 and W5500 compatibility
	==================================================================
	Caveats:
		-	need to follow ethernet library selection
		NB	select correct setSubnetMask()
		NB	follow to desired config.h in ArtNet.cpp
*/
#ifndef FINGERWINGconfig_H
#define FINGERWINGconfig_H



//	info --------------------------- Hardware related ----------------
/*	supported network interfaces:									*/
/*	(default) # W5100 	- be careful with power supply for dev.board*/
/*						Some need 3.3v instead of written 5V		*/
/*						- max socket count = 4 (ethernet.h for conf)*/
/*	W5200,W5500 		- check w5500.h, w5200.h files for socket	*/
/*						count. Also check w5100.h for selected		*/
/*						hardware									*/



// 	definitions, preprocessors ----- MCU related ---------------------
/*	uncomment your choice for device								*/

#define _kind_of_ARDUINO											// if your board is Arduino
//#define _kind_of_ESP												// if your board is ESP



//	specific paramaters ------------ Art-Net related -----------------
/*	correct for your device											*/

// --- device ID
#define SHORT_NAME    	"FingerWing"								// device name max 17 bytes
#define LONG_NAME   	"PIXELED.FingerWing - 25ch ArtNet controller" // device description max 63 bytes
#define ESTA      		0x5053										// ESTA code ("PS")
#define VERSION			2											// device firmware version
#define OEM				0xffff										// OEM code for ArtNet product (OEMunknown=0x00FF)
// --- device configuration
#define DMX_CHANNELS	26											// dmx512 channel count in one frame
#define en_read_DMX		0											// enable to receive dmx512
#define en_reply_msg	1											// enable to replay to discovery messages
#define PAYLOAD			8											// NZS channel count
#define STATUS1			0xd0										// 11-01-0-0-0-0 = 0xd0 (indicators normalMode - set by fronPanel - 0 - normalBoot - RDMon - UBEAoff)
#define PORTSc			0x0001										// port count
#define PORT1			0xC5										// 1-1-000101 = c5 (out_yes - in_yes - ArtNet data)
//#define PORT2
//#define PORT3
//#define PORT4
#define STYLE			1											// 0 - node, 1 - controller
#define STATUS2			0x08										// 000-0-1-0-0-0 = 0x08 (not squawking - no ACN - ArtNet4 - noDHCP - IP set manualy - no web config)
#define discoveryCONFIG	0											// 000-0-0-0-0-0 = 0x00	(unused - ignore VLC - (diag_msg broadcast)- no diag_msg - artPollReply after artPoll - uused)



//	info --------------------------- Network related -----------------
/*	_kind_of_ARDUINO												*/
/*	In WiFi mode configuration is such:								*/
/*		# MAC address is default (manufacturer)						*/
/*		# IP is assigned from router with DHCP						*/
/*	In ETH mode there is 2 options:									*/
/*		# it is possible to set IP and MAC by hand					*/
/*		# also possible to choose DHCP assigned IP					*/
/*	So in terms of ArtNet:											*/
/*		# Use of Class A (2.x.y.z) network is limited by router		*/
/*	<Ethernet.h> library:											*/
/*		# default subnet mask is 255.255.255.0						*/
/*		  if CLASS_A_ip enabled - 255.0.0.0 , otherwise default		*/




#endif