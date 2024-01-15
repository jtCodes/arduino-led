// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h> // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
// #include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 144 // Number of LEDs in strip

// Here's how to control th.e LEDs from any two pins:
#define DATAPIN 11
#define CLOCKPIN 13
// Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BRG);

void setup()
{

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

    strip.begin(); // Initialize pins for output
    strip.show();  // Turn all LEDs off ASAP
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int head = 0, tail = -10;  // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000; // 'On' color (starts red)

void loop()
{

    strip.setPixelColor(head, color); // 'On' pixel at head
    strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
    strip.show();                     // Refresh strip
    delay(20);                        // Pause 20 milliseconds (~50 FPS)

    if (++head >= NUMPIXELS)
    {                           // Increment head index.  Off end of strip?
        head = 0;               //  Yes, reset head index to start
        if ((color >>= 8) == 0) //  Next color (R->G->B) ... past blue now?
            color = 0xFF0000;   //   Yes, reset to red
    }
    if (++tail >= NUMPIXELS)
        tail = 0; // Increment, reset tail index
}

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
