#include <EtherCard.h>

#define B_PIN 3
#define R_PIN 5
#define G_PIN 6

static byte mymac[] = { 0x74, 0x59, 0x70, 0x2D, 0x30, 0x32 };
static byte myip[] = { 192, 168, 1, 45 };

byte Ethernet::buffer[500];
BufferFiller bfill;

const char HttpNotFound[] PROGMEM =
  "HTTP/1.0 404 Unauthorized\r\n"
  "Content-Type: text/html\r\n\r\n"
  "<h1>404 Page Not Found</h1>";


void setup ()
{
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
  {
    Serial.println("Failed to access Ethernet controller");
  }
  ether.staticSetup(myip);
}

static word HomePage(byte r, byte g, byte b)
{
  bfill = ether.tcpOffset();  
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "{ 'red': $D, 'green': $D, 'blue': $D }"), r, g, b);
  return bfill.position();
}

int r = 0;
int g = 0;
int b = 0;

void loop ()
{
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  if (pos)  // check if valid tcp data is received
  {
    bfill = ether.tcpOffset();
    char* data = (char*) Ethernet::buffer + pos;
    if (strncmp("GET /", data, 5) != 0)
    {
      bfill.emit_p(HttpNotFound);
    }
    else
    {
      data += 5;
      sscanf(data, "?r=%d&g=%d&b=%d", &r, &g, &b);
      HomePage(r, g, b);
    }
    ether.httpServerReply(bfill.position());    // send http response
  }
  analogWrite(B_PIN, b);
  analogWrite(R_PIN, r);
  analogWrite(G_PIN, g);
}
