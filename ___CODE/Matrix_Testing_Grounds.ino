#include <FastLED.h>

#define LED_PIN  33

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B

#define BRIGHTNESS 200
#define FRAMES_PER_SECOND  120

// Helper functions for an two-dimensional XY matrix of pixels.
// Simple 2-D demo code is included as well.
//
//     XY(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             No error checking is performed on the ranges of x and y.
//
//     XYsafe(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XYsafe(x,y) ] == CRGB::Red;
//             Error checking IS performed on the ranges of x and y, and an
//             index of "-1" is returned.  Special instructions below
//             explain how to use this without having to do your own error
//             checking every time you use this function.  
//             This is a slightly more advanced technique, and 
//             it REQUIRES SPECIAL ADDITIONAL setup, described below.


// Params for width and height
const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 8;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;
const bool    kMatrixVertical = true;

// Set 'kMatrixSerpentineLayout' to false if your pixels are 
// laid out all running the same way, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//     .----<----<----<----'
//     |
//     5 >  6 >  7 >  8 >  9
//                         |
//     .----<----<----<----'
//     |
//    10 > 11 > 12 > 13 > 14
//                         |
//     .----<----<----<----'
//     |
//    15 > 16 > 17 > 18 > 19
//
// Set 'kMatrixSerpentineLayout' to true if your pixels are 
// laid out back-and-forth, like this:
//
//     0 >  1 >  2 >  3 >  4
//                         |
//                         |
//     9 <  8 <  7 <  6 <  5
//     |
//     |
//    10 > 11 > 12 > 13 > 14
//                        |
//                        |
//    19 < 18 < 17 < 16 < 15
//
// Bonus vocabulary word: anything that goes one way 
// in one row, and then backwards in the next row, and so on
// is call "boustrophedon", meaning "as the ox plows."


// This function will return the right 'led index number' for 
// a given set of X and Y coordinates on your matrix.  
// IT DOES NOT CHECK THE COORDINATE BOUNDARIES.  
// That's up to you.  Don't pass it bogus values.
//
// Use the "XY" function like this:
//
//    for( uint8_t x = 0; x < kMatrixWidth; x++) {
//      for( uint8_t y = 0; y < kMatrixHeight; y++) {
//      
//        // Here's the x, y to 'led index' in action: 
//        leds[ XY( x, y) ] = CHSV( random8(), 255, 255);
//      
//      }
//    }
//
//
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
    }
  }

  if( kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y+1);
      }
    }
  }
  
  return i;
}


// Once you've gotten the basics working (AND NOT UNTIL THEN!)
// here's a helpful technique that can be tricky to set up, but 
// then helps you avoid the needs for sprinkling array-bound-checking
// throughout your code.
//
// It requires a careful attention to get it set up correctly, but
// can potentially make your code smaller and faster.
//
// Suppose you have an 8 x 5 matrix of 40 LEDs.  Normally, you'd
// delcare your leds array like this:
//    CRGB leds[40];
// But instead of that, declare an LED buffer with one extra pixel in
// it, "leds_plus_safety_pixel".  Then declare "leds" as a pointer to
// that array, but starting with the 2nd element (id=1) of that array: 
//    CRGB leds_with_safety_pixel[41];
//    CRGB* const leds( leds_plus_safety_pixel + 1);
// Then you use the "leds" array as you normally would.
// Now "leds[0..N]" are aliases for "leds_plus_safety_pixel[1..(N+1)]",
// AND leds[-1] is now a legitimate and safe alias for leds_plus_safety_pixel[0].
// leds_plus_safety_pixel[0] aka leds[-1] is now your "safety pixel".
//
// Now instead of using the XY function above, use the one below, "XYsafe".
//
// If the X and Y values are 'in bounds', this function will return an index
// into the visible led array, same as "XY" does.
// HOWEVER -- and this is the trick -- if the X or Y values
// are out of bounds, this function will return an index of -1.
// And since leds[-1] is actually just an alias for leds_plus_safety_pixel[0],
// it's a totally safe and legal place to access.  And since the 'safety pixel'
// falls 'outside' the visible part of the LED array, anything you write 
// there is hidden from view automatically.
// Thus, this line of code is totally safe, regardless of the actual size of
// your matrix:
//    leds[ XYsafe( random8(), random8() ) ] = CHSV( random8(), 255, 255);
//
// The only catch here is that while this makes it safe to read from and
// write to 'any pixel', there's really only ONE 'safety pixel'.  No matter
// what out-of-bounds coordinates you write to, you'll really be writing to
// that one safety pixel.  And if you try to READ from the safety pixel,
// you'll read whatever was written there last, reglardless of what coordinates
// were supplied.

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  rainInitial();
  
  FastLED.clear(true);
  delay(2000);
  FastLED.showColor(CRGB::Red);
  delay(300);
  FastLED.showColor(CRGB::Lime);
  delay(300);
  FastLED.showColor(CRGB::Blue);
  delay(300);
  FastLED.showColor(CRGB::White);
  delay(300);
  FastLED.clear(true);
  delay(500);
}

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {digitalRain, /*matrixEffect,*/ rectangleBreathe};

