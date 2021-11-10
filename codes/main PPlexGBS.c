#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pps.h>

#include <P24FJ128GA106.H>
#include "uart_drv.h"
#include "cisco_io.h"
#include "utils.h"
#include "main.h"
#include <libpic30.h> //__delay functions

#include "OLED_screen.h"
#include "OLED_hw.h"
#include "leds_driver.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
_CONFIG1 (JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx3 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768)
_CONFIG2 (IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_ON & IOL1WAY_ON & POSCMOD_XT)


//==========prototypes==========
void initHard ( void );
void ISR_enable ( void );
void ms1_virtual_timer(void);
//==========variables==========
volatile unsigned int mS_counter;

void test_butons(void);
void scroll_title_procedure(void);
void wait_procedure (void);
void set_on_wait_state (void);

extern sCISCO_ToDo cisco_ToDo;

//sGroup group[4];
//sVlan Vlan[4];
UINT32_VAL Leds;

//==========SYSTEM interrupts==========
#ifdef __DEBUG
        unsigned int Ms_count_dbg;
	volatile int error = 1;
	#define ERRINT while(error)
#else
	#define ERRINT asm("RESET")
#endif
	
void _ISR_NOPSV _OscillatorFail(void)
{
	INTCON1bits.OSCFAIL = 0;
	ERRINT;
} 
void _ISR_NOPSV _StackError(void)
{
	INTCON1bits.STKERR = 0;
	ERRINT;
}
void _ISR_NOPSV _AddressError(void)
{
	INTCON1bits.ADDRERR = 0;
	ERRINT;
}
void _ISR_NOPSV _MathError(void)
{
	INTCON1bits.MATHERR = 0;
	ERRINT;
}

//////////////////////////////////////////////////////////////////////////////////

//void _ISR_NOPSV _U1RXInterrupt (void)
//{
//	_U1RXIF = 0;
//	unsigned char data = U1RXREG;
//
//	LED_OK ^=1;
//}

//mm

//sCISCO_mode cisco_mode;
volatile unsigned long Out_LinkBit=0;
volatile unsigned long Poe_Bit=0;
volatile unsigned long Out_LinkBit_old=0xFFFFFFFF;
volatile int flag_poe_status=1;

volatile  unsigned int delay_counter;
volatile  unsigned int ms_10_counter;
volatile  unsigned int timeout_ShowCiscoMode;
volatile  int saglaba_mode;

void stop_sroll_title(void);
volatile char rinda[20];
volatile unsigned long led_ind_mas[10];                // masivs kuru izvada uz indikaciju
volatile  unsigned int led_ind_idx=0;                  // masiva indeksa skaititajs


 volatile int ID=0;
 
volatile unsigned int pre_default;                          // karogs kas norada ka nospiestas tris pogas 3 sekundes
volatile  unsigned int pre_def_timeout;                     // laiks kura mera taustinu 1,2,3 nospiesanas laiku 
 
volatile int v1,v2,v3,v4,v5,v6; 

void delay_ms( int mssec )
{
 
 
 delay_counter=mssec;
   
 
 while(delay_counter); 

    
}
void _ISR_NOPSV _T3Interrupt(void) // 1mS clock
{
    IFS0bits.T3IF = 0; //reset timer interrupt flag

    mS_counter++;
    if(ms_since_last_cisco_rx < 0xFFFF)ms_since_last_cisco_rx++;


 //  jauna dala   
    if (delay_counter) delay_counter--;
    
     if  (ms_10_counter!=0)  ms_10_counter--;
    
    if (timeout_ShowCiscoMode!=0) timeout_ShowCiscoMode--;
    
// scroll_title_procedure();
    
}

void _ISR_NOPSV _T4Interrupt(void) // 2 mS LCD
{
    IFS1bits.T4IF = 0; //reset timer interrupt flag

  if  (ID==5)  
  { 
     scroll_title_procedure();
     
     wait_procedure();
     
     if (pre_default==0)  
                       {
                                                                          // ja visi taustini nospiesti tad timeout aug ja ne nulle
                          if ((!Buton1)&& ( ADC1BUFF < 150 )&&(!Buton2)) pre_def_timeout++;   else pre_def_timeout=0;
           
                           if (pre_def_timeout>= (3000/2))  {pre_def_timeout=0; pre_default=1;}   // uzstada ka default aktivizets 
         
                       }
  }  
     
}


void _ISR_NOPSV _SPI1Interrupt(void) // SCT SPI
{
//	LED_LATCH = 1;
	Nop();
	Nop();
//	LED_LATCH = 0;
	IFS0bits.SPI1IF = 0; //reset SPI interrupt flag
}

void displayTitle( void )
{
int i;
	SetBrightness (1);
	fillrect(0-X_OFFSET,0,X_SIZE,Y_SIZE,COL_GRAY);
	fillrect(8-X_OFFSET,8,X_SIZE-16,Y_SIZE-16,COL_NORMAL);
	setDisplay (2,2,FONT_12x16,COL_INVERSE);
	putstr( "Flare RZ" );
    setDisplay (2,5,FONT_6x8,COL_INVERSE);
   
        putstr( "12 pix" );
   
        
	setDisplay (10,5,FONT_6x8,COL_INVERSE);
//	putstr( VERS_STR );
    
    putstr( "BU-BU" );
	for( i=1; i<10; i++)
	{
		SetBrightness (i);
		delay_ms(100);
	}
	for( i=10; i<256; i++)
	{
		SetBrightness (i);
		delay_ms(10);
	}
	//clear
    clrscr();
    enableOLEDoffset();
//	fillrect(0,0,X_SIZE,Y_SIZE,COL_INVERSE);
}





void TEST_ID()
{

int i,v;
int  nosp,vala;

    ID=0;               //
    
                         // 1 parbauda vai poga nav nospiesta 
                         // 2 ja poga nospiesta gaida kad atlaidis
                         // 3 Parsledz uz analogo ieeju
                         // 3 nomera ar ACP spriegumu
                         // 4 uzstada uztada ID
 
    _CN12PUE = 1;
     Nop();
     Nop();
     Nop();
     Nop();
     
     
 //   delay_ms(10); 
     
     
//wait_buton_id:   

 // mans  LED_OK^=1;
    
 // if (Buton_ID==0) goto wait_buton_id;  
     
 
 //  _CN12PUE = 0;
   
//-------

   AD1CON1 = 0;
   
   AD1PCFG = Hex(0111,1111,1111,1111);
   AD1CSSL = Hex(1000,0000,0000,0000);
// AD1CSSL = Hex(1111,1111,1111,1111);
    
   AD1CON1 = Hex(0000,0000,1110,0000);
   AD1CON3 = Hex(0000,1111,0000,0001);
   AD1CON2 = Hex(0000,0100,0011,1100);
   AD1CHS  = Hex(0000,0000,0000,0000);

   AD1CON1bits.ADON = 1;
   AD1CON1bits.ASAM = 1;   
    
poga_ID_parbaude: 
   
    _CN12PUE = 1;
     Nop();
     Nop();
     Nop();
     Nop();
     
                                // parbauda vai LCD plate pieslegta
     
      nosp=0,vala=0;
     
      for (i=0;i<6;i++)
      {    
       delay_ms(10);
       v=ADC1BUFF;
       
       if (v > 900 ) vala++;   // nav  LCD modula
       
       if (v < 100 ) nosp++;   // poga atrodas nospiesta stavokli
   
      }
   
   
   _CN12PUE = 0;
 
   
   if (vala > 4) { ID=0; return;} //>4 merijumi uz linijas gais
   
    if (nosp > 4) goto poga_ID_parbaude;  //poga nospiesta
   
   
   
    for (i=0;i<6;i++)
        
    {     
   delay_ms(10);
   v=ADC1BUFF;
   
   if ((v > 204)&&(v <255))   {ID=5; break; }

    }
   
 
}
/////////////////////////////////////////////////


volatile int key;
volatile unsigned oldkey;
volatile unsigned kbStartTime, kbRepeatTime, kbDelayTime;



unsigned kbhit( void )
{
unsigned curkey;

     key = 0;

    if (enable_wait==1)  return 0;//  kamer  nav aparats uzladejies pogas bloke 
     
    if  (ms_10_counter)  return 0;
   
     ms_10_counter=40;// 10
   
	curkey = 0;

   
        if (!Buton1)
            curkey |= kbUp;
        if ( ADC1BUFF < 150 )   
            curkey |= kbDown;
        if (!Buton2)
            curkey |= kbOk;
    
 

   if( !curkey ) {
      kbStartTime = 0;
      kbRepeatTime = 0;
      kbDelayTime = 0;
      }

   if( curkey && !oldkey ) {
      key = curkey;
      }
    
                                                                                                                                                                                                                                                                                                                                                                                                                                                          
   else if ( curkey == oldkey ) {
      if( curkey == kbUp || curkey == kbDown ) {
	 if( kbStartTime < 50 )
	    ++kbStartTime;
	 else {
	    if( kbStartTime < 400 )
	       ++kbStartTime;
	    kbRepeatTime += kbStartTime;
	    if( kbRepeatTime >= 512 ) {
	       kbRepeatTime = 0;
	       key = curkey;
	       }
	    }
	 }
      if( curkey == kbOk ) {
	 if( kbDelayTime < 120 ) // 1.2 sec
	    ++kbDelayTime;
	 else {
	    if( kbDelayTime == 120 ) {
	       ++kbDelayTime;
	 //    key = kbMenuLong;
	       }
	    }
	 }
      }
 
   oldkey = curkey;

return key;
}


