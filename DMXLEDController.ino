#include <cstring>

#include <FastLED.h>
#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;

#define NUM_LEDS 30
#define DATA_PIN 11
#define CLOCK_PIN 13

#define DEBUG false

CRGB leds[NUM_LEDS];

teensydmx::Receiver dmxRx{Serial1};

// Buffer in which to store packet data.
uint8_t packetBuf[NUM_LEDS * 3]{0};

// The last values received, initialized to zero.
uint8_t rgb[NUM_LEDS * 3]{0};


void setup() {
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
 
    if(DEBUG){
        // Serial initialization, for printing things
        Serial.begin(115200);
        while (!Serial && millis() < 4000) {
            // Wait for initialization to complete or a time limit
        }
        
        Serial.println("Starting Receiver");
    }
  // Turn on the LED, for indicating activity
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWriteFast(LED_BUILTIN, HIGH);

  
  // Start the receiver
  dmxRx.begin();

}

void loop() {

  //Read DMX
  int read = dmxRx.readPacket(packetBuf, 1, (NUM_LEDS * 3));
    
  if (read == (NUM_LEDS * 3)) { 
    if (memcmp(packetBuf, rgb, (NUM_LEDS * 3)) != 0) {
      
      memcpy(rgb, packetBuf, (NUM_LEDS * 3));
       
      if(DEBUG){
        Serial.printf("RGB: %d %d %d\n", rgb[0], rgb[1], rgb[2]);
      }
        
      int p = 0;
      for(int i = 0; i < NUM_LEDS; i++)
      {
         leds[i] = CRGB(rgb[p+2],rgb[p+1],rgb[p]);
         p = p + 3;
      }
     
      FastLED.show();
    }
  }

  
}
