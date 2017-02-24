#include <Adafruit_NeoPixel.h>

/*
  This sketch uses the buzzer to play songs.
  The Arduino's tone() command will play notes of a given frequency.
  We'll provide a function that takes in note characters (a-g),
  and returns the corresponding frequency from this table:

  note  frequency
  c     262 Hz
  d     294 Hz
  e     330 Hz
  f     349 Hz
  g     392 Hz
  a     440 Hz
  b     494 Hz
  C     523 Hz

  For more information, see http://arduino.cc/en/Tutorial/Tone
*/

// We'll set up an array with the notes we want to play
// change these values to make different songs!

// Length must equal the total number of notes and spaces

// Notes is an array of text characters corresponding to the notes
// in your song. A space represents a rest (no tone)

//char notes[] = "cdfda ag cdfdg gf "; // a space represents a rest
char notes[] = "g C"; // a space represents a rest
const int songLength = 3;

// Beats is an array of values for each note and rest.
// A "1" represents a quarter-note, 2 a half-note, etc.
// Don't forget that the rests (spaces) need a length as well.

int beats[] = {1, 1, 8};

// The tempo is how fast to play the song.
// To make the song play faster, decrease this value.
int tempo = 60;

#define BUZZER_PIN  9
#define BUTTON_PIN  2
#define LEDS_PIN    6
#define NUM_LEDS    4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LEDS_PIN, NEO_GRB + NEO_KHZ800);

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
  Serial.println("start");
  randomSeed(analogRead(0));
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  strip.begin();
  newColor();
}

int lowCount = 0;

void loop()
{
  // read the state of the pushbutton value:
  int state = digitalRead(BUTTON_PIN);
  if (state == LOW)
  {    
    if (lowCount++ == 10)
    {
      Serial.println("SWITCH");      
      newColor();
    }
  }
  else
  {
    lowCount = max(0, lowCount -1);
  }  
  delay(5);
}

float getRandomHue()
{
  return static_cast <float>(random(0, 1000)) / 1000.0;
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


void newColor()
{
  auto color = HsvToRgb(getRandomHue(), 1, 1);
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    strip.setPixelColor(i, strip.Color(color.r, color.g, color.b));
  }
  strip.show();
  play();
}


void play()
{
  int i, duration;

  for (i = 0; i < songLength; i++) // step through the song arrays
  {
    duration = beats[i] * tempo;  // length of note/rest in ms

    if (notes[i] == ' ')          // is this a rest?
    {
      delay(duration);            // then pause for a moment
    }
    else                          // otherwise, play the note
    {
      tone(BUZZER_PIN, frequency(notes[i]), duration);
      delay(duration);            // wait for tone to finish
    }
    delay(tempo / 10);            // brief pause between notes
  }
}


int frequency(char note)
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 8;  // number of notes we're storing

  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return (frequencies[i]);    // Yes! Return the frequency
    }
  }
  return (0); // We looked through everything and didn't find it,
  // but we still need to return a value, so return 0.
}

