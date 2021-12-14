
/*********************************************************************
 *
 *                   UART DMX
 *
 *********************************************************************
 * FileName:        UART_DMX.c
 * Processor:       PIC24
 * Complier:        MPLAB C30/C32
 * Company:         NA
 * Author		Date     	 Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * A.Bishofs 	01/08/2013	UART DMX driver
 * - || -		07/08/2013	time delay after data sent added
 * - || -		20/08/2013	cSumm removed from TX interrupt routine and
 * placed in DMX_send_buffer() 
 * - || -		23/08/2013	time delay before data sent added
 * all timer functions replaced with UART functionality
 * A.Pastars	05/12/2013	Add support for external wireless module
 * A.Pastars	28/03/2019	remove support for external wireless module
 ********************************************************************/
//****** I N C L U D E S **********************************************************/
#include "UART_DMX.h"
#include "utils.h"
#include "main.h"
#include "RDM_PID.h"
#include "RDM_SLAVE_NA.h"
#include <PPS.H>


//****** V A R I A B L E S ********************************************************/
unsigned int DMX_RX_Get_Start_Code; // flag - is set after break condition
unsigned int DMX_RX_Start_Code; // start byte value container

unsigned int DMX_TX_step; // for break generator timer

unsigned char *pDMX_TX_buff; // pointer for UART buffer
unsigned int DMX_TX_buff_size; // size of UART buffer
unsigned int DMX_UART_BRG_VALUE; // brg register value for DMX baudrate

//unsigned int DMX_use_wireless = 0; //set to use wireless DMX module

//****** L O C A L   P R O T O T Y P E S  ******************************************/

void initRND( void );
unsigned long longRND(void);
unsigned char charRND(void);

//****** F U N C T I O N S  *******************************************************/

void DMX_init_UART( void )
{

   DMX_TX_STA  = 0; // clear all
   
   DMX_TX_STAbits.UTXISEL0 = 1;
   DMX_TX_STAbits.UTXISEL1 = 0;	
   // 0b01 = Interrupt when the last character is shifted out of the Transmit Shift Register;
   // all transmit operations are completed
   DMX_UART_BRG_VALUE = (MIPS*1000000L/DMX_BAUD_RATE)/16-1;
   DMX_UART_BRG  = DMX_UART_BRG_VALUE;

   DMX_TX_PRI = DMX_TX_INT_PRIORITY;
   DMX_RX_PRI = DMX_RX_INT_PRIORITY;
   
	#ifdef DMX_UART_OPEN_DRAIN
		DMX_UART_OPEN_DRAIN = 1; //set to open drain
	#endif
	#ifdef DMX_UART_PULLUP
		DMX_UART_PULLUP = 1; //enable pull-up
	#endif
   
   DMX_UART_LAT = 1; /* set pin HIGH state */ \
   DMX_DRIVER_TRIS = 0; // set as output
   DMX_UART_TRIS = 0; // set as output
   
	#ifdef DMX_UART_RX_TRIS
		DMX_UART_RX_TRIS = 1; //set as input
	#endif

	#ifdef DMX_UART_RX_PULLUP
		DMX_UART_RX_PULLUP = 1;
	#endif
	
   DMX_UART_LAT = 1; /* set pin HIGH state */ \
   DMX_DRIVER_TRIS = 0; // set as output
   DMX_UART_TRIS = 0; // set as output
   
   DMX_SET_RX(); // switch to RX mode

   initRND(); //for discovery response delay times
}

