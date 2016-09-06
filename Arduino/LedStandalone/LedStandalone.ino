#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
 
#define FADESPEED (1000 * 5)
#define DELTA 0.01
#define STEP 0.005

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
 
void setup()
{
  Serial.begin(9600);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  randomSeed(analogRead(0));
}
  
void loop()
{
  float currentHue = 0;
  float targetHue = 0;
  
  if (abs(currentHue - targetHue) < DELTA)
  {
    targetHue = static_cast <float>(rand()) / static_cast<float>(RAND_MAX); // rand_max?
  }
  if (currentHue > targetHue)
  {
    currentHue -= DELTA;
  }
  else
  {
    currentHue += DELTA;
  }

  currentHue = max(0, min(1, currentHue)); // clamp
  auto col = HsvToRgb(currentHue, 1, 1);
  analogWrite(REDPIN, col.r);
  analogWrite(GREENPIN, col.g);
  analogWrite(BLUEPIN, col.b);

  delay(FADESPEED);
}

Color HsvToRgb(float h, float s, float v)
{
  byte r;
  byte g;
  byte b;
  long i = floor(h * 6);
  auto f = h * 6 - i;
  auto p = v * (1 - s);
  auto q = v * (1 - f * s);
  auto t = v * (1 - (1 - f) * s);

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
