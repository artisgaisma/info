#include <DmxReceiver.h>
#include <FastLED.h>

#define DMX_NUM_CHANNELS 15
#define NUM_LEDS 15
#define DATA_PIN 12
#define LED_TYPE WS2811
#define COLOUR_ORDER
#define CLOCK_PIN 13 // Clock pin only needed for SPI based chipsets when not using hardware SPI

CRGB leds[NUM_LEDS];

DmxReceiver myDMX = DmxReceiver();

volatile unsigned char dmx_data[DMX_NUM_CHANNELS];
volatile unsigned char data;

void setup(void) {

#ifdef USE_SERIAL
  Serial.begin(115200);
#endif

  FastLED.addLeds<LED_TYPE, DATA_PIN, CLOCK_PIN, COLOUR_ORDER>(leds, NUM_LEDS);

}

void loop(){

#ifdef USE_SERIAL
  if (Serial.available()){
    handleInput(Serial.read());
  }
#endif

  DMX.bufferService();

  if (DMX.newFrame()) {
    for(int i=1; i < DMX_NUM_CHANNELS; i++){
      dmx_data[i] = dmx_getDimmer(i);
    }
    for(int i=0; i < NUM_LEDS; i++) {
      leds[i].setRGB( dmx_data[i], 0,0);
    }
    FastLED.show();
  }

}
