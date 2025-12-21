# HJ-185 BLE Module

### TO DO
-[ ]is it possible to set/acces pasw from MCU??

### Comunication chanels
```
Services	BLE [FFF0]
App 	<- 	BLE [FFF1] <-	RX port
App 	-> 	BLE [FFF2]  ->	TX port
App 	<->	BLE [FFF3]	for internal commands 
```
### Serial Bluetooth Terminal app Custom settings
```
1-Service UUID - only one will be shown
	FFF0
2-Read characteristics (read from BLE)
	FFF1 - Trasnparent 
	FFF3 - App <- BLE
3-Write charasteristics (write to BLE)
	FFF2 - Trasnparent
	FFF3 - App -> BLE

#Config Mode - APP <-> BLE
	FFF0
	FFF3
	FFF3

#Transparent Mode - APP <-> BLE <-> MCU
	FFF0
	FFF1
	FFF2
			
- #RECONNECT BLE afters setting change!!!!!!!!!!!
```
### PIN Settings
```
PIN14/P0.16 - HIGH - Config mode Enable.HIGH-Enable, LOW-Disable
PIN9/P0.02 - Ble Serial RX Enable. LOW-enable, HIGH-Disable
VCC - 1.7-3.6V
```

### Config Mode Commands
```
<RD_NAME> <ST_NAME=xxx>
<RD_ADV_ONOFF> <ST_ADV_ONOFF=X> X=1:BroadCast ON, X=0 BroadCast Off
<RD_ADV_DATA> <ST_ADV_DATA=xx..xx> Set/Read Data about Broadcasting-HEX
<RD_ADV_GAP> <ST_ADV_GAP=xx..xx> Broadcast gab 20-10`000ms
<RD_CLIENT_LINK> <ST_CLIENT_LINK=0> Disconnect from Host. Reads MAC
<RD_CON_MIN_GAP> <ST_CON_MIN_GAP=xx..xx> Min connection gab 7.5-4000ms
<RD_CON_MAX_GAP> <ST_CON_MAX_GAP=yy..yy> Max connection Gap 7.5-4000ms
<RD_CON_TIMEOUT> <ST_CON_TIMEOUT=xx..xx> Timeout of slave connection 500-8000ms
<RD_SECRET> <ST_SECRET=xx..xx> <ST_CLEAR_SECRET=1> Pasw Up to 8 chars
<RD_SECRET_TIMEOUT> <ST_SECRET_TIMEOUT=xx..xx> Pasword timeout 1-255s
<RD_SOFT_VERSION>
<RD_TX_POWER> <ST_TX_POWER=+4> -40/-20/-16/-12/-8/-4/0+3/+4dBm
<ST_RESET_BLE> Resest BLE after 500ms
<RD_BAUD> <ST_BAUD=xx..xx> 1200/2400/4800/9600/19200/38400/57600/115200/230400/460800/921600 bps
<RD_BLE_MAC>
<ST_OWN_MAC=xxxxxxxxxxxx> Set User-Defined MAC address
<ST_OWN_MAC=0> Cancel User-Defined MAC address
<RD_T1> <ST_T1=x> 1-255ms T1 value – App receiving indicator PIN10
<RD_UART_EVEN> <ST_UART_EVEN=x> 1-enables even parity. 0-disabled
<RD_AD_WAIT> <ST_AD_WAIT=x> Set/Red PIN10 puldown delay 1-255ms
<RD_UART_PULL> <ST_UART_PULL=x> BLE RX pullup – 1-Enable, 0-Disable Internal Pull-up - PIN5
<ST_FACTORY=1> RESTORE FACTORY SETTINGS
<RD_WAKEUP_HIGH> <ST_WAKEUP_HIGH=x> 1-Active HIGH 0-Active LOW(default) PIN9
```

# Password
```
<RD_SECRET> <ST_SECRET=xx..xx> <ST_CLEAR_SECRET=1> Pasw Up to 8 chars
<RD_SECRET_TIMEOUT> <ST_SECRET_TIMEOUT=xx..xx> Pasword timeout 1-255s
```
### Option 1
```
-	app sends IMEI first `8` chars to [FFF3]
-	if SET
	-	[Access] or [TimeOut]
-	if NOT SET
	-	App send later "<ST_SECRET=IMEI>" to [FFF3]
```
### Option 2 - similar to Opt1
```
-	app sends to [FFF3]
	-	IMEI
	-	<ST_SECRET=IMEI>
-	if SET 
	-	[Access] or [TimeOut]
-	if NOT -> will write in 2nd step
```

### Option 3 - stulbs variants
```
-	app sends IMEI first `8` chars to [FFF2]
-	if Pasw is set
	-	[Access] or [TimeOut]
-	if NOT SET
	-	MCU sends this to back to BLE <ST_SECRET=IMEI>
-	app sends IMEI first `8` chars to [FFF3]
```
### Option 4 - if setable from MCU
```
-	app sends IMEI first `8` chars to FFF2	{IMEI:"IMEI"}
-	BLE sends this to MCU
	-	if no pasw allready set -> MCU sends this back to BLE with added "<ST_SECRET=IMEI>"
	-	if Pasw is set, check is if equla
		-	==	-> Connect
		-	!=	-> TimeOut
```

### Commands from MCU
```
void forward() { 
  if (bleSerial && Serial) {
    int byte;
    while ((byte = bleSerial.read()) > 0) Serial.write((char)byte);
    while ((byte = Serial.read()) > 0) bleSerial.write((char)byte);
  }
}
```

-	void ble_serial_begin(){digitalWrite(BLE_SER_RX_ENABLE, LOW);}		//works
-	void ble_serial_end(){digitalWrite(BLE_SER_RX_ENABLE, HIGH);}		//works
-	void ble_app_config_enable(){digitalWrite(BLE_APP_CONFIG, HIGH);}	//works
-	void ble_app_config_off(){digitalWrite(BLE_APP_CONFIG, LOW);}		//works
-	void set_name(){bleSerial.write("<ST_NAME=FFT-KON2>");}			//works
-	void set_password_def(){bleSerial.write("<ST_SECRET=1234>");}		//up to 8 chars
-	void clr_def_password(){ bleSerial.write("<ST_CLEAR_SECRET=1>");}	//works
-	void set_broadcast_on(){ bleSerial.write("<ST_ADV_ONOFF=1>");}
-	void set_broadcast_off(){bleSerial.write("<ST_ADV_ONOFF=0>");}

/*untested*/
void set_new_baud(){
	Serial.begin(115200);
	Serial.print("<ST_BAUD=9600>");
	Serial.end();
}