void DMX_send_buffer ( unsigned char * buff, unsigned int size)
{
	unsigned int i;
	unsigned char delay;
	UINT16_VAL DMX_TX_cSumm;
	
		pDMX_TX_buff = buff; // set buffer pointer
		DMX_TX_buff_size = size;
		
        DMX_DRIVER_LAT = DMX_DRIVE_RX; /* set RS485 driver RX mode */ 
        DMX_RX_IE = 0; /* disable interrupt */
        DMX_UART_EN = 0; /* disable UART - all UART pins operate as port pins*/ 
        DMX_MODEbits.STSEL = 1; /* 2 stop bits */
        DMX_MODEbits.PDSEL = 0; /* 8bit, no parity */ 
        DMX_UART_EN = 1; /* enable UART */ 
        DMX_TX_EN = 1; /* enable transmit  */

        // perform pre-processing
        switch(buff[0]){ // depends on buffer type
            case RDM_SC_RDM: // RDM
                DMX_TX_cSumm.Val = 0; // clear
				for(i=0;i<size;i++)DMX_TX_cSumm.Val += buff[i]; // generate csumm
				// add csumm at the end of buffer
				buff[i++] = DMX_TX_cSumm.v[1];
				buff[i++] = DMX_TX_cSumm.v[0];
				DMX_TX_buff_size += 2; // update size
				
				DMX_TX_step = DMX_TX_BREAK;// next step - break  
				DMX_UART_BRG  = DMX_UART_BRG_VALUE*5; // 5 = 192uS turnaround time	
                DMX_TX_IF = 0;
                DMX_TX_IE = 1; /* enable TX INT*/
                DMX_TX_REG = 0; // put any char in TX register
                break;
            case 0xFE: // UID encoded
				delay = charRND();
				delay = (delay+(delay>>4))&0x0F; //0 thu 15
            	DMX_TX_step = DMX_TX_MAB;// next step - mark after break  
				DMX_UART_BRG  = DMX_UART_BRG_VALUE*(5+delay); // 5 = 192uS turnaround time	
                DMX_TX_IF = 0;
                DMX_TX_IE = 1; /* enable TX INT*/
				DMX_TX_REG = 0; // put any char in TX register
            
            	break;    
            case 0: // DMX
            	// switch baudrate for break generation
                DMX_DRIVER_LAT = DMX_DRIVE_TX; /* set RS485 driver in TX mode */
            	DMX_UART_BRG  = DMX_UART_BRG_VALUE*8; // 8 = 200uS BREAK and 13uS MAB	
                DMX_TX_step = DMX_TX_MAB; // next step - mark after break   
                DMX_TX_IF = 0;
                DMX_TX_IE = 1; /* enable TX INT*/
                DMX_TX_REG = 0x80; // 7 low bits for BREAK + 1 high bit for MAB
                break;
                
            default: // any other buffers are sent without break sequence
                DMX_DRIVER_LAT = DMX_DRIVE_TX; /* set RS485 driver in TX mode */
            	DMX_TX_step = DMX_TX_SEND_DATA;
				DMX_TX_IF = 1; /* fire TX INT to start auto transmit */
				DMX_TX_IE = 1; /* enable TX INT*/
                break;
        }
        //DMX_TX_IE = 1; /* enable TX INT*/
        
}

void __attribute__((__interrupt__(__save__(RCOUNT)) NAPSV )) DMX_TX_UART_INT(void)
	{

    DMX_TX_IF = 0;
    
	switch(DMX_TX_step){
		
		case DMX_TX_BREAK:
            DMX_DRIVER_LAT = DMX_DRIVE_TX; /* set RS485 driver in TX mode */
			// switch baudrate for break generation
			DMX_UART_BRG  = DMX_UART_BRG_VALUE*8; // 8 = 200uS BREAK and 13uS MAB	     
            DMX_TX_REG = 0x80; // 7 low bits for BREAK + 1 high bit for MAB
			DMX_TX_step = DMX_TX_MAB;
		break;
		
		case DMX_TX_MAB: // mark after break
            DMX_DRIVER_LAT = DMX_DRIVE_TX; /* set RS485 driver in TX mode */
			DMX_UART_BRG = DMX_UART_BRG_VALUE; // restore DMX baudrate
			DMX_TX_step = DMX_TX_SEND_DATA;
		case DMX_TX_SEND_DATA:

		default:
		if (!DMX_TX_buff_size) { // if all bytes transmitted
			dly16; // small delay for mark end
            DMX_SET_RX(); // switch back to RX mode
        } else {	
            while (!DMX_TX_STAbits.UTXBF && DMX_TX_buff_size) { // while The transmitter is able to accept data to transmit
				DMX_TX_REG = *pDMX_TX_buff++; // write to TX hardware FIFO
                DMX_TX_buff_size--; // update info
            }
        }
		break;	
		
	}	
//	DMX_TX_IF = 0;
	
}	

