#include <ArtnetWifi.h>
#include <FastLED.h>

// LED Strip
const int numLeds = 180;                  // Change if your setup has more or less LED's
const int numberOfChannels = numLeds * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
#define DATA_PIN 19                       // The data pin that the WS2812 strips are connected to.
#define DATA_PIN2 23                      // The data pin that the WS2812 strips are connected to.
#define DATA_PIN3 18                      // The data pin that the WS2812 strips are connected to.
CRGB leds[numLeds];
CRGB leds2[numLeds];

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 250;

bool sendFrame = 1;
int previousDataLength = 0;

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
  sendFrame = 1;

  // read universe and put into the right part of the display buffer

  if (universe == 250)
  {
    for (int i = 0; i < length / 3; i++)
    {
      int led = i + (universe - startUniverse) * (previousDataLength / 3);
      if (led < numLeds)
      {
        leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      }
    }
    previousDataLength = length;
    FastLED.show();
  }
  else if (universe == 251)
  {
    for (int i = 0; i < length / 3; i++)
    {
      int led = i + (universe - 251) * (previousDataLength / 3);
      if (led < numLeds)
      {
        leds2[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      }
    }
    previousDataLength = length;
    FastLED.show();
  }
}

void arnetSetup()
{

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, numLeds);
  FastLED.addLeds<WS2812, DATA_PIN2, GRB>(leds2, numLeds);
  FastLED.addLeds<WS2812, DATA_PIN3, GRB>(leds2, numLeds);
  // onDmxFrame will execute every time a packet is received by the ESP32
  artnet.setArtDmxCallback(onDmxFrame);

  artnet.begin();
}

void artnetParse()
{
  artnet.read();
  FastLED.show();
}
