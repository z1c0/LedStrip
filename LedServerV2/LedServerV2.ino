#include <ESP8266WiFi.h>
#include "wifi.h"

const int redPin = 12;
const int greenPin = 13;
const int bluePin = 14;

WiFiServer server(80);

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void setup()
{
  // Start off with the LED off.
  setColourRgb(0,0,0);

  Serial.begin(115200);
  delay(10); 

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");   
}

byte parseComponent(const String& request, const char* key)
{
  byte value = 0;
  auto i = request.indexOf(key);
  if (i != -1)
  {
    value = atoi(request.c_str() + i  + strlen(key));
  }
  return value;
}

void loop()
{
  delay(50);
  Serial.print('.');
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {  
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf('?') != -1)
  { 
    // Match the request
    auto r = parseComponent(request, "r=");
    auto g = parseComponent(request, "g=");
    auto b = parseComponent(request, "b=");
    auto x = parseComponent(request, "x=");
    
    String rgb = "R = ";
    rgb += r;
    rgb += ", G = ";
    rgb += g;
    rgb += ", B = ";
    rgb += b;
    rgb += " (x = ";
    rgb += x;
    rgb += ")";
    Serial.println(rgb);
    setColourRgb(r, g, b);
    
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>"); 
    client.println(rgb); 
    client.println("</html>"); 
    delay(5);
  }
  Serial.println("Client disconnected");
  Serial.println("");   
}

 
 void colorLoop()
 {
  unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(15);
    }
  }
}


