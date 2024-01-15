#include <Adafruit_DotStar.h>
#include <SPI.h>    
#include <ArduinoJson.h>     

#define NUMPIXELS 144 

#define DATAPIN    11
#define CLOCKPIN   13

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

void setup() {
  Serial.begin(9600); 

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); 
#endif

  strip.begin(); 
  strip.show();  
}

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

// void loop() {

//   strip.setPixelColor(head, color); // 'On' pixel at head
//   strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
//   strip.show();                     // Refresh strip
//   delay(20);                        // Pause 20 milliseconds (~50 FPS)

//   if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
//     head = 0;                       //  Yes, reset head index to start
//     if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
//       color = 0xFF0000;             //   Yes, reset to red
//   }
//   if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
// }

// void loop() 
// {

//   // loop through each pixel in the control array setting it to the colour mix you want.  
//   for (int i=0; i <= NUMPIXELS; i=i+1) {
//     strip.setPixelColor(i, 25, 25, 25); // Even 25 on my strip gives reasonably bright flash
//   }

//   strip.show(); // Then push the control array to the strip 
//   delay(50); // and leave it set for a while

// // Then do it again for the 2nd brightness in your scheme

//   // loop through each pixel in the control array setting it to off.  
//   for (int i=0; i <= NUMPIXELS; i=i+1) {
//     strip.setPixelColor(i, 0, 0, 0); 
//   }

//   strip.show(); // Then push the control array to the strip 
//   delay(50); // and leave it set for a while

// }

String inputString = "";     // A string to hold incoming data
bool stringComplete = false; // Whether the string is complete

int r = 0;
int g = 0;
int b = 0;
int brightness = 20;

void loop() {
  if (stringComplete) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, inputString);
    JsonObject obj = doc.as<JsonObject>();

    r = obj["r"];
    g = obj["g"];
    b = obj["b"];
    brightness = obj["bn"];

    strip.setBrightness(brightness);
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();

    // Clear the string for the next message
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
