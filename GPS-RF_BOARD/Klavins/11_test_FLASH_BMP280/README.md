DEVELOPMENT NOTES:
--- Nepilnības:--------------------------------------------------------------------------
* sesiju skaits ir ierobežots (254), jo '255' tiek izmantots kā indikātors, kad EEOPROM ir tukšs (jauns MCU)
* EEPROM tiek nonullēts pie vertības 255, bet FLASH overflow ir piefiksēts, bet nav atrsisināts
--- Idejas: -----------------------------------------------------------------------------------------
* visas error un status Serial ziņas glabāt harcoded iekš FLASH sektora (ieguvums kādi 50-150 bytes)
  (atvelēt atsevišķu sektoru, kuru saprogrammēt izstrādes laikā, un izveidot par read-only)
* FLASH datus iedalīt zonās - altimetrs, GPS, u.t.t.t (pielagotas funkcijas katrai zonai)
* salikt EEPROM.update kur vajag


FEATURES:
--- Altimeter ---------------------------------------------------------------------------------------
* lasa altimetru ik pēc lietotāja izvēlēta laika intervāla (bez aiztures, neieskaita kļūdas)
* iestata relatīvo nulles līmeni (nolasa 'n' reizes un izvelk vidējo, neieskaita kļūdas)
--- Atmiņa ------------------------------------------------------------------------------------------
* ieraksta datus pēc lietotāja izvēlēta laika intervāla un minimālā augstuma (bez aiztures)
* ieraksta brīdī izvelk vidējo no pēdejiem 'n' augstuma lasījumiem
* izmanto EEPROM priekš ātri updeitojamiem datu parametriem
* izdzēš no FLASH tikai ierakstītos laukus (patērē max maz laika)
* no atmiņās var nolasīt konkrētu sesiju, ar konkrētu sākuma laiku


FUNCTION MAP (precedence from left to right):
-----------------------------------------------------------------------------------------------------
begin_ALTIMETER()	read_ALTIMETER( READ_step )
			altimeter_REC( REC_step, REC_threshold )
begin_FLASH()	memoryDUMP()
		erase_FLASH()
		read_FLASH( sID, start_time )
update_STATUS( Status )
			

=====================================================================================================
   Used libraries, datasheets etc:
      https://github.com/Marzogh/SPIMemory 
      https://github.com/mahfuz195/BMP280-arduino-library ( memory usage - 26%/25% with test code)
      https://github.com/adafruit/Adafruit_BMP280_Library ( memory usage - 39%/25% with test code)
      https://github.com/sparkfun/BMP180_Breakout_Arduino_Library ( memory usage - 26%/27% with test code)
      https://github.com/mikalhart/TinyGPSPlus ( memory usage - 24%/41% with BasicExample code)
=====================================================================================================
   Change Log:
   04.12.2020 - added EEPROM for ID and packet count tracking, because 
                in FLASH memory it would take 4KB and data update would take 50ms
   05.12.2020 - added altimeter
   06.12.2020 - added base level for height, attempt to stabilise altimeter readouts
   08.12.2020 - cacthed memory overflows and reduced erase time
		improved code (.h/.cpp files) and altimeter reading
   10.12.2020 - new memory read functions
   15.12.2020 - session bug fix
=====================================================================================================
   To-Do:
      # does failed altimeter read get's discarded?
   NB:
      ! erasing whole FLASH memory will take at least 20 seconds
      ! every FLASH memory cell can only be written once, for updating cell please erase it first
      ! its not possible to erase only single FLASH memory cell, only whole sector
      ! for faster FLASH memory writing - flash.writeByte(ID_ADD, sessionID, NOERRCHK);