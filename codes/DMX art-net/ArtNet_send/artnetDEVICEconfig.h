/* Mārtiņš Kļaviņš 2019.07 */
#ifndef artnetDEVICEconfig_H
#define artnetDEVICEconfig_H


//	info --------------------------- Hardware related ----------------
/*	supported network interfaces:									*/
/*		# W5100 ( be careful with power supply for dev.board		*/
/*				Some need 3.3v instead of written 5V				*/


// 	definitions, preprocessors ----- MCU related ---------------------
/*	uncomment your choice for device								*/

#define _kind_of_ARDUINO											// if your board is Arduino
#define _kind_of_ESP												// future option


//	specific paramaters ------------ Art-Net related -----------------
/*	correct for your device											*/

#define SHORT_NAME    	"FingerWing"								// device name max 17 bytes
#define LONG_NAME   	"PIXELED.FingerWing - 24ch Artnet controller" // device description max 63 bytes
#define ESTA      		0x5053										// ESTA code "PS"
#define DMX_CHANNELS	24											// dmx512 channel count in one frame
#define en_read_DMX		0											// enable to receive dmx512
#define en_reply_msg	1											// enable to replay to discovery messages


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

#define CLASS_A_ip		1											// 1 - true, 0 - false



#endif