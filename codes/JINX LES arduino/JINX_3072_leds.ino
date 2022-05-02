
#include <Artnet.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN            D1  //pin de salida D1
#define NUMPIXELS     2304  ///1536 //425  //numero de pixeles a controlar
#include <SPI.h>
#include <SD.h>



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



const char* ssid     = "xxxxxxx";  
const char* password = "xxxxxxx"; 
uint8_t buf[50000]; //5000

IPAddress ip(192, 168, 1, 228  );
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

File datafile;
char fileName[] = "data.jnr";
const int chipSelect = D8;
bool record = 0;
bool playback = 0;

unsigned int localPort = 65506;      // local port to listen for UDP packets  65506

const int PACKET_SIZE = 6918 ;  //2311; //2306   1357

byte packetBuffer[PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
 int i; 

 
typedef struct
{
      byte r;
      byte g;
      byte b;
} colorpin;

colorpin led;
int led_index = 0;

void WiFiEvent(WiFiEvent_t event)
{
      switch (event)
      {
            case WIFI_EVENT_STAMODE_GOT_IP:
                  pixels.begin();
                  break;
            case WIFI_EVENT_STAMODE_DISCONNECTED:
                  break;
      }
}

void setup()
{
      Serial.begin(115200);
      WiFi.disconnect(true);
      delay(1000);
      WiFi.onEvent(WiFiEvent);
      WiFi.config(ip, gateway, subnet);
      WiFi.begin(ssid, password);
  // WiFi.mode(WIFI_STA);
  //WiFi.begin(STASSID, STAPSK);

      while (WiFi.status() != WL_CONNECTED)
      {
            delay(500);
            Serial.print("x");
      }

       Serial.println(" ");
       Serial.println(" ");
       
       Serial.println("WiFi connected");
       Serial.println("IP address: ");
       Serial.println(WiFi.localIP());

       Serial.println("Starting UDP");
       udp.begin(localPort);
       Serial.print("Local port: ");
       Serial.println(udp.localPort());
nuevo:
         if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    
  }
  else
    Serial.println("initialization done.");

    delay(200);
    goto nuevo;

      initTest();

      // datafile = SD.open("data.txt");  // read only
     //  datafile = SD.open("data.jnr", FILE_READ);
       
      if (datafile) {
      Serial.println("file open ok");
      datafile.close();
        }

   //     Serial.println("Open a new file called data.txt' and write 1 to 5 to it");
    //    File datafile = SD.open("data.txt", FILE_WRITE); // FILE_WRITE opens file for writing and moves to the end of the file, returns 0 if not available
   //     if (datafile) {
   //     for (int entry = 0; entry <= 5; entry = entry + 1) {
   //     datafile.print(entry);
  //    Serial.print(entry); 
 //   }
  //}
       
}

void loop()
{

  
      int cb = udp.parsePacket();
     // Serial.println(cb);
      if (cb)
      {
     //   Serial.println("no");
           //     Serial.setDebugOutput(true);
   //   }
    //  else
   //   {
      //  Serial.println("si");
       
            // We've received a packet, read the data from it
            udp.read(packetBuffer, PACKET_SIZE); // read the packet into the buffer
         //  Serial.println(PACKET_SIZE);

        //   File datafile = SD.open("data.jnr", FILE_READ); // FILE_WRITE opens file for writing and moves to the end of the file, returns 0 if not available
            
      //   if (datafile) {  
       //   for (int entry = 0; entry <= PACKET_SIZE; entry = entry + 1) {
      //  datafile.print(packetBuffer[entry]);
  //   Serial.println("data.txt ok"); 
  //   datafile.read(packetBuffer, PACKET_SIZE);
     
 //   }
    //    }
            if (cb >= 6 && packetBuffer[0] == 0x9C)
            {
           //   datafile.write(packetBuffer, PACKET_SIZE);
           //   datafile.close();
                  // header identifier (packet start)
                  byte blocktype = packetBuffer[1]; // block type (0xDA)
                  unsigned int framelength = ((unsigned int)packetBuffer[2] << 8) | (unsigned int)packetBuffer[3]; // frame length (0x0069) = 105 leds
                   //   Serial.print("Frame.");
                  //    Serial.println(framelength); // chan/block
                  byte packagenum = packetBuffer[4];   // packet number 0-255 0x00 = no frame split (0x01)
                  byte numpackages = packetBuffer[5];   // total packets 1-255 (0x01)
//Serial.println(blocktype);
                  if (blocktype == 0xDA)
                  {
                        // data frame ...
                        //        Serial.println("command");

                        int packetindex;
//Serial.println(cb);
int a = framelength + 7 && packetBuffer[6 + framelength] == 0x36;
//Serial.print("framelength :");
//Serial.println(framelength);

//Serial.print("packetBuffer :");
//Serial.println(packetBuffer[6 + framelength]);

  // if (cb >= framelength + 7 && packetBuffer[6 + framelength] == 0x36)
                       if (cb)
                        {
                              // header end (packet stop)
                              //Serial.println("s:");
                              int i = 0;
                              packetindex = 6;
                              if (packagenum == 1)
                              {
                                    led_index = 0;
                              }
                              while (packetindex < (framelength + 6))
                              {
                                    led.r = ((int)packetBuffer[packetindex]);
                                    led.g = ((int)packetBuffer[packetindex + 1]);
                                    led.b = ((int)packetBuffer[packetindex + 2]);
                                    pixels.setPixelColor(led_index, led.r, led.g, led.b);
                                    led_index++;
                                //   Serial.println(led_index);
                              //     Serial.print("  ");
                               //    Serial.println(packetindex);

                                    packetindex += 3;
                              }
                        }
                      //   Serial.print(packagenum);
                       //   Serial.print("/");
                      //   Serial.println(numpackages);

                  }

                  if (packagenum == numpackages) //&& (led_index == NUMPIXELS))
                 // if (packagenum )
                  {
               //     Serial.print("ok");
                        pixels.show();
                        led_index == 0;
                   //     Serial.println("ok");
                  }

            }

      }

}

void initTest()
{
   Serial.println("Test");
 //  for (int i = 0 ; i < NUMPIXELS  ; i++)
  //   pixels.setPixelColor(i, 0, 0, 0);
 // pixels.show();
  
  for (int i = 0 ; i < NUMPIXELS ; i++){
    pixels.setPixelColor(i, 127, 0, 0);
   // delay(500);
  }
  pixels.show();
  delay(500);
  for (int i = 0 ; i < NUMPIXELS  ; i++)
     pixels.setPixelColor(i, 0, 127, 0);
  pixels.show();
  delay(500);
  for (int i = 0 ; i < NUMPIXELS  ; i++)
     pixels.setPixelColor(i, 0, 0, 127);
  pixels.show();
  delay(500);
  for (int i = 0 ; i < NUMPIXELS  ; i++)
     pixels.setPixelColor(i, 0, 0, 0);
  pixels.show();
}
