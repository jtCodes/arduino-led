#include "FastLED.h"
#include <ArduinoJson.h>

#define COLOR_ORDER GRB
CRGB leds[64];

String json = "{\"r\":255,\"g\":160,\"b\":33}";

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

int r = 0;
int g = 0;
int b = 0;
int brightness = 20;

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, 7, GRB>(leds, 64);
  FastLED.setBrightness(brightness);

  // leds[2] = CRGB(r, g, b);
  // FastLED.show();
}

void loop()
{
  static uint8_t hue = 0;
  // FastLED.showColor(CHSV(hue++, 255, 10));
  // FastLED.showColor(CHSV(hue++, 150, 255));

  //   FastLED.show();
  //  delay(500);
  //  leds[2] = CHSV(339,25,0);
  //    FastLED.show();
  //    delay(500);
  if (stringComplete)
  {

    DynamicJsonDocument doc(1024);

    // You can use a String as your JSON input.
    // WARNING: the string in the input  will be duplicated in the JsonDocument.
    deserializeJson(doc, inputString);
    JsonObject obj = doc.as<JsonObject>();

    // Fetch values.
    //
    // Most of the time, you can rely on the implicit casts.
    // In other case, you can do doc["time"].as<long>();
    r = obj[String("r")];
    g = obj[String("g")];
    b = obj[String("b")];
    brightness = obj[String("bn")];

    // leds[2] = CRGB(r, g, b);

    //  FastLED.showColor(CRGB(r, g, b));

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  // FastLED.show();
  FastLED.setBrightness(brightness);
  FastLED.showColor(CRGB(r, g, b));
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    else
    {
      inputString += inChar;
    }
  }
}
