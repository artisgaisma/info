/* Mārtiņš Kļaviņš 2019.07 */
#include <ArtNetMK.h>
#include <artnetDEVICEconfig.h>


#if defined(ARDUINO_SAMD_ZERO) || defined(ESP8266) || defined(ESP32)
    WiFiUDP Udp;
#else
	EthernetUDP Udp;
#endif


// definitions, declarations ---------------------------------------------------------------------------------

struct artnet_reply ArtPollReply;
struct artnet_dmx ArtDmx;
//struct artnet_dmx* pArtDmx = &ArtDmx;

/*	declaration for pointer to a function that has two input arguments and returns nothing					*/
void (*DMX_frame)( uint16_t, uint8_t*);


// functions -------------------- Network Setup --------------------------------------------------------------

void set_IP( uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4 ){
	IPaddress[0] = octet1;
	IPaddress[1] = octet2;
	IPaddress[2] = octet3;
	IPaddress[3] = octet4;
}

void set_MAC( uint8_t oui1, uint8_t oui2, uint8_t oui3, uint8_t nic1, uint8_t nic2, uint8_t nic3 ){
	MACaddress[0] = oui1;
	MACaddress[1] = oui2;
	MACaddress[2] = oui3;
	MACaddress[3] = nic1;
	MACaddress[4] = nic2;
	MACaddress[5] = nic3;
}

void set_Broadcast( uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4 ){
	BCaddress[0] = octet1;
	BCaddress[1] = octet2;
	BCaddress[2] = octet3;
	BCaddress[3] = octet4;
}

void set_destIP( uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4 ){
	dest_ip[0] = octet1;
	dest_ip[1] = octet2;
	dest_ip[2] = octet3;
	dest_ip[3] = octet4;
}

void ClassA_network(){
	#if (CLASS_A_ip)
		// get hard-coded MAC from device and store it in MACaddress
		#if defined(ARDUINO_SAMD_ZERO) || defined(ESP8266) || defined(ESP32)
			WiFi.macAddress(MACaddress);
		#elif defined(WIZ550io_WITH_MACADDRESS)
			W5100.getMACAddress(MACaddress);
			
		#endif
	
		uint16_t octet2 = MACaddress[3] + (ESTA >> 8) + (uint8_t)ESTA;
	
		IPaddress[0] = 2;
		IPaddress[1] = (uint8_t)octet2;
		IPaddress[2] = MACaddress[4];
		IPaddress[3] = MACaddress[5];
		
		BCaddress[0] = 2;
		BCaddress[1] = 255;
		BCaddress[2] = 255;
		BCaddress[3] = 255;
		
		Subnet_Mask[0] = 255;
		Subnet_Mask[1] = 0;
		Subnet_Mask[2] = 0;
		Subnet_Mask[3] = 0;
	
	#endif
}

uint8_t* get_IP(){
	return IPaddress;
}

#ifdef _kind_of_ARDUINO

	void ArtNet_beginETH(){
		Ethernet.begin(MACaddress,IPaddress);
		
		local_ip[0] = Ethernet.localIP()[0];
		local_ip[1] = Ethernet.localIP()[1];
		local_ip[2] = Ethernet.localIP()[2];
		local_ip[3] = Ethernet.localIP()[3];

		Udp.begin(ART_NET_PORT);
		
		#if (CLASS_A_ip)
			//Ethernet.setSubnetMask(Subnet_Mask);
			W5100.setSubnetMask(Subnet_Mask);
		#endif
	}
	
	void ArtNet_beginETH_dhcp(){
		// to make sure the DHCP lease is renewed, call Ethernet.maintain() regularly
		Ethernet.begin(MACaddress);
		
		local_ip[0] = Ethernet.localIP()[0];
		local_ip[1] = Ethernet.localIP()[1];
		local_ip[2] = Ethernet.localIP()[2];
		local_ip[3] = Ethernet.localIP()[3];

		Udp.begin(ART_NET_PORT);
	}

	void ArtNet_beginWiFi(){
		#if defined(ARDUINO_SAMD_ZERO) || defined(ESP8266) || defined(ESP32)
			local_ip[0] = WiFi.localIP()[0];
			local_ip[1] = WiFi.localIP()[1];
			local_ip[2] = WiFi.localIP()[2];
			local_ip[3] = WiFi.localIP()[3];
		#endif
			
		Udp.begin(ART_NET_PORT);
	}
	
#endif

