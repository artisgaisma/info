#ifdef FFT_FIRST_REV_C
/*2020.10.02*/
#define STAB  		  14
#define WING  		  15
#define SW_1  		  16
#define SW_2  		  17
#define SW2 		    9
#define BUZ  		    13
#define LED_STATUS  	8
#define SERVO_ON  		5
//PINMAP FFT_FIRST_REV_C
//D3-LED status			OK!
//D5-servo 4
//D6-servo 3
//D7-BUZ-no gen			OK!
//D9-servo 1
//D10-servo ON
//D11-servo 2
//D12-hall 1
//D13-hall 2
//A6-BUTTON-ON OF ..ERRROR - analog pin, not digital
//A7-POW ON       ..ERRROR - analog pin, not digital
//A0-voltmeter
//D15 - TX2-GPS
//D16 - RX2-GPS
//D17 - DT in 
//D18 - to BLE wake UP
//D19 - to BLE Config
#endif

#ifdef MODEL_A
/*2020-10-01*/
#define STAB  		  14
#define WING  		  15
#define SW_1  		  16
#define SW_2  		  17
#define SW2 		    9
#define BUZ  		    13
#define LED_STATUS  8
#define SERVO_ON  	5
#endif

#ifdef GPS_RF_C
/*2020.10.02 - added GPS_RF_C*/
#define BOARD			"GPS_REV_C"
#define STAB			9		
#define WING			-1		//not used
#define SW_1			-1		//not used
#define SW_2			-1		//not used
#define BUTON			6		//push button on board
#define BUZ				-1		//not used
#define LED				4		//LED on board
#define SERVO_ON 		-1		//not used
#define INT_GYRO		17		//accelerometer INT pin
#define RF_NSS			10		//RFM9x NSS PIN
#define DATA_RF			15		//RFM9x data 1
#define DATA_RF_2		16		//RFM9x data 2
#define RF_FM			-1		//send FM tone broadcast, should be DIO2 on RFM9x??
#define	MEM_CS			14		//Flash memory chip select - used for altimeter data
#define GPS_RX			3
#define GPS_TX			2
//#define BMP280_ADRESS 0x76	//addr to gnd	
//#define BMP180_ADRESS	0x77	//has no addres pin
//#define GYRO_ADRESS
#endif