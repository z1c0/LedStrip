#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LEDS 31

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//
// Effects are copied from this great tutorial
// http://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#rainbow_cycle
//


void setPixel(int Pixel, byte red, byte green, byte blue)
{
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void showStrip()
{
  strip.show();
}

void setAll(byte red, byte green, byte blue)
{
  for(int i = 0; i < NUM_LEDS; i++ )
  {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);
  
  strip.begin();
  strip.show();
}


#define MAX_ANIM 4
static int animation = 0;
static int runs = 0;

void loop()
{  
  if (runs <= 0)
  {
    runs = random(10, 100);
    animation++;
    if (animation > MAX_ANIM)
    {
      animation = 0;
    }
    Serial.print("New Animation #");
    Serial.print(animation);
    Serial.print(", runs: ");
    Serial.println(runs);
  }
  runs--;
  //Serial.println(runs);
   
  switch (animation)
  {
    case 0:
      TwinkleRandom(20, 250, false);
      break;
      
    case 1:
      theaterChaseRainbow(50);
      break;
      
    case 2:
      rainbowCycle(25);
      break;
      
    case 3:
      Fire(55, 180, 50);
      break;

    case MAX_ANIM:
    default:
      CylonBounce(0xff, 0, 0, 4, 75, 75);
      break;
  }
}

void Fire(int Cooling, int Sparking, int SpeedDelay)
{
  static byte heat[NUM_LEDS];
  for (int i = 0; i < 500; i++)
  {
    int cooldown;
    
    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
      
      if(cooldown>heat[i]) {
        heat[i]=0;
      } else {
        heat[i]=heat[i]-cooldown;
      }
    }
    
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
      
    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if( random(255) < Sparking ) {
      int y = random(7);
      heat[y] = heat[y] + random(160,255);
      //heat[y] = random(160,255);
    }
  
    // Step 4.  Convert heat to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      setPixelHeatColor(j, heat[j] );
    }
  
    showStrip();
    delay(SpeedDelay);
  }
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}


void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++)
  {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne)
     { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}


void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void theaterChaseRainbow(int SpeedDelay)
{
  byte *c; 
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < NUM_LEDS; i=i+3) {
          c = wheel( (i+j) % 255);
          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        showStrip();
       
        delay(SpeedDelay);
       
        for (int i=0; i < NUM_LEDS; i=i+3) {
          setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}


void rainbowCycle(int SpeedDelay)
{
  byte *c;
  uint16_t i, j;
  for (j=0; j< 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i=0; i< NUM_LEDS; i++)
    {
      c = wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte* wheel(byte wheelPos)
{
  static byte c[3];
  
  if (wheelPos < 85)
  {
   c[0]= wheelPos * 3;
   c[1]=255 - wheelPos * 3;
   c[2]=0;
  } else if(wheelPos < 170) 
  {
   wheelPos -= 85;
   c[0]=255 - wheelPos * 3;
   c[1]=0;
   c[2]=wheelPos * 3;
  } else {
   wheelPos -= 170;
   c[0]=0;
   c[1]=wheelPos * 3;
   c[2]=255 - wheelPos * 3;
  }
  return c;
}


