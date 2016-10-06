#include "Wire.h"
#include <LiquidCrystal.h>

#define DS3231_I2C_ADDRESS 0x68

#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3
 
#define FADESPEED (1000 * 10)
#define DELTA 0.01
#define STEP 0.0005

LiquidCrystal lcd(12, 11, 10, 4, 9, 2);

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

float currentHue;
float targetHue;
String lcdString;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  auto seed(getSeconds());  
  randomSeed(seed);

  lcd.begin(16, 2);

  currentHue = getRandomHue();
  targetHue = getRandomHue();
}
  
float getRandomHue()
{
  return static_cast <float>(random(0, 1000)) / 1000.0;
}

void loop()
{  
  if (abs(currentHue - targetHue) < DELTA)
  {
    targetHue = getRandomHue();
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

  lcd.clear();
  lcd.setCursor(0, 0);
  GetTimeString(lcdString);
  //Serial.println(lcdString);  
  lcd.print(lcdString);
  
  lcdString = "RGB: ";
  lcdString += col.r;
  lcdString += "/";
  lcdString += col.g;
  lcdString += "/";
  lcdString += col.b;
  lcd.setCursor(0, 1);
  lcd.print(lcdString);
  
  delay(FADESPEED);  
  //displayTime();
}

Color HsvToRgb(float h, float s, float v)
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

byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

void GetTimeString(String& s)
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  s = "";
  if (hour < 10)
  {
    s += '0';
  }
  s += hour;
  s += ':';
  if (minute < 10)
  {
    s += '0';
  }
  s += minute;
  s += ':';
  if (second < 10)
  {
    s += '0';
  }
  s += second;
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute < 10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch (dayOfWeek) {
    case 1:
      Serial.println("Sunday");
      break;
    case 2:
      Serial.println("Monday");
      break;
    case 3:
      Serial.println("Tuesday");
      break;
    case 4:
      Serial.println("Wednesday");
      break;
    case 5:
      Serial.println("Thursday");
      break;
    case 6:
      Serial.println("Friday");
      break;
    case 7:
      Serial.println("Saturday");
      break;
  }
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

byte getSeconds()
{
  byte seconds = 0;
  byte dummy;
  // retrieve data from DS3231
  readDS3231time(&seconds, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy);
  return seconds;
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