void loop()
{

  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

}

uint8_t hue = 0;

void rectangleBreathe()
{
  uint8_t squarespeed = beatsin8(45, 0, 3, 0, 0);
  
  fadeToBlackBy(leds, NUM_LEDS, BRIGHTNESS/2);
  
  for(uint8_t x = 0; x < kMatrixWidth - (squarespeed * 2); x++)   // horizontal lines of the squares
  {
    leds[XYsafe(x + squarespeed, 0 + squarespeed)] = CHSV(hue, 255, 255);
    leds[XYsafe(x + squarespeed, (kMatrixHeight - 1) - squarespeed)] = CHSV(hue, 255, 255);
  }
  for(uint8_t y = 0; y < kMatrixHeight - (squarespeed * 2); y++)   // vertical lines of the squares
  {
    leds[XYsafe(0 + squarespeed, y + squarespeed)] = CHSV(hue, 255, 255);
    leds[XYsafe((kMatrixWidth - 1) - squarespeed, y + squarespeed)] = CHSV(hue, 255, 255);
  }
  
  hue += 1;

}

/*void matrixEffect() {

  int frequency = 2;
  int xPos[frequency];
  int yPos[frequency];

  uint8_t startingPoint = random8(0, 5);
  
  uint8_t pace1 = map(beat8(20, 0), 0, 255, 0, kMatrixHeight);
  uint8_t pace2 = map(beat8(30, 0), 0, 255, 0, kMatrixHeight);
  
  fadeToBlackBy(leds, NUM_LEDS, 10);

  
  
  for(uint8_t i = 0; i < 2; i++) 
  {
    if(pace1 == 0)
    {
      xPos[i] = random8(0, 10);
    }

    
    yPos[i] = kMatrixHeight - pace1;
    leds[XYsafe(xPos[i], yPos[i])] = CHSV(100, 255, 255);
  }


  
}*/

void digitalRain() {

  EVERY_N_MILLISECONDS(100) {
    updateRain();
  }
}

byte rain[NUM_LEDS];

void updateRain() {

  fadeToBlackBy(leds, NUM_LEDS, BRIGHTNESS/3);
  
  leds[XYsafe(random8(kMatrixWidth), kMatrixHeight - 1)] = CHSV(110, 255, BRIGHTNESS);

  for(byte x = 0; x < kMatrixWidth; x++) {
    for(byte y = 0; y < kMatrixHeight - 1; y++) {
      leds[XYsafe(x, y)] = leds[XYsafe(x, y + 1)];
    }
  }
}

void rainInitial() {

  for(int i = 0; i < NUM_LEDS; i++) {
    if(random8(20) == 0) {
      rain[i] = 1;
    }
    else {
      rain[i] = 0;
    }
  }
}



























 