int getch( void )
{

   do {
      kbhit();
      } while ( !key );

return key;

}


/////////////////////////////////////////////////////////////////
//#define  count_mode 5             // rezims skaits


unsigned char selected_mode[count_mode+2];

  
unsigned char sel_set_mode=0;                   //  0  no SET MODE 
                                                //  1  No
                                                //  2  YES

volatile unsigned sel_time_out=0;                     // laiks cik ilgi strada selektesanas menu

volatile unsigned menu_time_out=0;                     // laiks cik ilgi atrodas kada (neizveleta menu)  menu


const char title_text[count_mode+2][30]={

                        " Unmanaged ",	
                        " Managed",    
                   //     " Managed + DHCP ",
                        " Managed + 2 groups/VLANs ",    
                        " Managed + 3 groups/VLANs ",   
                        " Custom/Unknown ",
                        " Default parameters set ? "  
                       };
 /* 
char title_text[5][30]={

                        "Unmanaged ",	
                        "Managed ",    
                        "Managed + DHCP ",
                        "Managed + 2 groups/VLANs ",    
                        "Managed + 3 groups/VLANs "     
                       };
   */
char comment_text[count_mode+2][13][18]=

                         {
                          /*  "add ports 1,2 to group 1;   ",
                            "ports 3,4 to group 2;       ", 
                            "ports 5,6 to group 3;       ",
                            "ports 7,8,9,10 to Trunk     ",  
                            "(compatible with EZ-LAN)    "
                          */ 
  //  all advanced features disabled
 //  basic managed switch features enabled (RSTP, IGMP, QoS). Good for basic use with sACN, MA-Net, Hog-Net, Dante ect  
 //same + DHCP address server running  
 // add ports 1,2,3 to group 1; ports 4,5,6 to group 2; ports 7,8,9,10 to Trunk   (compatible with EZ-LAN)

   
                             {     
                            "                ", 
                            "  all advanced  ",
                            "    features    ",
                            "    disabled    ",
                            "                ",        
                            ""
                             },
                             { 
                         //   "                ", 
                         //   "                ",
                            "  basic managed ",
                            "switch features ",
                            "    enabled     ",
                            "  (RSTP, IGMP,  ",
                            " QoS). Good for ",
                            " basic use with ",
                            " sACN, MA-Net,  ",
                            " Hog-Net, Dante ",
                            " ect.           ",
                            "                ", 
                            "                ",        
                                    
                            ""
                             },
                        /*     
                             {        
                            "                ", 
                            "   same + DHCP  ",
                            " address server ",
                            "   running      ",
                            "                ",        
                            ""
                             },
                          */   
                             {
                        //    "                ", 
                        //    "                ",
                            " add ports 1,2,3",
                            "   to group 1;  ",
                            "   ports 4,5,6  ",
                            "   to group 2;  ",
                            " ports 7,8,9,10 ",
                            "    to Trunk    ",  
                            "  (compatible   ",
                            "  with EZ-LAN)  ",
                            "                ", 
                            "                ",     
                            ""
                             },
                             {   
                         //   "                ", 
                         //   "                ",
                            "  add ports 1,2 ",
                            "   to group 1;  ",
                            "   ports 3,4    ",
                            "   to group 2;  ", 
                            "   ports 5,6    ",
                            "   to group 3;  ",
                            " ports 7,8,9,10 ",
                            "    to Trunk    ",  
                            "  (compatible   ",
                            "  with EZ-LAN)  ",
                            "                ", 
                            "                ",        
                            ""  
                             },    
                             
                        
                                                          //  Custom MODE
                            { 
                            "                ",      
                            "  RSTP          ",
                            "  IGMP          ",
                            "  QoS           ",
                            "  DHCP          ", 
                            " 2  gr.VLAN     ", 
                            " 3  gr.VLAN     ",
                            " No gr.VLAN     ",        
                            "                ",
                           
                         //   "                ",         
                                 
                          //  "                ", 
                          //  "   ??????????   ",
                          //  "   ??????????   ",
                          //  "   ??????????   ",
                          //  "                ",  
                                    
                                    
                            ""
                             },       
                            
                                             // Default  parametrs SET 
                              { 
                            "                ",      
                            "                ",
                            "                ",
                            "                ",
                            "                ", 
                            "                ", 
                            "                ",
                            "                ",        
                            "                ",
                           
                         //   "                ",         
                                 
                          //  "                ", 
                          //  "   ??????????   ",
                          //  "   ??????????   ",
                          //  "   ??????????   ",
                          //  "                ",  
                                    
                                    
                            ""
                             }   
                             
                             
                         };



//#define dal_koef  12                            // atieciba atrumiem vertikalais srol pret horizontalo
#define digit_scroll_V_time_const     100/2      //Vertikala  aizkave  ms

#define digit_scroll_H_time_const     30/2 //  1200/2 //1200  4000 666*2//430//666 //666         //Horizontala aizkave izvadama simbola laiks  ms
//#define digit_scroll_H_time_const  4000/dal_koef  //666*2//430//666 //666         // izvadama simbola laiks 


volatile int sroll_curent_digit=0;      // tekosais izvaditais burts
volatile int digit_scroll_H_time=0;          // piksela  scrolesanas laiks milisekundes
volatile int digit_scroll_V_time=0;          // piksela scrolesanas laiks milisekundes
volatile int vert_tek_pix_lcd=0;       // simbola pikselu skaititajs
volatile int vert_tek_rind_sk=0;
volatile int hor_tek_colonas_sk=0;

volatile int mode=3;                  // !!!!!!!!!!!!!!!!!!!!!!!!!!   pagaidamizveletais rezims skaits    

volatile int eanble_scroll_mode=0;    // atlauja scrolet  
volatile int eanble_V_scroll=0;    // atlauja scrolet Vertikali
volatile int eanble_H_scroll=0;    // atlauja scrolet Horizontali
volatile int preset_V_scroll=0;    // vajag  scrolet Vertikali
volatile int preset_H_scroll=0;    // vajag  scrolet Horizontali
volatile int delay_V_scroll=0;     //  aizkave Vertikali srollesanai
volatile int delay_H_scroll=0;     //  aizkave horizontalai  srollesanai
volatile int repet_V_scroll=0;     //  atkartosana Vertikali srollesanai
volatile int repet_H_scroll=0;     //  atkartosana horizontalai  srollesanai
//----------------------------------------------------------------------



void start_sroll_V(void)
{

 int i;      
    
     repet_V_scroll=0;
 
 
          for(i =0; i < 30; i++)     //  lidz 30 riddam teksta
     {    
          
      if ((comment_text[mode][i][0])==0) break;   
     }    
    
        
      if (i>5)  preset_V_scroll=1;   else preset_V_scroll=0;                         // vajag vai nevajag vertikalo skrolesanu
  
     setDisplay_ram (18,3,FONT_6x8,0);
     putstr_ram (comment_text[mode][0]);
     setDisplay_ram (18,4,FONT_6x8,0);
     putstr_ram (comment_text[mode][1]);
     setDisplay_ram (18,5,FONT_6x8,0);
     putstr_ram (comment_text[mode][2]);
     setDisplay_ram (18,6,FONT_6x8,0);
     putstr_ram (comment_text[mode][3]);
     setDisplay_ram (18,7,FONT_6x8,0); 
     putstr_ram (comment_text[mode][4]);    
    
     
    
  digit_scroll_V_time=digit_scroll_V_time_const;
  
  
  

  vert_tek_pix_lcd=7;
  vert_tek_rind_sk=4;
 
    
}
//----------------------------------------------------------------

void start_sroll_H(void)
{
    
 int len,i;   
    
    repet_H_scroll=0;
 
    len=strlen(title_text[mode]); 
    
    if (len >8)  preset_H_scroll=1;   else preset_H_scroll=0;                         // vajag vai nevajag horizontalo skrolesanu
    
 
  

         if ( selected_mode[mode]==0 )    setDisplay_ram (18,0,FONT_12x16,0);             
  
                else  setDisplay_ram (18,0,FONT_12x16,1);                     //  Uzliek kursoru uz nakamo simbolu aiz kadra malas
    
    
                                             // izlek 8 simbolus =1 ieladetu
    
  //   setDisplay_ram (18,0,FONT_12x16,0); 
  //   putstr_ram (title_text[1]);    
    
   
    
  
    for(i =0; i < 9; i++)
    {    
   //  putch(title_text[mode][i]); 
 
     putchf_ram(title_text[mode][i] );
    }
  /*  
    if (len==8)
    {
        
        Scrol_ram_hor();  
        Scrol_ram_hor(); 
        Scrol_ram_hor(); 
        Scrol_ram_hor(); 
        Scrol_ram_hor(); 
        Scrol_ram_hor(); 
        
    }
   */ 
    sroll_curent_digit=i-1;
    hor_tek_colonas_sk=0;     
    
   
 
      

  digit_scroll_H_time=digit_scroll_H_time_const;    
    

}
//----------------------------------------------------------------



