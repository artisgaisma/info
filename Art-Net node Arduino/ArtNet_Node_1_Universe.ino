#include <Artnet.h>
#include <DMXSerial.h>

const IPAddress ip(192, 168, 0, 180); // set IP adress
uint8_t mac[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB}; // set MAC adress
ArtnetReceiver artnet;

void setup() {
  Ethernet.begin(mac, ip);
  artnet.begin();
  artnet.subscribe(0, callback); //set universe
  DMXSerial.init(DMXController);
}
void loop() {
  artnet.parse();
}
void callback(uint8_t* data, uint16_t size){
  for(int i=0;i<512;i++){
    DMXSerial.write(i+1,data[i]);
  }
}
