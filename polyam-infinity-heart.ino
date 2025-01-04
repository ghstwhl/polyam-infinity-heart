#include <FastLED.h>


// How many leds in your strip?
// Heart is 0-49 [50]
// Infiity loop is 50-108 [59]
#define NUM_LEDS 109
#define HEART_LEDS 50
#define HEART_START 0
#define HEART_STOP 49
#define INFINITY_LEDS 59
#define INFINITY_START 50
#define INFINITY_STOP 108

#define SLOW_FACTOR 4


// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 6

const uint8_t heartECG[] = { 
  231, 200, 151, 102,  54,  18,  15,  25,  34,  44, 
   49,  49,  50,  53,  55,  58,  59,  58,  57,  56,
   55,  55,  56,  58,  59,  61,  62,  64,  66,  67,
   69,  71,  73,  75,  76,  78,  81,  84,  86,  89,
   92,  95,  98, 100, 103, 106, 109, 110, 112, 113,
  113, 111, 110, 108, 106, 103,  99,  95,  92,  88,
   85,  81,  78,  74,  71,  68,  64,  61,  58,  56,
   54,  53,  51,  51,  51,  50,  50,  50,  50,  50,
   49,  49,  49,  49,  49,  49,  49,  49,  49,  49,
   49,  49,  49,  50,  50,  50,  50,  51,  51,  51,
   51,  52,  52,  52,  53,  53,  53,  54,  54,  54,
   55,  55,  56,  56,  56,  56,  55,  55,  54,  53,
   52,  51,  49,  48,  46,  44,  43,  42,  42,  42,
   43,  43,  44,  43,  42,  41,  40,  41,  42,  40,
   37,  33,  32,  46,  83, 133, 170, 207, 227 };

// Define the complete array of leds
CRGB leds[NUM_LEDS];

// Define a working array for the infinity logo
CRGB infinityleds[INFINITY_LEDS];

int heartpulse = 0;
int slowfactor = SLOW_FACTOR;
bool blinkstatus = true;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(64);
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  for (int x=0; x<NUM_LEDS; x++) {
    // Now turn the LED off, then pause
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { 
  
  for (int x=HEART_START; x<=HEART_STOP; x++) {
    // Turn the LED on, then pause
    leds[x].red = heartECG[heartpulse];
  }

  slowfactor--;
  if (slowfactor == 0) {
    slowfactor = SLOW_FACTOR;
    heartpulse++;
    if (heartpulse == 149) {
       heartpulse = 0;
    }
  if (blinkstatus) {
    digitalWrite(LED_BUILTIN, HIGH);
    blinkstatus = false;
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    blinkstatus = true;
  }
}

  // Sweep the loop
  infinitypride();
  for(int i = 0; i < INFINITY_LEDS; i++)  {
    leds[(INFINITY_START + i)] = infinityleds[i];               // Set the i'th led to red 
  }  
  FastLED.show();

  
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void infinitypride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < INFINITY_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (INFINITY_LEDS-1) - pixelnumber;
    
    nblend( infinityleds[pixelnumber], newcolor, 64);
  }
}
