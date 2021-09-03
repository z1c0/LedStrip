#include "FastLED.h"

#define NUM_LEDS_BIG   24
#define NUM_LEDS_SMALL 12
CRGB ledsBig[NUM_LEDS_BIG];
CRGB ledsSmall[NUM_LEDS_SMALL];
int pos = 0;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  FastLED.addLeds<NEOPIXEL, 6>(ledsBig, NUM_LEDS_BIG);
  FastLED.addLeds<NEOPIXEL, 5>(ledsSmall, NUM_LEDS_SMALL);
}

void ring(CRGB* leds, int ledCount, const CRGB& col, int pos)
{
  pos = pos % ledCount;   
  for (auto i = 0; i < ledCount / 4; i++) 
  {
    auto a = (pos + ledCount + i) % ledCount; 
    auto b = (pos + ledCount - i) % ledCount; 
    leds[a] = col;
    leds[b] = col;
    FastLED.show();
    delay(35);    
  }
  delay(60);    
  
  for (auto i = ledCount / 4 - 1; i >= 0; i--)
  {
    auto a = (pos + ledCount + i) % ledCount; 
    auto b = (pos + ledCount - i) % ledCount; 
    leds[a] = CRGB::Black;
    leds[b] = CRGB::Black;
    FastLED.show();
    delay(10);    
  }
}

CRGB col1 = CRGB::Black;
CRGB col2 = CRGB::Black;
bool lightOn = false;
long timeStamp = 0;

void loop()
{
  auto light = analogRead(0);
  // help with random
  if (random(100000) == 500) {
    light = 1000;
  }
  //Serial.println(light);
  if (light > 900)
  {
    timeStamp = millis();
    //if (!lightOn)
    {
      lightOn = true;
      col1 = GetRandomColor();
      col2 = GetRandomColor();
    }
  }
  if (lightOn && millis() - timeStamp > 3500)
  {
    col1 = CRGB::Black;
    col2 = CRGB::Black;
    lightOn = false;
  }

  if (lightOn)
  {
    ring(ledsBig, NUM_LEDS_BIG, col1, pos);
    ring(ledsSmall, NUM_LEDS_SMALL, col2, pos);  
    pos += 1;
  }
  else
  {
    FastLED.clear();
    FastLED.show();
  }
}


CRGB GetRandomColor()
{
  static CRGB::HTMLColorCode colors[] = 
  {
    CRGB::AliceBlue,
    CRGB::Amethyst,
    CRGB::AntiqueWhite,
    CRGB::Aqua,
    CRGB::Aquamarine,
    CRGB::Azure,
    CRGB::Beige,
    CRGB::Bisque,
    // CRGB::Black
    CRGB::BlanchedAlmond,
    CRGB::Blue,
    CRGB::BlueViolet,
    CRGB::Brown,
    CRGB::BurlyWood,
    CRGB::CadetBlue,
    CRGB::Chartreuse,
    CRGB::Chocolate,
    CRGB::Coral,
    CRGB::CornflowerBlue,
    CRGB::Cornsilk,
    CRGB::Crimson,
    CRGB::Cyan,
    CRGB::DarkBlue,
    CRGB::DarkCyan,
    CRGB::DarkGoldenrod,
    CRGB::DarkGray,
    CRGB::DarkGrey,
    CRGB::DarkGreen,
    CRGB::DarkKhaki,
    CRGB::DarkMagenta,
    CRGB::DarkOliveGreen,
    CRGB::DarkOrange,
    CRGB::DarkOrchid,
    CRGB::DarkRed,
    CRGB::DarkSalmon,
    CRGB::DarkSeaGreen,
    CRGB::DarkSlateBlue,
    CRGB::DarkSlateGray,
    CRGB::DarkSlateGrey,
    CRGB::DarkTurquoise,
    CRGB::DarkViolet,
    CRGB::DeepPink,
    CRGB::DeepSkyBlue,
    CRGB::DimGray,
    CRGB::DimGrey,
    CRGB::DodgerBlue,
    CRGB::FireBrick,
    CRGB::FloralWhite,
    CRGB::ForestGreen,
    CRGB::Fuchsia, 
    CRGB::Gainsboro, 
    CRGB::GhostWhite,
    CRGB::Gold, 
    CRGB::Goldenrod, 
    CRGB::Gray,
    CRGB::Grey,
    CRGB::Green,
    CRGB::GreenYellow,
    CRGB::Honeydew,
    CRGB::HotPink,
    CRGB::IndianRed,
    CRGB::Indigo,
    CRGB::Ivory,
    CRGB::Khaki,
    CRGB::Lavender,
    CRGB::LavenderBlush,
    CRGB::LawnGreen,
    CRGB::LemonChiffon,
    CRGB::LightBlue,
    CRGB::LightCoral,
    CRGB::LightCyan,
    CRGB::LightGoldenrodYellow,
    CRGB::LightGreen,
    CRGB::LightGrey,
    CRGB::LightPink,
    CRGB::LightSalmon,
    CRGB::LightSeaGreen,
    CRGB::LightSkyBlue,
    CRGB::LightSlateGray,
    CRGB::LightSlateGrey,
    CRGB::LightSteelBlue,
    CRGB::LightYellow,
    CRGB::Lime,
    CRGB::LimeGreen,
    CRGB::Linen,
    CRGB::Magenta,
    CRGB::Maroon,
    CRGB::MediumAquamarine,
    CRGB::MediumBlue,
    CRGB::MediumOrchid,
    CRGB::MediumPurple,
    CRGB::MediumSeaGreen,
    CRGB::MediumSlateBlue,
    CRGB::MediumSpringGreen,
    CRGB::MediumTurquoise,
    CRGB::MediumVioletRed,
    CRGB::MidnightBlue,
    CRGB::MintCream,
    CRGB::MistyRose,
    CRGB::Moccasin,
    CRGB::NavajoWhite,
    CRGB::Navy,
    CRGB::OldLace,
    CRGB::Olive,
    CRGB::OliveDrab,
    CRGB::Orange,
    CRGB::OrangeRed,
    CRGB::Orchid,
    CRGB::PaleGoldenrod,
    CRGB::PaleGreen,
    CRGB::PaleTurquoise,
    CRGB::PaleVioletRed,
    CRGB::PapayaWhip,
    CRGB::PeachPuff,
    CRGB::Peru,
    CRGB::Pink,
    CRGB::Plaid,
    CRGB::Plum,
    CRGB::PowderBlue,
    CRGB::Purple,
    CRGB::Red,
    CRGB::RosyBrown,
    CRGB::RoyalBlue,
    CRGB::SaddleBrown,
    CRGB::Salmon,
    CRGB::SandyBrown,
    CRGB::SeaGreen,
    CRGB::Seashell,
    CRGB::Sienna,
    CRGB::Silver,
    CRGB::SkyBlue,
    CRGB::SlateBlue,
    CRGB::SlateGray,
    CRGB::SlateGrey,
    CRGB::Snow,
    CRGB::SpringGreen,
    CRGB::SteelBlue,
    CRGB::Tan,
    CRGB::Teal,
    CRGB::Thistle,
    CRGB::Tomato,
    CRGB::Turquoise,
    CRGB::Violet,
    CRGB::Wheat,
    CRGB::White,
    CRGB::WhiteSmoke,
    CRGB::Yellow,
    CRGB::YellowGreen,
    CRGB::FairyLight,
    CRGB::FairyLightNCC,
  };
  int nrOfColors = sizeof(colors) / sizeof(colors[0]);
  int index = random(nrOfColors);
  //Serial.println(nrOfColors);
  //Serial.println(index);
  return colors[index];
}
