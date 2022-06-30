#include <OSCMessage.h>
#include <DMXSerial.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
//#include <OSCMessage.h>

EthernetUDP Udp;

int canal1;
int canal2;
int canal3;
int canal4;
int canal5;
int canal6;

bool estat1 = true;
bool estat2 = true;
bool estat3 = true;
bool estat4 = true;
bool estat5 = true;
bool estat6 = true;

//the Arduino's IP
IPAddress ip(192, 168, 0, 30);
//destination IP
IPAddress outIp(192, 168, 0, 40);
const unsigned int outPort = 1111;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
}; // you can find this written on the board of some Arduino Ethernets or shields
void setup() {
  
  Ethernet.begin(mac, ip);
  Udp.begin(1111);

  delay(1000);
  DMXSerial.init(DMXReceiver);

}


void loop() {

  // Calculate how long no data backet was received
  unsigned long lastPacket = DMXSerial.noDataSince();

  //Llegir canals
  canal1 = DMXSerial.read(1);
  canal2 = DMXSerial.read(2);
  canal3 = DMXSerial.read(3);
  canal4 = DMXSerial.read(4);
  canal5 = DMXSerial.read(5);
  canal6 = DMXSerial.read(6);

  //the message wants an OSC address as first argument
  //OSCMessage msg("/media_index/0");

  if ( canal1 == 255 && estat1 == true) {
    OSCMessage msg("/media_index/0");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    estat1 = false;
    return;
  } else if ( canal1 < 255){
    estat1 = true;
  }

  if ( canal2 == 255 && estat2 == true) {
    OSCMessage msg("/media_index/1");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    estat2 = false;
    return;
  } else if ( canal2 < 255){
    estat2 = true;
    }

if ( canal3 == 255 && estat3 == true) {
    OSCMessage msg("/media_index/2");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    estat3 = false;
    return;
  } else if ( canal3 < 255){
    estat3 = true;
    }

  if ( canal4 == 255 && estat4 == true) {
    OSCMessage msg("/media_index/3");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    estat4 = false;
    return;
  } else if ( canal4 < 255){
    estat4 = true;
    }

  if ( canal5 == 255 && estat5 == true) {
    OSCMessage msg("/media_index/4");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    estat5 = false;
    return;
  } else if ( canal5 < 255){
    estat5 = true;
  }

 // Brillo master
    OSCMessage msg("/master_luminosity");
    msg.add((float)canal6/255);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
 

  delay(20);
}