extern int slave_mode;
extern void finalizeDataCollect( void );
void __attribute__((__interrupt__(__save__(RCOUNT)) NAPSV )) DMX_RX_UART_INT(void)
{
unsigned int data;

	DMX_RX_IF = 0;

	data = DMX_RX_REG; // read RX buffer
	
	if( data == 0 ) { // break received ;
		DMX_RX_Get_Start_Code = 1;
      	DMX_RX_Start_Code = SC_UNDEF; // set to unknown type
		dmxin.count = 0;
		finalizeDataCollect();
		
		return;
      	}

	// data byte received
   	data &= 0xFF; // discard 9 bit
   
   	if(DMX_RX_Get_Start_Code){ // determine packet type
	   DMX_RX_Get_Start_Code = 0;
	   DMX_RX_Start_Code = data; // save packet type
	   
	   // init start values if any
	   switch(DMX_RX_Start_Code)
	  {
		  case SC_DMX: // standart DMX packet
		case SC_MAN_ID: //manufacturer specific ID packet
                     DMX_Reset_Packet(DMX_RX_Start_Code);
		  break;

		  
		  case RDM_SC_RDM: // RDM packet
		  	RDM_Reset_Packet();
		  break;
		  
		  default: // other - out of interest
		  
		  break;

		 }
	   
	  }else{
	  // packet data distribution by type
	  switch(DMX_RX_Start_Code)
	  {
		 case SC_DMX: // standart DMX packet
		 case SC_MAN_ID: //manufacturer specific ID packet
		  	DMX_Collect_Packet(data);
		  break;
		  
		  case RDM_SC_RDM: // RDM packet
              if(!slave_mode){
                RDM_Collect_Packet(data);
              }
		  break;
		  
		  default: // other - out of interest
		  
		  break;

		 }
		 
		  
	  } // END packet data distribution by type
	 // END data byte received

}

/////////////////////////////////////////////////////////////////////////////////////
///////////////// RANDOM GENERATOR (FOR DISCOVERY RESPONSE DELAY) ///////////////////
/////////////////////////////////////////////////////////////////////////////////////
unsigned long __attribute__((persistent)) seed_w;
unsigned long __attribute__((persistent)) seed_z;

unsigned long longRND(void)
{
unsigned long res;
	seed_z = __builtin_muluu( 36969U, (unsigned)seed_z ) + (seed_z >> 16);
	seed_w = __builtin_muluu( 18000U, (unsigned)seed_w ) + (seed_w >> 16);
    res = (seed_z << 16) + seed_w;  /* 32-bit result */
    return res;
}

unsigned char charRND(void)
{
	unsigned char rnd8;
	union{
		unsigned long rnd_32;
		unsigned char rnd_16[2];
		unsigned char rnd_8[4];
	} random;
	
	random.rnd_32 = longRND();
	rnd8 = random.rnd_8[0] + random.rnd_8[1] + random.rnd_8[2] + random.rnd_8[3];
	return rnd8;
}

void initRND( void )
{
   seed_w += 99;//meas.mean;
   seed_z += 88;//meas.val;
    if( !seed_w )
      seed_w = 1;
   if( !seed_z )
      seed_z = 1;
}
/////////////////////////////////////////////////////////////////////////////////////


/********************************************************************************************
                  E N D     O F     UART_DMX . C  
 *********************************************************************************************/
