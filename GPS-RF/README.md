# TEST-GPS_RF
### Possible Products:
```
- FF-GPS    GPS locator. coordinates sent by RF module
- FF-DTK    Radio Determalize Key and GPS Coordinate receiver
- FF-Alti   Altimeter Logger
- FF-Angle  Angelmeter, low weight, BLE, Battery, charger
- FF-Gyro   Accelerometer Sensor for Model positionig, angle, speed.
- FF-B-Box  Altitudes and Accelerometer Logger for full review.
```
### RF
- SX1262  - 150-960MHz
- SX1276
- SX1278  - 137-525MHz

###### Martins-write your comments in your folder readmi file

### BOARD:
`TEST-GPS_RF-REV-C` - red pcb
- GPS module
- BMP180 module
- BMP280 module
- MPU6050 - use I2C for module
- RF98 - rf module on board



### TODO:
- [x] Read BMP280 with different times per second
- [x] Write data in Flashm memmory with different times per second
- [ ] - Read BMP280 -> write in eeprom
- [ ] - creat Starr_write_eeprom(HZ, TIME)
- `HZ` - writes per second. 1-10hz, or even 0.1-10hz
- `TIME` - record time. like 1-10 secs. maybe could be up to 1 min, if HZ is 0.1(write one time per 10 seconds)


### Comands

```
<RD_NAME> 			<ST_NAME=xxx>
<RD_ADV_ONOFF> 		<ST_ADV_ONOFF=X> X=1:BroadCast ON, X=0 BroadCast Off
<RD_ADV_DATA> 		<ST_ADV_DATA=xx..xx> Set/Read Data about Broadcasting-HEX
<RD_ADV_GAP> 		<ST_ADV_GAP=xx..xx> Broadcast gab 20-10`000ms
<RD_CLIENT_LINK>	<ST_CLIENT_LINK=0> Disconnect from Host. Reads MAC
<RD_CON_MIN_GAP> 	<ST_CON_MIN_GAP=xx..xx> Min connection gab 7.5-4000ms
<RD_CON_MAX_GAP> 	<ST_CON_MAX_GAP=yy..yy> Max connection Gap 7.5-4000ms
<RD_CON_TIMEOUT> 	<ST_CON_TIMEOUT=xx..xx> Timeout of slave connection 500-8000ms
<RD_SECRET> 		<ST_SECRET=xx..xx> <ST_CLEAR_SECRET=1> Pasword. Up to 8 chars
<RD_SECRET_TIMEOUT> <ST_SECRET_TIMEOUT=xx..xx> Pasword timeout 1-255s
<RD_SOFT_VERSION>
<RD_TX_POWER> 		<ST_TX_POWER=+4> -40/-20/-16/-12/-8/-4/0+3/+4dBm
<ST_RESET_BLE> 		Resest BLE after 500ms
<RD_BAUD> 			<ST_BAUD=xx..xx> 1200/2400/4800/9600/19200/38400/57600/115200/230400/460800/921600 bps
<RD_BLE_MAC>
<ST_OWN_MAC=xxxxxxxxxxxx> Set User-Defined MAC address
<ST_OWN_MAC=0> 		Cancel User-Defined MAC address
<RD_T1> <ST_T1=x> 	1-255ms T1 value – App receiving indicator PIN10
<RD_UART_EVEN> 		<ST_UART_EVEN=x> 1-enables even parity. 0-disabled
<RD_AD_WAIT> 		<ST_AD_WAIT=x> Set/Red PIN10 puldown delay 1-255ms
<RD_UART_PULL> 		<ST_UART_PULL=x> BLE RX pullup – 1-Enable, 0-Disable Internal Pull-up - PIN5
<ST_FACTORY=1> 		RESTORE FACTORY SETTINGS
<RD_WAKEUP_HIGH> 	<ST_WAKEUP_HIGH=x> 1-Active HIGH 0-Active LOW(default) PIN9
```
