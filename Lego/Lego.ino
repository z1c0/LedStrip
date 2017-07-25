#include <Adafruit_NeoPixel.h>

#define LEDS_PIN    4
#define NUM_LEDS    3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

float getRandomHue()
{
  return static_cast <float>(random(0, 1000)) / 1000.0;
}

struct Color
{
  Color(byte r, byte g, byte b) 
  {
    this->r = r;
    this->g = g;
    this->b = b;
  }
  byte r;
  byte g;
  byte b;
};

struct Color HsvToRgb(float h, float s, float v)
{
  float r;
  float g;
  float b;
  long i = floor(h * 6);
  float f = h * 6 - i;
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  switch (i % 6) 
  {
        case 0:
            r = v, g = t, b = p;
            break;
        case 1:
            r = q, g = v, b = p;
            break;
        case 2:
            r = p, g = v, b = t;
            break;
        case 3:
            r = p, g = q, b = v;
            break;
        case 4:
            r = t, g = p, b = v;
            break;
        case 5:
            r = v, g = p, b = q;
            break;
    }
    r = floor(r * 255);
    g = floor(g * 255);
    b = floor(b * 255);
    return Color(r, g, b);
}

void setup() 
{
  strip.begin();
}

void loop() 
{
  Color colHead(HsvToRgb(getRandomHue(), 1, 1));
  Color colBody(HsvToRgb(getRandomHue(), 1, 1));

  strip.setPixelColor(0, colBody.r, colBody.g, colBody.b);
  strip.setPixelColor(1, colBody.r, colBody.g, colBody.b);
  strip.setPixelColor(2, colHead.r, colHead.g, colHead.b);
  strip.show();

  delay(10 * 1000);
}