// functions -------------------- Art-Net --------------------------------------------------------------------
/*	returns OpCode from received ArtNet packet or 0 if its not ArtNet. reads DMX, sends ART_POLL_REPLY		*/
#ifdef _kind_of_ARDUINO

	uint16_t ArtNet_read(){
		packetSize = Udp.parsePacket();
		remote_ip[0] = Udp.remoteIP()[0];
		remote_ip[1] = Udp.remoteIP()[1];
		remote_ip[2] = Udp.remoteIP()[2];
		remote_ip[3] = Udp.remoteIP()[3];
	
		if ( (packetSize <= MAX_BUFFER_ARTNET) && (packetSize > 0) ){
			Udp.read(artnetPacket, MAX_BUFFER_ARTNET);

			// Check that packetID is "Art-Net" else ignore
			for( uint8_t i = 0 ; i < 8 ; i++ ){
				if ( artnetPacket[i] != ART_NET_ID[i] ){
					return 0;
				}
			}

			/* vispirms izpilda 9. paketes bitshift un tad OR	*/
			/*         xxxxxxxx									*/
			/* yyyyyyyy00000000									*/
			opcode = artnetPacket[8] | (artnetPacket[9] << 8);

			if ( opcode == ART_DMX && en_read_DMX ){
				sequence = artnetPacket[12];
				physical = artnetPacket[13];
				incomingUniverse = artnetPacket[14] | (artnetPacket[15] << 8);
				dmxDataLength = artnetPacket[17] | (artnetPacket[16] << 8);
			
				if ( dmxCallback && (incomingUniverse == userUniverse) ){
					// calling user function through explicit dereference
					(*DMX_frame)( dmxDataLength, artnetPacket + ART_DMX_START );
				}
			
				return ART_DMX;
			}
			if ( opcode == ART_POLL && en_reply_msg ){
				for( uint8_t i = 0; i < 8; i++ ){
					ArtPollReply.id[i]  	= ART_NET_ID[i];
				}
				ArtPollReply.opCode 		= ART_POLL_REPLY;
				for( uint8_t i = 0; i < 4; i++ ){
					ArtPollReply.ip[i]  	= local_ip[i];
				}
				ArtPollReply.port       	= ART_NET_PORT;
				ArtPollReply.ver	       	= 0;
				ArtPollReply.subH       	= 0;
				ArtPollReply.sub        	= 0;
				ArtPollReply.oemH       	= 0;
				ArtPollReply.oem        	= 0xff;
				ArtPollReply.ubea       	= 0;
				ArtPollReply.status     	= 0xd2;
				ArtPollReply.etsa 			= ESTA;
				memcpy( ArtPollReply.shortname, SHORT_NAME, sizeof(SHORT_NAME) );
				memcpy( ArtPollReply.longname, LONG_NAME, sizeof(LONG_NAME) );
				ArtPollReply.nodereport[0] 	= 0;
				ArtPollReply.numbportsH 	= 0;
				ArtPollReply.numbports  	= 1;
				ArtPollReply.porttypes[0] 	= 0xc0;
				ArtPollReply.goodinput[0] 	= 0x08;
				ArtPollReply.goodoutput[0] 	= 0x80;
				ArtPollReply.swin[0]      	= 0x01;
				ArtPollReply.swout[0]     	= 0x01;
				ArtPollReply.swvideo    	= 0;
				ArtPollReply.swmacro    	= 0;
				ArtPollReply.swremote   	= 0;
				ArtPollReply.style      	= 0x00;
				ArtPollReply.mac[0]     	= 0x00;
				for( uint8_t i = 0; i < 4; i++ ){
					ArtPollReply.bindip[i] = local_ip[i];
				}
				ArtPollReply.bindindex		= 1;
				ArtPollReply.status2		= 0x08;
        
				//send the packet to the broadcast address
				Udp.beginPacket(BCaddress, ART_NET_PORT);
				/* Udp.write(buffer, size)	, kur buffer - baiti, size - bufera garums/izmērs					*/
				/* (uint8_t*)				type casting ( nozīme turpmako interpetēt kā pointeri uz uint8_t)	*/
				/* &ArtPollReply			paņem adresi no struct datu kopas									*/
				Udp.write((uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
				Udp.endPacket();

				return ART_POLL;
			}
		
		}
		else {
			return 0;
		}
	
	}
	uint8_t ArtNet_write( uint16_t Universe, uint8_t DmxFrame[DMX_CHANNELS] ){
		for( uint8_t i = 0; i < 8; i++ ){
			ArtDmx.id[i]	= ART_NET_ID[i];
		}
		ArtDmx.opCode		= ART_DMX;
		ArtDmx.protVersion 	= prot_version << 8;
		ArtDmx.sequence 	= 0;
		ArtDmx.physical 	= 0;
		ArtDmx.portAddress 	= Universe;
		ArtDmx.dmxDataLenght = DMX_CHANNELS << 8; // 0x0018 << 8
		for( uint8_t i = 0; i < 24; i++ ){
			ArtDmx.dmxData[i] = DmxFrame[i];
		}
		
		Udp.beginPacket(dest_ip, ART_NET_PORT);
		Udp.write((uint8_t *)&ArtDmx, sizeof(ArtDmx));
		Udp.endPacket();
		
		return 0;
	}
	
#endif

/* returns a pointer to the start of the DMX data 															*/
uint8_t* get_DmxFrame(){
	/* atgriež artnetPacket array pirmo elementu ( + offset = ART_DMX_START )	*/
    return artnetPacket + ART_DMX_START;
}

/* returns incomingUniverse																					*/
uint16_t get_Universe(){
	/* agriež mainīgā "incomingUniverse" vertību 								*/
    return incomingUniverse;
}

/* setup function for calling another function at DMX data arrival 											*/
void set_DMXcallback( uint16_t selectUniverse, void (*pointer_to_function)( uint16_t, uint8_t* ) ){
/* funkcija, kas aktivizē citu funkciju brīdī, kad tiek saņemta DMX pakete		*/
/* funkcija sagaida divus parametrus:											*/
/*		# izvēlēta juniversa skaitli											*/
/*		# funkciju, kuru izsaukt brīdī, kad pienāk DMX							*/
	userUniverse = selectUniverse;
	DMX_frame = pointer_to_function;			// asigning DMX_frame to pointer_to_function's address
	dmxCallback = 1;
}

/* set DMX channel 																																														
void set_DmxSlot( uint16_t ch, uint8_t value ){
	if ( ch > 25 ){
		pArtDmx->dmxData[ART_DMX_START + ch] = value;
	}
}
*/