//----------------------------------------------------------------------
void start_sroll_title(void)
{
    
 
    
        stop_sroll_title();
        clrscr();                   // notira ekranu
        clrscr_ram();               // notira ram
  
        setDisplay_ram (18,2,FONT_6x8,0);
        putstr_ram("------------------");     // atdalosa stripa
      
        start_sroll_H();
        if ( preset_H_scroll==1) delay_H_scroll=2000/2; // sakuma aizkave
 
        start_sroll_V();
        if ( preset_V_scroll==1) delay_V_scroll=4000/2;   // sakuma
 
    
        copy_ram_to_LCD (); 

        eanble_scroll_mode=1;

  
}
//------------------------------------------------------


void stop_sroll_title(void)
{
    
 //   DISABLE_INT 
    eanble_scroll_mode=0; 
     preset_H_scroll=0;
     preset_V_scroll=0;
 //   Scroll_H_16_title_stop(); 
 //   ENABLE_INT
    
}
void scroll_title_procedure(void)
{
 int izv_uz_lcd;   
    
    
     izv_uz_lcd=0;
            
     if  (eanble_scroll_mode==0) return;        
    
    
  
   if (delay_H_scroll!=0)   delay_H_scroll--;
 
    

    
  if (delay_H_scroll==0)
 {    
       
  
     digit_scroll_H_time--;    
       
  if  ((digit_scroll_H_time==0)&& ( preset_H_scroll==1))  
       
  
          {  
    
               
                Scrol_ram_hor();
                
                izv_uz_lcd=1;
                
                 hor_tek_colonas_sk++;
                
        if (  hor_tek_colonas_sk==12)
            {              
                 
               hor_tek_colonas_sk=0;
            
                sroll_curent_digit++;
 
    
             //   if (title_text[mode][sroll_curent_digit]==0 ){ delay_H_scroll=2000;repet_H_scroll=1; sroll_curent_digit=0; }
                
                if (title_text[mode][sroll_curent_digit]==0 ) { sroll_curent_digit=0;  repet_H_scroll=1;}
  
                
                
            if ( selected_mode[mode]==0 )    setDisplay_ram (18+96,0,FONT_12x16,0);            
  
                else  setDisplay_ram (18+96,0,FONT_12x16,1);                     //  Uzliek kursoru uz nakamo simbolu aiz kadra malas
     
             //  putch( title_text[mode][sroll_curent_digit]);
                putchf_ram(title_text[mode][sroll_curent_digit] );
                             
              if (( sroll_curent_digit==8 )&& (repet_H_scroll==1)) { delay_H_scroll=2000/2;repet_H_scroll=0;}
                
           } 
            


            digit_scroll_H_time=digit_scroll_H_time_const;     // izliek jaunu timeout atskaiti
 
          }
    
  }
  
 
     
    if (delay_V_scroll!=0)   delay_V_scroll--;
      
    if (delay_V_scroll==0)
 {    
      
     
     
     digit_scroll_V_time--; 
     
     
     
  if  ((digit_scroll_V_time==0)&&( preset_V_scroll==1))
      {   
             if ( vert_tek_pix_lcd == 0) 
                  {
        
         
         
                   setDisplay_ram (18,8,FONT_6x8,0); 
                   putstr_ram (comment_text[mode][vert_tek_rind_sk]);
         
               //    putstr_ram (comment_text[0]);
          
       
                   }
   
    
            vert_tek_pix_lcd++;
            if (vert_tek_pix_lcd>7)
        
        
                  {
                  vert_tek_rind_sk++;
            
         
                 if (comment_text[mode][vert_tek_rind_sk][0] == 0 ) { vert_tek_rind_sk=0;    repet_V_scroll=1;}
         
        
                  vert_tek_pix_lcd=0;  
                  
                   if (( vert_tek_rind_sk==5 )&& (repet_V_scroll==1)) { delay_V_scroll=4000/2;repet_V_scroll=0;}
                  
                  }
    
          
    
            Scrol_ram_vert();
            
             izv_uz_lcd=1;
   
            digit_scroll_V_time=digit_scroll_V_time_const;     // izliek jaunu timeout atskaiti 
            
  
            
       }   

     }
    
  
    
     if ( izv_uz_lcd )   copy_ram_to_LCD ();
     
     
     if ( sel_time_out!=0 )   // parbauda vai nav parak ilgi selektesanas menu
       {
        if (sel_time_out==1)  {
            
                              if (  pre_default!=0)
                              {
                              pre_default=0; 
                              mode=saglaba_mode;  
                                  
                                  
                              }   
                               sel_set_mode=0; 
                               start_sroll_title ();
                               menu_time_out=20000/2;  //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                               }
        
        sel_time_out--;
       } 
  

     if ( menu_time_out!=0 )   // parbauda vai nav parak ilgi selektesanas menu
       {
        if (menu_time_out==1)  
        
                            {
            int i;
                                  for( i=0; i<count_mode+1; i++)   
                                  {                                         
                                   if (selected_mode[i]==1) break;                                 // atrod izveleto modi
                                  }
                                  mode=i;                                                           // uzstada to indicesanas rezima
            
                                  start_sroll_title (); 
                            }
        
        menu_time_out--;
       } 
  

    
    
}

//////////////////////////////////////////////////
/*
  //  struktura kas raksturo CISCO modi

   //   0 - nav  zinama
   //   1 - ON
   //   2 - OFF
   //   3 - nav definets
 
typedef union tCISCO_mode {
    unsigned int all_mode;

    struct {
        unsigned RSTP : 2;
        unsigned IGMP : 2;
        unsigned QoS : 2;
        unsigned DHCP : 2;
        
        unsigned VLANs_2gr : 2;
        unsigned VLANs_3gr : 2;
        unsigned unk_1 : 4;
       
    };
} sCISCO_mode;

*/
/*
void identifice_cisco_modi (void)
{
//unsigned int nosaka_modi;   

//      nosaka_modi=all_mode&0x0FFF;
      
#define    OFF    2
#define    ON     1      
      
      if ((cisco_mode.RSTP==OFF)&&(cisco_mode.IGMP==OFF)&&(cisco_mode.QoS==OFF)&&(cisco_mode.DHCP==OFF)&&(cisco_mode.VLANs_2gr==OFF)&&(cisco_mode.VLANs_3gr==OFF))
           {mode=0; return;}
     
      if ((cisco_mode.RSTP==ON)&&(cisco_mode.IGMP==ON)&&(cisco_mode.QoS==ON)&&(cisco_mode.DHCP==OFF)&&(cisco_mode.VLANs_2gr==OFF)&&(cisco_mode.VLANs_3gr==OFF))
           {mode=1; return;}
          
      if ((cisco_mode.RSTP==ON)&&(cisco_mode.IGMP==ON)&&(cisco_mode.QoS==ON)&&(cisco_mode.DHCP==ON)&&(cisco_mode.VLANs_2gr==OFF)&&(cisco_mode.VLANs_3gr==OFF))  
           {mode=2; return;}
          
      if ((cisco_mode.RSTP==ON)&&(cisco_mode.IGMP==ON)&&(cisco_mode.QoS==ON)&&(cisco_mode.DHCP==OFF)&&(cisco_mode.VLANs_2gr==ON)&&(cisco_mode.VLANs_3gr==OFF))
           {mode=3; return;}
          
      if ((cisco_mode.RSTP==ON)&&(cisco_mode.IGMP==ON)&&(cisco_mode.QoS==ON)&&(cisco_mode.DHCP==OFF)&&(cisco_mode.VLANs_2gr==OFF)&&(cisco_mode.VLANs_3gr==ON))  
            {mode=0; return;}
      
      
      
       {mode=5; return;}       // mode vai nu ir nepareiza vai nav atrasta
    
}

*/
/////////////////////////////////////////////////
void set_ind_mas(void)
{
                                         // uzstada indikacijas masivu atkariba no modes
    
    
int i;
unsigned long sak_const;
//volatile unsigned long led_ind_mas[10];                // masivs kuru izvada uz indikaciju
//volatile  unsigned int led_ind_idx=0;                  // masiva indeksa skaititajs



                  //0       " Unmanaged ",	
                  //1       " Managed",    
                  //nav     " Managed + DHCP ",
                  //2      " Managed + 2 groups/VLANs ",    
                  //3      " Managed + 3 groups/VLANs ",   
                  //4      " Custom/Unknown ",
                  //5      " Default parameters set ? "  



       led_ind_idx=0;                               // nomet indeksu nulle lai indikacija saktos no jauna  
       
       
       //  ???????? ko darit ar status diodi 0x00000020
       
       sak_const=0;
                                                   // ja modei ir trunk kanali tos uzliek
       if ((mode==2)||(mode==3)) sak_const=0x36D80000;
       

       for( i=0; i<10; i++)  led_ind_mas[i]=sak_const;      // notira masivu

       switch(mode)
                   {
                   case 0:
                          led_ind_mas[0]|=0xFFFFFFDF; 

                          break;
               
                   case 1:
                          led_ind_mas[0]|=0xFFFFFFDF;
                         
                   
                          break;
                    
                   case 2:
                                                      // VAN1
                          led_ind_mas[0]|=0x000000DF;
                          
                                                      // VAN2
                          led_ind_mas[4]|=0x0003FF00;
                          led_ind_mas[6]|=0x0003FF00;
                          break;
                 
                          
                    case 3:
                          led_ind_mas[0]|=0x0000001F;
                          led_ind_mas[2]|=0x000006C0;
                          led_ind_mas[4]|=0x000006C0;
                          led_ind_mas[5]|=0x00033000;
                          led_ind_mas[7]|=0x00033000;
                          led_ind_mas[9]|=0x00033000;
                          
                          break;
                   case 4:
                   case 5:
                  default:    
                          Nop();
                   
                          break;
                       
                          
                          
                   } 

       
    
}
//////////////////////////////////////////////////

void LCD_menu (void)
{
int i;
int cur_count_mode;

     kbhit(); //contains background tasks
     
     
    
     
      //--------------------------------------------------------------
    
      if ( pre_default==1)      //   pieliek rezimu selekt DEFAULT
     
                {
                   stop_sroll_title();
                  //      ----
                   clrscr();                   // notira ekranu
                   clrscr_ram();               // notira ram
                   setDisplay_ram (18,2,FONT_6x8,0);
                   putstr_ram("------------------");  
                   
         saglaba_mode=mode;          
         mode=count_mode+1; 
         
                   start_sroll_H();
                   if ( preset_H_scroll==1) delay_H_scroll=2000/2; // sakuma
 
                   setDisplay_ram (18+15,3,FONT_12x16,0);
                   putstr_ram("LOAD ?");  
    
                   setDisplay_ram (18,6,FONT_12x16,0);
                   putstr_ram("YES"); 
                   
                   setDisplay_ram (18+72,6,FONT_12x16,1);
                   putstr_ram("NO"); 
                   
                   
                   sel_set_mode=1;            //  0  no SET MODE 
                                                //  1  No
                                                //  2  YES
                   
                   sel_time_out=20000/2;  //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                   menu_time_out=0;
                                 
                   copy_ram_to_LCD (); 
                   eanble_scroll_mode=1;
                   pre_default=2;
                 }         
                   
     //--------------------------------------------------------------
     
    cur_count_mode=count_mode; 
    
    if (selected_mode[count_mode]==1)  cur_count_mode=count_mode+1;  // ja irieselekta custom unknown mode tad palielina modu skaitu 
  //    if (selected_mode[4]==1)  cur_count_mode=count_mode+1; 
     
 if ( sel_set_mode==0)
 {   
		if( key == kbUp )
		{
            
            
            stop_sroll_title();
            mode++;
            if (mode==cur_count_mode) mode=0;
            
            set_ind_mas();                              //uzstada LED indikacijas masivu
            start_sroll_title();
            menu_time_out=20000/2;  //3000/2;//20000/2;            // 20 sek lidz menu novaksanai  ???? 
           
        //    set_on_wait_state();
			
		}

     
        if( key == kbDown )
		{
            
           
			stop_sroll_title();
      
            mode--;
            if (mode<0 ) mode=cur_count_mode-1;
            set_ind_mas();                              //uzstada LED indikacijas masivu
            start_sroll_title();
            menu_time_out=20000/2;  //3000/2;//20000/2;            //  20 sek lidz menu novaksanai  ????
          
            
        //   set_off_wait_state();  
            
		}

        if( key == kbOk)
		{
            
          if    (selected_mode[mode]!=1)
          {
			       stop_sroll_title();
                  //      ----
                   clrscr();                   // notira ekranu
                   clrscr_ram();               // notira ram
                   setDisplay_ram (18,2,FONT_6x8,0);
                   putstr_ram("------------------");  
                   start_sroll_H();
                   if ( preset_H_scroll==1) delay_H_scroll=2000/2; // sakuma
 
                   setDisplay_ram (18+15,3,FONT_12x16,0);
                   putstr_ram("LOAD ?");  
    
                   setDisplay_ram (18,6,FONT_12x16,0);
                   putstr_ram("YES"); 
                   
                   setDisplay_ram (18+72,6,FONT_12x16,1);
                   putstr_ram("NO"); 
                   
                   
                   sel_set_mode=1;            //  0  no SET MODE 
                                                //  1  No
                                                //  2  YES
                   
                   sel_time_out=20000/2;  //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                   menu_time_out=0;
                                 
                   copy_ram_to_LCD (); 
                   eanble_scroll_mode=1;

            
         }
            
		}
     
     key=0;
  }
  if ( sel_set_mode!=0)
      
       {
      
      
                if( key == kbUp )
                  { 
         
                   setDisplay_ram (18,6,FONT_12x16,1);
                   putstr_ram("YES"); 
                   
                   setDisplay_ram (18+72,6,FONT_12x16,0);
                   putstr_ram("NO"); 
                   
                   
                   sel_set_mode=2;            //  0  no SET MODE 
                                                //  1  No
                                                //  2  YES  
                   sel_time_out=20000/2;  //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                   menu_time_out=0;
                   
                    copy_ram_to_LCD ();  
                   
                    
                    
                  } 
                
                if( key == kbDown ) 
                  {
         
                   setDisplay_ram (18,6,FONT_12x16,0);
                   putstr_ram("YES"); 
                   
                   setDisplay_ram (18+72,6,FONT_12x16,1);
                   putstr_ram("NO"); 
                   
                   
                   sel_set_mode=1;            //  0  no SET MODE 
                                                //  1  No
                                                //  2  YES  
                    sel_time_out=20000/2;  //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                    menu_time_out=0;
                    
                    copy_ram_to_LCD ();   
                    
                    
                  }
                if( key == kbOk) 
                  {  
                    
                      if  ( sel_set_mode==2 )               //  YES aktivs
                    
                        {
                          
                            for( i=0; i<cur_count_mode; i++) selected_mode[i]=0;       // novac visus aktivizetos rezimus
                            selected_mode[mode]=1;                                 // uzstada tekoso modi k? izveleto
                            // !!!!!!vajag nedaudz aizbloket partraukumus           
                            sel_set_mode=0;            //  0  no SET MODE 
                                                       //  1  No
                                                       //  2  YES
                    
                            sel_time_out=0;         //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                            start_sroll_title(); 
                            
                           cisco_ToDo.set_default_config=1;   // uzstada pazimi ka jauzstada parametri
                           
                        if (pre_default!=0)
                            
                             {
                             cisco_ToDo.set_default_config=0;
                             pre_default=0;                      // novac pazimi ka
                             selected_mode[mode]=0;
                             mode=0;                            // default mode pec uzstadisanas
                             selected_mode[mode]=1;
                             cisco_ToDo.delete_config=1;         // defaulto rezinu uzstada 
                             
                             //  pec tam jauzliek parbaude
                             
                             }   
                            
                       //     cisco_ToDo.delete_config=1;    //atslega lai nomestu
                            
                            set_on_wait_state();     // uzliek gaidisanas menu
                            menu_time_out=0;
                            
                        } 
                      
                     if  ( sel_set_mode==1 )                 //  No aktivs
                      
                       {
                           sel_set_mode=0;            //  0  no SET MODE 
                                                       //  1  No
                                                       //  2  YES
                          if (pre_default!=0) 
                          { 
                                                    // atcell defaulto paramertru uzstadisanu
                            mode=saglaba_mode;      
                            pre_default=0;                      // novac pazimi ka
                            
                             stop_sroll_title();
                             //      ----
                             clrscr();                   // notira ekranu
                            clrscr_ram();               // notira ram
                            
                            
                            
                          } 
                            sel_time_out=0;         //3000/2;//20000/2;            //20 sek lidz menu novaksanai  ????
                            start_sroll_title();    
                            menu_time_out=0;
                      
                            
                            
                       }
                      
                      
                  }
                
          key=0;         
      
       }   
     
                  //  nomet  pogu
     
}
//////////////////////////////////////////////////

void drawLine(unsigned int x0,unsigned int y0, unsigned int x1, unsigned int y1, eColor color)
{
    int x = x1-x0;
    int y = y1-y0;
    int dx = abs(x), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;
    for (;;){
      //  drawPixel(x0,y0,color);
     //   fillrect( x0, y0, 1, 1,color);
        
        put_pixel_ram(x0,y0,color);
        put_pixel_ram(x0,y0+1,color);
    //    fillrect( x0, y0+1, 1, 1,color);
        
        e2 = 2*err;
        if (e2 >= dy) {
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}
/*
void SGL::drawVerticalLine(uint16_t x, uint16_t y, uint16_t height,uint16_t color)
{
    uint16_t y1 = min(y+height,_height-1);
    for(int16_t i = y; i < y1; i++){
        drawPixel(x,i,color);
    }
}

void SGL::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
    uint16_t x1 = min(x+width,_width-1);
    for(int16_t i = x; i < x1; i++){
        drawPixel(i,y,color);
    }
}

*/


void drawCircle(unsigned int poX, unsigned int poY, unsigned int r, eColor color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do{
        put_pixel_ram(poX-x, poY+y,color);
        put_pixel_ram(poX+x, poY+y,color);
        put_pixel_ram(poX+x, poY-y,color);
        put_pixel_ram(poX-x, poY-y,color);
        e2 = err;
        if(e2 <= y) {
            err += ++y*2+1;
            if(-x == y && e2 <= x) e2 = 0;
        }
        if(e2 > x) err += ++x*2+1;
    } while(x <= 0);
}

void drawSquare (unsigned int poX, unsigned int poY, unsigned int r, eColor color)
{
    
    int i,j; 
    
    for (j=0;j<r;j++)
    {    
    for (i=0;i<r;i++)put_pixel_ram(poX+i, poY+j,color);
    }

}


/*
                      0+70,40,
                      0+70,44,
                      0+70,48,
                      0+70,52,
                      0+70,56,
                      0+70,60,
                      74,60,
                      78,60,
                      82,60,
                      84,60,
                      86,60,
                      88,60,
                      92,60,
                     92,56,
                     92,52,
                     92,48,
                     92,44,
                     92,40,
                     88,40,
                     84,40,
                     80,40,
                     78,40,
                     74,40,
                     0+70,40      
 * 
                       70,38,
                      70,42,
                      70,46,
                      70,50,
                      70,54,
                      74,54,
                      78,54,
                      82,54,
                      86,54,
                 //     90,54,
                 //     90,54,
                      90,54,
                      90,50,
                      90,46,
                      90,42,
                      90,38,
                      86,38,
                      82,38,
                      78,38,
                      74,38,
                      70,38         
                                                           
*/


#define step    17

int wait_coordinates[step][2]={
                   
                     {70,38},
                     {70,42},
                     {70,46},
                     {70,50},
                     {70,54},
                     {74,54},
                     {78,54},
                     {82,54},
                     {86,54},
                 //     90,54,
                 //     90,54,
                  
                     {86,50},
                     {86,46},
                     {86,42},
                     {86,38},
                     {82,38},
                     {78,38},
                     {74,38},
                     {70,38}         
                              
};

unsigned int wait_counter_ms;
unsigned int wait_state_status;
unsigned int enable_wait;
unsigned int Lcd_wait_conter_ms;
#define pix_time_to_LCD  150/2;

void set_on_wait_state (void)
{
    
     stop_sroll_title();
    
     clrscr();
     copy_BMP_to_ram ();
     copy_ram_to_LCD (); 
     wait_counter_ms=0;
     wait_state_status=0;
     
  //   Lcd_wait_conter_ms=pix_time_to_LCD;
     Lcd_wait_conter_ms=0;
     enable_wait=1;
    
     
    
    
}

void set_off_wait_state (void)
{
    
                                    // mode ir atsasta no ciso modes noteiksanas
    int i;
     enable_wait=0;
     
             stop_sroll_title(); 
            
             for( i=0; i<count_mode+1; i++) selected_mode[i]=0;       // novac visus aktivizetos rezimus
            
             //               selected_mode[3]=1;    // pagaidam oec reset ieselekte 3 modi;
            
             selected_mode[mode]=1;       // ieselekte atrasto modi
           
        //    mode=3;
            start_sroll_title();

    
    
}

void wait_procedure (void)
{
    
int y;  


     if ( enable_wait==0 )  return;
     
     if (Lcd_wait_conter_ms)  { Lcd_wait_conter_ms--;   return;  }
     
     copy_wait_ram_to_LCD ();

     wait_state_status++;

     if (wait_state_status>=step)   wait_state_status=0;
     
     
     y=wait_state_status;
     
        //      for (y=0;y<step;y++)   
      {
      drawSquare (wait_coordinates[y][0], wait_coordinates[y][1], 4, COL_NORMAL);
      
     if (y!=step-1)  drawSquare (wait_coordinates[y+1][0], wait_coordinates[y+1][1], 4, COL_NORMAL);
      
         else  drawSquare (wait_coordinates[0][0], wait_coordinates[0][1], 4, COL_NORMAL);
      
      
      
     if (y!=0) drawSquare (wait_coordinates[y-1][0], wait_coordinates[y-1][1], 4, COL_INVERSE);
        else  drawSquare (wait_coordinates[step-1][0], wait_coordinates[step-1][1], 4, COL_INVERSE);
      
     // delay_ms(150);
      
      Lcd_wait_conter_ms=pix_time_to_LCD;
 
    //  copy_wait_ram_to_LCD ();
      } 

     
     
     
    
}    
//=============================================      ===================================================
//============================================= MAIN ===================================================
//=============================================      ===================================================
int main ( void )
{
unsigned int y;
//unsigned int x;

	initHard();
	ISR_enable();
    //enable LED output
	LED_OE = 0;


//        for (i=0;i<4;i++){
//            Vlan[i].id = 0;
//            Vlan[i].newID = 0;
//            Vlan[i].change_in_progress = 0;
////            group[i].run7seg=1;
//        }
    




    
    
      TEST_ID();                              // parbauda vai ir klat LCD modulis
    
    
       if (ID==5)
    
    {
    
    
     EN_12V=1;
     
     delay_ms(100);
     
    OLED_Display_init();
    SetBrightness (0x55);
    
     clrscr();
     
     set_ind_mas();            // uzstada indikacijas masivu
     
     
     copy_BMP_to_ram ();
     copy_ram_to_LCD (); 
  //   X_OFFSET
  //   fillrect(0,63-32,32,32,COL_GRAY);
     
   //   fillrect(0,63-32+16,4,4,COL_NORMAL);
      
       delay_ms(2000);
       
     //   clrscr_ram();               // notira ram
   
       
       goto kuku;
       
   //    drawSquare (0, 64-32, 4, COL_NORMAL);
        copy_ram_to_LCD ();
muz_cikl:   
        
/*       
      for (y=0;y<step;y++)   
      {
      drawSquare (wait_coordinates[y][0], wait_coordinates[y][1], 4, COL_NORMAL);
      
     if (y!=0) drawSquare (wait_coordinates[y-1][0], wait_coordinates[y-1][1], 4, COL_INVERSE);
        else  drawSquare (wait_coordinates[step-1][0], wait_coordinates[step-1][1], 4, COL_INVERSE);
      
      delay_ms(50);
      copy_ram_to_LCD (); 
      
      } 
*/
        
     
         for (y=0;y<step;y++)   
      {
      drawSquare (wait_coordinates[y][0], wait_coordinates[y][1], 4, COL_NORMAL);
      
     if (y!=step-1)  drawSquare (wait_coordinates[y+1][0], wait_coordinates[y+1][1], 4, COL_NORMAL);
      
         else  drawSquare (wait_coordinates[0][0], wait_coordinates[0][1], 4, COL_NORMAL);
      
      
      
     if (y!=0) drawSquare (wait_coordinates[y-1][0], wait_coordinates[y-1][1], 4, COL_INVERSE);
        else  drawSquare (wait_coordinates[step-1][0], wait_coordinates[step-1][1], 4, COL_INVERSE);
      
      delay_ms(150);
 
      copy_wait_ram_to_LCD ();
      } 


 
        
//muz_cikl:      
   /*     
       x=0; 
       
      for (y=0;y<32;y+=2){   
        
      drawLine(0+16,63-16-x ,  16+16, 63-y, COL_NORMAL);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
      drawLine(0+16,63-16-x ,  16+16, 63-y, COL_INVERSE);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
       
      }

    //    y=63-30;
        
        
           for (x=0;x<32;x+=2){   
        
      drawLine(0+16,63-16 ,  16+16-x, 63-y, COL_NORMAL);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
      drawLine(0+16,63-16 ,  16+16-x, 63-y, COL_INVERSE);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
       
      }


  
      for (y=32 ;y>0;y-=2){   
        
      drawLine(0+16,63-16 ,  16+16-x, 63-y, COL_NORMAL);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
      drawLine(0+16,63-16 ,  16+16-x, 63-y, COL_INVERSE);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
       
      }


  
      for (x=32 ;x>0;x-=2){   
        
      drawLine(0+16,63-16 ,  16+16-x, 63-y, COL_NORMAL);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
      drawLine(0+16,63-16 ,  16+16-x, 63-y, COL_INVERSE);
      copy_ram_to_LCD (); 
      delay_ms(50);
      
       
      }



*/

 //      drawLine(0+16,63-16 ,  16+16, 63-16-16, COL_INVERSE);
//      copy_ram_to_LCD (); 
 //      delay_ms(2000);
    /*   
       put_pixel_ram (16,63,COL_NORMAL);
       put_pixel_ram (16,62,COL_NORMAL);
       put_pixel_ram (16,61,COL_NORMAL);
       put_pixel_ram (16,60,COL_NORMAL);
       put_pixel_ram (16,59,COL_NORMAL);
       
         copy_ram_to_LCD (); 
       */
       
   //   drawCircle(32, 32, 14, COL_NORMAL);
   //    copy_ram_to_LCD (); 
    //   delay_ms(2000);        
              
         
 //     drawCircle(32, 32, 14, COL_INVERSE);
 //      copy_ram_to_LCD (); 
 //      delay_ms(2000);     
       
    //  delay_ms(6000);
    
      goto muz_cikl;
      
  //   setDisplay (0,0,FONT_12x16,0); 
  //   putstr("buton_ID1234567890"); 
     
   //  setDisplay (0,1,FONT_6x8,0); 
   //  putstr("buton_ID1234567890123456ytre"); 
     
   /*  
        setDisplay (0,2,FONT_6x8,0); 
        putstr("                "); 
        setDisplay (0,3,FONT_6x8,0); 
        putstr("                "); 
        setDisplay (0,4,FONT_6x8,0); 
        putstr("                "); 
        setDisplay (0,5,FONT_6x8,0); 
        putstr("                "); 
        setDisplay (0,6,FONT_6x8,0); 
        putstr("                "); 
        setDisplay (0,7,FONT_6x8,0); 
        putstr("                "); 
    */
  
     
   /*  
     
         setDisplay (0,2,FONT_6x8,0); 
         putstr("buton_ID32345612"); 
         setDisplay (0,3,FONT_6x8,0); 
         putstr("buton_ID32345612"); 
         setDisplay (0,4,FONT_6x8,0); 
         putstr("buton_ID42345612"); 
         setDisplay (0,5,FONT_6x8,0); 
         putstr("buton_ID52345612"); 
         setDisplay (0,6,FONT_6x8,0); 
         putstr("buton_ID62345612"); 
         setDisplay (0,7,FONT_6x8,0); 
         putstr("buton_ID72345612"); 
      
       
       
       
       
       
     
     delay_ms(1000);
     
    */
    }
    
    
  //  rotateDisplay();
  /*  
     setDisplay (2,5,FONT_6x8,0); 
     putstr("URed");
    
      setDisplay (2,6,FONT_6x8,0); 
     putstr("Green");
     
    int bidnis=98;
   // unsigned char braitnes;   
    char rinda[10];
    
    */
    
 /*   
 kuku:   
    
     setDisplay (0,0,FONT_24x32,0); 
     sprintf(rinda,"%3d",bidnis);
     putstr( rinda );
     
     setDisplay (2,5,FONT_6x8,0); 
     putstr("Red");
     
 //    LED_OK^=1;
     
      
     goto kuku;
     
   
  */   
   
  
 //   EN_12V=1;
   // delay_10ms(10);
  //  EN_12V=0;
    
  //  delay_ms(100);
    
 // int i;  
  //   OLED_Display_init();
     
kuku:     

//  for( i=10; i<256; i++)
//	{
//		SetBrightness (55);
//		delay_10ms(1);
//	}

	//displayTitle();
//goto kuku;

     //     if(ms_since_last_idle > 8000 || InitStep == ciLOGED_OUT){
        
    
     //   InitStep = ciLOGED_OUT;      
              
        cisco_init();
        
        
     //   sroll_digit=0;
        
        // start_sroll_title();
        
        
        
            
      _CN31PUE = 1;
      
      _CN32PUE = 1;
       
      _CN12PUE = 1;
    
    if (ID==5){   
              selected_mode[2]=1;     // pagaidam oec reset ieselekte 3 modi;
      
              stop_sroll_title();
              mode=1;
              start_sroll_title();
            
              set_on_wait_state();     // uzliek gaidisanas menu
              }

      //   form_led_mas();  
         
    //  apr_indic_izv_mas();
        
//================== MAIN LOOP ===============
	while(1)
	{

    
    
        
       cisco_io();
       ms1_virtual_timer();
      // ms2_virtual_timer();
     //  ms3_virtual_timer();
        uart_drv_io();
 
        
       
        
 //   if (ID==5)    test_butons(); 
       
       
       if (ID==5)      LCD_menu();
        
        
    
        

//        if(lastErrorCode != ecNONE){
//            if((mS_counter>>8)&1){
//                set_7seg(&group[0].data7seg, SYM_OFF, 0);
//                set_7seg(&group[1].data7seg, SYM_OFF, 0);
//                set_7seg(&group[2].data7seg, SYM_OFF, 0);
//                set_7seg(&group[3].data7seg, SYM_OFF, 0);
//            }else{
//                set_7seg(&group[0].data7seg, SYM_E, 0);
//                set_7seg(&group[1].data7seg, SYM_r, 0);
//                set_7seg(&group[2].data7seg, SYM_r, 0);
//                set_7seg(&group[3].data7seg, lastErrorCode, 0);
//
//            }
//
//        }else{
//
//            for (i=0;i<4;i++)
//		{
//                    if (Vlan[i].unlocked && (mS_counter>>8)&1)
//                    {
//                            set_7seg(&group[i].data7seg, SYM_OFF, 0);
//                    }
//                    else
//                    {
//                            set_7seg(&group[i].data7seg, Vlan[i].id, 0);
//                    }
//		}
//        }
		
	}// END while(1);
}

//==========functions==========


//============================================================================================================================
//============================================================================================================================



void initPorts ( void )
{
	AD1PCFG = 0xFFFF; //All analog pins as digital
	_CN13PUE = 1;

	//           5432 1098 7654 3210
	ODCB   = Hex(0000,0000,0000,0000);
	PORTB  = Hex(1111,1111,1111,1111);
	TRISB  = Hex(1111,1111,1011,1111);

	ODCC   = Hex(0000,0000,0000,0000);
	PORTC  = Hex(1111,1111,1111,1111);
	TRISC  = Hex(1111,1111,1111,1111);

	ODCD   = Hex(0000,0000,0000,0000);
	PORTD  = Hex(1111,0011,1111,1111);
	TRISD  = Hex(1111,0011,1111,1111);

	ODCE   = Hex(0000,0000,0000,0000); 
	PORTE  = Hex(1111,1111,1111,1111);
	TRISE  = Hex(1111,1111,1111,1111);

	ODCF   = Hex(0000,0000,0000,0000);
	PORTF  = Hex(1111,1111,1111,1111);
	TRISF  = Hex(1111,1111,1111,0011);

	ODCG   = Hex(0000,0000,0000,0000); 
//	PORTG  = Hex(1111,1111,1111,1111);
//	TRISG  = Hex(1111,1111,1111,1111);
		
    
    PORTG  = Hex(1111,1111,1011,1111);
	TRISG  = Hex(1111,1111,1011,1111); 
}

void initPPS ( void )
{
 
	PPSUnLock;

	//RS232
	PPSInput  ( PPS_U1RX , PPS_RP22 );
	PPSOutput ( PPS_RP25 , PPS_U1TX );

    
    //SPI1 - SCT LED
	PPSOutput ( PPS_RP17 , PPS_SDO1 );    //MOSI
	PPSOutput ( PPS_RP10 , PPS_SCK1OUT ); //SCK

    
    // SPI2 - LCD
    
    PPSOutput ( PPS_RP2 , PPS_SDO2 );    //MOSI
	PPSOutput ( PPS_RP4 , PPS_SCK2OUT ); //SCK
    
    
  /*  
   
    //RS232
	
    _U1RXR=_RP22R;
    _RP25R=_RPOUT_U1TX;
    
    
    _RP17R=_RPOUT_SDO1;
    _RP10R=_RPOUT_SCK1OUT;
            

 // mm  
      _RP2R=_RPOUT_SDO2;
     _RP4R=_RPOUT_SCK2OUT;
    
 */   
 


}

void initUART (void)
{
	//RS232
	U1STA  = 0;
	U1BRG  = (MIPS*1000000L/RS232_BAUD_RATE)/4-1;
	U1MODEbits.BRGH = 1; //high baud = Fcy/4
	U1MODEbits.STSEL = 0; //one stop bit
	U1MODEbits.PDSEL = 0; //8bit data, no parity
	U1MODEbits.RXINV = 0; // RS232 is inverse
	U1STAbits.UTXINV = 0; //
	U1MODEbits.UARTEN = 1;
	U1STAbits.UTXEN = 1;
	_U1RXIF = 0;
	_U1RXIE = 1;

}
void initTimers ( void )
{
	//----- DMX TX timer -----
	TMR2 = 0; //reset counter
	T2CON = 0x8000; //prescale 1:1
	PR2 = MIPS * 2000; // first interrupt after 2ms
	IPC1bits.T2IP = 2;
	
	//----- 1mS clock timer -----
	TMR3 = 0; //reset counter
	T3CON = 0x8000; //prescale 1:1
	PR3 = MIPS * 1000; // interrupt after 1ms
	IPC2bits.T3IP = 4; //interrupt priority
    
    
    
    //----- 1mS clock timer -----
	TMR4 = 0; //reset counter
	T4CON = 0x8000; //prescale 1:1
	PR4 = MIPS * 2000; // interrupt after 2ms
	IPC6bits.T4IP = 1; //interrupt priority
	
	

}

void initSPI ( void )
{
	//----- SPI1 ----- SCT for LEDs
		SPI1CON1bits.MODE16 = 1; //16 bit mode
		SPI1CON1bits.SMP = 0; //sampling at middle of data
		SPI1CON1bits.CKE = 1; //rising edge data
		SPI1CON1bits.SSEN = 0; //SS disable
		SPI1CON1bits.CKP = 0; //idle = low
		SPI1CON1bits.MSTEN = 1; //master mode	
		SPI1CON1bits.PPRE = 3; //primary prescale 1:1
		SPI1CON1bits.SPRE = 6; //secondary prescale 1:2
		
        
        
		SPI1CON2bits.SPIBEN = 1; //enhanced buffer mode
		//SPI1CON2bits.FRMEN = 1; //frame mode
		//SPI1CON2bits.SPIFSD = 1; //frame slave

		IPC2bits.SPI1IP = 3; //interrupt priority

		SPI1STAT = 0; // clear flags
		SPI1STAT = Hex(1000,0000,0001,0100);; // enable; interrupt when transmit complete
        
  
        
        // LCD SPI2
        
        SPI2STAT = 0;
  
        SPI2CON1 = Hex(0000,0010,0111,1111); // 16 Mbit, 8 bit
        SPI2CON2 = Hex(0000,0000,0000,0001);
        SPI2STAT = Hex(1000,0000,0000,0000);   
   
   
}

unsigned char ekrana_kopija[9][132];


/*
void test_butons(void)
{

    
    
      _CN31PUE = 1;
      
      _CN32PUE = 1;
       
      _CN12PUE = 1;
    
   // return;   
   
      
if (!Buton1)    
   
    
     {
  //   setDisplay (2,2,FONT_6x8,0); 
  //   putstr("buton1");
  //   LED_OK=1;
   
  start_sroll_title(); 
 //  Scroll_H_16_title();
    
    //  Tek_kom(0);
    
    
    
    
    
    
   
   //   setDisplay (0,0,FONT_12x16,0);    // clear  row
   //  putstr("123456789");
   
 //  Scroll_H_16_title();
   
   
      }
 else

      {
 //   setDisplay (2,2,FONT_6x8,0); 
 //  putstr("XXXXXX"); 
 //  putstr("      ");
       
    //   LED_OK=0;
      
      }
   // return;   

if (!Buton2)    
   
    
     {

  int i,j;
	for(i = 0; i < 8; i++)
	{
		SetAddress(i, 0);
		for(j = 0; j < X_SIZE; j++)
        {   // Write to all pixels
            
         //   delay_ms(100);
      //   if (i==0)   ekrana_kopija[i][j]=0x55; //TODO - set to 0x00
      //   else ekrana_kopija[i][j]=0xFF; 
            
         ekrana_kopija[i][j]=0x0;    
            
        }
    }

 //   setDisplay (0,0,FONT_6x8,0); 
 
  //   for (i = 0; i <= 40; i++) {
 
 //    clrscr();
 //    SetXY(0,i);  
  //    Tek_kom(7);
     
   // putstr("Done");
   //   setDisplay (0,i,FONT_6x8,0); 
     
   delay_ms(100);
     
    // }
 
    
      }
 else

      {
 //    setDisplay (2,3,FONT_6x8,0); 
   //  putstr("buton1"); 
 //      putstr("      ");
      }

    
    return;
    
//if (!Buton_ID)    
    
   if ( ADC1BUFF < 150 )   
   
    
     {
  //   setDisplay (0,4,FONT_6x8,1); 
   //  putstr("buton_ID12345612"); 
     
   //  Scroll_H_16_title();
     
   //  delay_ms(2000);
       
       
     //  start_sroll_title();
      }
      

     
     
 
     
  //      setDisplay (1,7,FONT_6x8,0); 
  //   sprintf(rinda,"ID = %d",ID);
  //   putstr( rinda );
     
  
// delay_ms(10);   
 

      
}    
*/
void initHard ( void )
{
	initPorts();
	initPPS();
    initSPI();
	initUART();
        //InitializeUSART(); // cisco rs232
	initTimers();
}

void ISR_enable ( void )
{
	//----- TIMER2 - DMX TX
	//IFS0bits.T2IF = 0; //reset timer2 interrupt flag
	//IEC0bits.T2IE = 1; //enable interrupt

	//----- TIMER3 - 1mS clock
	IFS0bits.T3IF = 0; //reset timer3 interrupt flag
	IEC0bits.T3IE = 1; //enable interrupt

    //----- SPI - SCT LED driver
	IFS0bits.SPI1IF = 0; //reset timer interrupt flag
	IEC0bits.SPI1IE = 1; //enable interrupt
    
    
    //----- TIMER4 - 1mS clock
	IFS1bits.T4IF = 0; //reset timer3 interrupt flag
	IEC1bits.T4IE = 1; //enable interrupt
	
}
/*
void ms1_virtual_timer(void){

    static unsigned int mS_counter_before;
    static unsigned int blink_state;
    static unsigned int activity_blink_state;
    unsigned int i;
    unsigned long ActivityBit;
    static unsigned long StatusBit = (1ul<<5); // Status Led loacation
    unsigned long LinkBit;
    unsigned long AnimationBits;
    static unsigned int boot_animation_step = 0;


    while(mS_counter_before != mS_counter){
        mS_counter_before++;
                                                // mns iespraudums if else
     if ((enable_wait==0)&&( selected_mode[mode]==0))                  // ja nav vait rezima un nestav uz izveletas modes
     {
        if(mS_counter_before % 100 == 0)
            {
            
            led_ind_idx++;
            if (led_ind_idx>9) led_ind_idx=0;
            Leds.Val=led_ind_mas[led_ind_idx];
            
            SPI1BUF = Leds.w[0];
            SPI1BUF = Leds.w[1];
        
            }
     }   else  
       //mans if(0){
        if(mS_counter_before % 50 == 0){
            activity_blink_state ^=1; // blink

           
            for(i=0;i<SW_PORT_COUNT;i++){
                LinkBit = (2ul<<(i*3));
                ActivityBit = (1ul<<(i*3));
                
                if(i>4){ // add +1 offset for second SCT chip
                    LinkBit <<= 1;
                    ActivityBit <<= 1;
                }
                
                if(rs232_link_is_up() != 0){ // link to switch is active
                    
                    Leds.Val |= StatusBit;
                    // set port activity leds
                    if(port_link_status[i]){// if link is up
                        Leds.Val |= LinkBit; // set bit
                        if(activity_led_counter[i] != 0){ //activity timer
                            activity_led_counter[i]--;

                            if(activity_blink_state != 0){
                                Leds.Val |= ActivityBit; // set bit
                            }else{
                                Leds.Val &= ~ActivityBit; // clear bit
                            }
                             
                        }else{ // link is idle state
                            Leds.Val |= ActivityBit; // set bit
                            
                        }
                    }else{ //link is down
                        Leds.Val &= ~LinkBit; // clear bit
                        Leds.Val &= ~ActivityBit; // clear bit
                    }
                }else{
                    Leds.Val &= ~LinkBit; // clear bit
                    Leds.Val &= ~ActivityBit; // clear bit
                    
                    
                    AnimationBits = (3ul<<(boot_animation_step*3));
                    
                
                    if(boot_animation_step > 4){ // add +1 offset for second SCT chip
                        AnimationBits <<= 1;
                    }
                    
                    //if(mS_counter_before % 150 == 0){
                        boot_animation_step++;
                        if(boot_animation_step > SW_PORT_COUNT)boot_animation_step = 0;
                   // }
                        
                        if(blink_state != 0){
                            AnimationBits |= StatusBit;
                        }else{
                            AnimationBits &= ~StatusBit;
                        }

                    Leds.Val = AnimationBits;

                }

            }
            
            
            // move LED data to SCT chips
            SPI1BUF = Leds.w[0];
            SPI1BUF = Leds.w[1];
        }

       

        // set status led state
        if(mS_counter_before % 100 == 0){
            blink_state ^=1; // blink

            if(rs232_link_is_up()){
                
                LED_OK = 1; // turn on
            }else{
                LED_OK = blink_state; // blink
                
                if (enable_wait==0) set_on_wait_state ();    // uzliek ekranu gaidisanas rezimu ja tads vel nav

            }
        }
  
        
    }
}
*/

/*

void ms2_virtual_timer(void){

    static unsigned int mS_counter_before;
    static unsigned int blink_state;
    static unsigned int activity_blink_state;
    unsigned int i;
    unsigned long ActivityBit;
    static unsigned long StatusBit = (1ul<<5); // Status Led loacation
    unsigned long LinkBit;
    unsigned long AnimationBits;
    static unsigned int boot_animation_step = 0;

    unsigned int j;
extern unsigned long int  LED_indic[29];    
    
    while(mS_counter_before != mS_counter){
        mS_counter_before++;
                                                // mns iespraudums if else
    // if ((enable_wait==0)&&( selected_mode[mode]==0))                  // ja nav vait rezima un nestav uz izveletas modes
     {
        if(mS_counter_before % 1000 == 0)
            {
            
            
            
            
           apr_indic_izv_mas();

             izvada_LED();
               
             
           if  (activity_blink_state==0) LinkBit=0x0000;
            if  (activity_blink_state==1) LinkBit=0xFF0000;
              if  (activity_blink_state==2) LinkBit=0x00FF00;
              if  (activity_blink_state==3) LinkBit=0x0000FF;
             
             
              
              for (j=0;j<29;j++)   LED_indic[j]=LinkBit;  
             
             activity_blink_state++;
             
              if  (activity_blink_state>3) activity_blink_state=0;
        
             
             
            }
     }  
    
  
        
    }
}
*/
///////////////////////////////======
void ms1_virtual_timer(void){

    static unsigned int mS_counter_before;
    static unsigned int blink_state;
    static unsigned int activity_blink_state;
    unsigned int i;
    unsigned long ActivityBit;
    static unsigned long StatusBit = (1UL<<28); // Status Led loacation
    unsigned long LinkBit;
 //   unsigned long AnimationBits;
 //   unsigned long Out_LinkBit=0;
    unsigned long Out_ActivityBit=0;
  //  unsigned long Out_AnimationBits;
 //   static unsigned int boot_animation_step = 0;
    extern unsigned long int  LED_indic[LEDS]; 

    static unsigned long maska;

    while(mS_counter_before != mS_counter){
        mS_counter_before++;
                                               // mns iespraudums if else
     if ((ID==5)&&(enable_wait==0)&&( selected_mode[mode]==0))                  // ja nav vait rezima un nestav uz izveletas modes
     {
        if(mS_counter_before % 100 == 0)
            {
          // te viss jalabo  
        //    led_ind_idx++;
        //    if (led_ind_idx>9) led_ind_idx=0;
        //    Leds.Val=led_ind_mas[led_ind_idx];
        // japarbuve     
        //    SPI1BUF = Leds.w[0];
        //    SPI1BUF = Leds.w[1];
            Nop();
            Nop();
            }
     }   else  
       //mans if(0){
       
         
        if(mS_counter_before % 50 == 0){
            activity_blink_state ^=1; // blink

           
            for(i=0;i<SW_PORT_COUNT;i++){
                LinkBit = (1UL<<(i));
                ActivityBit = (1UL<<(i));
                
           //     if(i>4){ // add +1 offset for second SCT chip
           //         LinkBit <<= 1;
           //         ActivityBit <<= 1;
           //     }
                
                if(rs232_link_is_up() != 0){ // link to switch is active
                    
  //mans pagaidam nonemts              Leds.Val |= StatusBit;
                    
                      Out_LinkBit  |= StatusBit;    // iesledz zalo
                      Out_ActivityBit &= ~StatusBit;  // nodzes sarkano
                    // set port activity leds
                    if(port_link_status[i]){// if link is up
                      //  Leds.Val |= LinkBit; // set bit
                        Out_LinkBit|= LinkBit; // set bit    
                        if(activity_led_counter[i] != 0){ //activity timer
                            activity_led_counter[i]--;

                            if(activity_blink_state != 0){
                               // Leds.Val |= ActivityBit; // set bit
                                Out_ActivityBit|= ActivityBit; // set bit  //??????
                            }else{
                              //  Leds.Val &= ~ActivityBit; // clear bit
                                Out_ActivityBit&= ~ActivityBit; // clear bit
                            }
                             
                        }else{ // link is idle state
                           // Leds.Val |= ActivityBit; // set bit
                            Out_ActivityBit|= ActivityBit; // set bit     //??????
                        }
                    }else{ //link is down
                       // Leds.Val &= ~LinkBit; // clear bit
                      //  Leds.Val &= ~ActivityBit; // clear bit
                        Out_LinkBit&= ~LinkBit; // clear bit
                        Out_ActivityBit&= ~ActivityBit; // clear bit
                        
                        
                    }
                }else{
                   // Leds.Val &= ~LinkBit; // clear bit
                   // Leds.Val &= ~ActivityBit; // clear bit
                    Out_LinkBit&= ~LinkBit; // clear bit
                    Out_ActivityBit&= ~ActivityBit; // clear bit
                   
                     
                 /*   
                    AnimationBits = (3ul<<(boot_animation_step*3));
                    
                
                    if(boot_animation_step > 4){ // add +1 offset for second SCT chip
                        AnimationBits <<= 1;
                    }
                    
                    //if(mS_counter_before % 150 == 0){
                        boot_animation_step++;
                        if(boot_animation_step > SW_PORT_COUNT)boot_animation_step = 0;
                   // }
                        
                        if(blink_state != 0){
                            AnimationBits |= StatusBit;
                        }else{
                            AnimationBits &= ~StatusBit;
                        }

                    Leds.Val = AnimationBits;
              */
                  
                           Out_LinkBit &= ~StatusBit;                   // nodzes zalo
                           
                           if(blink_state != 0){
                            Out_ActivityBit |= StatusBit;               // iesledz sarkno
                        }else{
                            Out_ActivityBit &= ~StatusBit;              // izsledz sarkno
                        }   
                    
                    
                }

            }
            
            
            // move LED data to SCT chips
           // SPI1BUF = Leds.w[0];
           // SPI1BUF = Leds.w[1];
            
              maska=1;
           for(i=0;i<SW_PORT_COUNT+1 ;i++)    // aiz 28 porta seko Statusa ports
   
                 
             {
            
              LED_indic[i]=0;
            if (i==SW_PORT_COUNT)
            {   
               if (maska&Out_ActivityBit) LED_indic[i]=0x3F0000;          // uzliek sarkano
               if (maska&Out_LinkBit)    LED_indic[i]=0x003F00;          // uzliek zalo 
               break; 
            }  
            if ((activity_blink_state)&&(maska&Out_ActivityBit))
               {    
             //  if (maska&Out_ActivityBit) LED_indic[i]|=0x7F0000;          // uzliek sarkano
             //  if (maska&Out_LinkBit)    LED_indic[i]|=0x007F00;          // uzliek zalo 
                
                
                if (maska&Out_LinkBit)   
                  {
                    
                  if  (port_link_speed[i]==1000)  LED_indic[i]=0x003f00; // uzliek zalo 
                  if  (port_link_speed[i]<1000)   LED_indic[i]=0x7F2600; // uzliek oranzu
                  if  (Poe_Bit&maska)  LED_indic[i]=0x00003f;            // uzliek zilo
                  
                  
                  }   
               }
            
                maska<<=1;   
             }
         //    maska<<=1;  
           // izliek status bitu  
      //       LED_indic[SW_PORT_COUNT+1]=0; 
      //       if (maska&Out_ActivityBit) LED_indic[SW_PORT_COUNT+1]|=0x7f0000;          // uzliek sarkano
      //       if (maska&Out_LinkBit)     LED_indic[SW_PORT_COUNT+1]|=0x007f00;          // uzliek zalo  
              
              
       // trenesana 
       //       if  (ID!=5) LED_indic[28]=0x0000FF;  
              
              
             apr_indic_izv_mas();
          izvada_LED();    
          
        if (Out_LinkBit!=Out_LinkBit_old)  flag_poe_status=1;
          
         Out_LinkBit_old=Out_LinkBit;
          
        //  Poe_Bit=2;
            
        }

       

        // set status led state
        if(mS_counter_before % 100 == 0){
            blink_state ^=1; // blink

            if(rs232_link_is_up()){
                
                LED_OK = 1; // turn on
            }else{
                LED_OK = blink_state; // blink
                
                if  ((ID==5)&&(enable_wait==0)) set_on_wait_state ();    // uzliek ekranu gaidisanas rezimu ja tads vel nav

            }
        }
  
        
    }
}


// EOF