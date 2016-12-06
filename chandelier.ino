#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <aREST.h>



// Initialize the NeoPixel
// #define PIN 13
// Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, PIN);
#define PIN_GRBW            14 // What pin is the data being sent to the Neopixels
#define PIN_GRB             13 // What pin is the data being sent to the Neopixels
#define NUMPIXELS_GRBW      16 // How many Neopixels are you using?
#define NUMPIXELS_GRB      64 // How many Neopixels are you using?

// Adafruit_NeoPixel pixels_grbw = Adafruit_NeoPixel(NUMPIXELS_GRBW, PIN_GRBW, NEO_RGBW + NEO_KHZ800); // Set up the Neopixels
// Adafruit_NeoPixel pixels_grb = Adafruit_NeoPixel(NUMPIXELS_GRB, PIN_GRB, NEO_GRB + NEO_KHZ800); // Set up the Neopixels
Adafruit_NeoPixel pixels_grbw = Adafruit_NeoPixel(NUMPIXELS_GRBW, PIN_GRBW, NEO_RGBW + NEO_KHZ800); // Set up the Neopixels
Adafruit_NeoPixel pixels_grb = Adafruit_NeoPixel(NUMPIXELS_GRB, PIN_GRB); // Set up the Neopixels

// Old school named array
String colorName[] = {"none", "red", "pink", "green", "blue", "cyan", "white", "warmwhite", "oldlace", "purple", "magenta", "yellow", "orange", "1999"}; // List of CheerLights color names
int colorRGB[][3] = {     0,  0,  0,  // "none"  // Map of RGB values for each of the Cheerlight color names
                          255,  0,  0,  // "red"
                          255, 192, 203, // "pink"
                          0, 255,  0, // "green"
                          0,  0, 255, // "blue"
                          0, 255, 255, // "cyan",
                          255, 255, 255, // "white",
                          255, 223, 223, // "warmwhite",
                          255, 223, 223, // "oldlace",
                          128,  0, 128, // "purple",
                          255,  0, 255, // "magenta",
                          255, 255,  0, // "yellow",
                          255, 165,  0, // "orange"
                          128,  0, 128
                    }; // "1999 Prince purple"

// Create aREST instance

aREST rest = aREST();

const char* ssid     = "StarBrite";
const char* password = "beatrice bodkins";

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Declare functions to be exposed to the API
int setPendantColor(String command);
int chasePendantColor(String command);
int showOffLamp(String command);
int multiChase(String command);
int lampMode(String command);

void setup() {
  pixels_grb.begin();
  pixels_grbw.begin();
  setColor(pixels_grb, pixels_grb.Color(8, 204, 60));
  chase(pixels_grbw, pixels_grbw.Color(128, 0, 128));

  Serial.begin(115200);
  delay(100);
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    chase(pixels_grbw, pixels_grbw.Color(255, 0, 255));
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  chase(pixels_grbw, pixels_grbw.Color(8, 204, 60));


  rest.function("color", setPendantColor);
  rest.function("chase", chasePendantColor);
  rest.function("multichase", multiChase);
  rest.function("showoff", showOffLamp);
  rest.function("lampmode", lampMode);
  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("1");
  rest.set_name("chandelier");

  // Start the server
  server.begin();
  Serial.println("Server started");
  setColor(pixels_grbw, pixels_grbw.Color(8, 204, 60));

}

void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);
}

// Custom function accessible by the API

int setPendantColor(String command) {
  // Look through the list of colors to find the one that was requested
  Serial.print("Setting the color to: ");
  Serial.println(command);
  for (int iColor = 0; iColor <= 13; iColor++)
  {
    if (command == colorName[iColor])
    {
      setColor(pixels_grbw, pixels_grbw.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
    Serial.print("The color should now be: ");
    Serial.println(command);
    return 1;
  }
}

int chasePendantColor(String command) {
  String color = command;
  // Look through the list of colors to find the one that was requested
  for (int iColor = 0; iColor <= 13; iColor++)
  {
    if (color == colorName[iColor])
    {
      chase(pixels_grbw, pixels_grbw.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
  }
  return 1;
}

int showOffLamp(String command) {
  String color = command;
  // Look through the list of colors to find the one that was requested
  for (int iColor = 0; iColor <= 13; iColor++)
  {
    if (color == colorName[iColor])
    {
      chase(pixels_grbw, pixels_grbw.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
      chase(pixels_grb, pixels_grb.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
      chase(pixels_grb, pixels_grb.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
      chase(pixels_grb, pixels_grb.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
      chase(pixels_grbw, pixels_grbw.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
  }
  return 1;
}

int multiChase(String command) {
  String color = command;
  // Look through the list of colors to find the one that was requested
  for (int iColor = 0; iColor <= 13; iColor++)
  {
    if (color == colorName[iColor])
    {
      multiChase(pixels_grb, pixels_grb.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
  }
  return 1;
}

int lampMode(String command) {
  if (command == "bright") {
    setColor(pixels_grbw, pixels_grbw.Color(255, 255, 255));
    setColor(pixels_grb, pixels_grb.Color(255, 255, 255));
  }
  if (command == "nightlight") {
    setColor(pixels_grbw, pixels_grbw.Color(32, 0, 32));
    setColor(pixels_grb, pixels_grb.Color(32, 0, 32));
  }
  for (int iColor = 0; iColor <= 13; iColor++){
    if (command == colorName[iColor]){
      setColor(pixels_grbw, pixels_grbw.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
      setColor(pixels_grb, pixels_grb.Color(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
  }
    return 1;
}

  // Internal Functions

  int countRings(Adafruit_NeoPixel & strip) {
    int ringsize = 16;
    int ringcount = strip.numPixels() / ringsize;
    if ((strip.numPixels() % ringsize) != 0) {
      return ringcount + 1;
    }
    return ringcount;
  }

  // Test this in the morning
  static void multiChase(Adafruit_NeoPixel & strip, uint32_t c) {
    int lednum = 0;
    for (int m = 0; m < 16; m++) {
      Serial.println(countRings(strip));
      for (int r = 0; r < countRings(strip); r++) {
        lednum = m + (16 * r);
        strip.setPixelColor(lednum, c); // Draw new pixel
        strip.setPixelColor(lednum - 4, 0); // Erase pixel a few steps back
        Serial.print(lednum);
      }
    }
    Serial.println(".");
  }

  static void setColor(Adafruit_NeoPixel & strip, uint32_t c) {
    for (int n = 0; n < strip.numPixels(); n++) {
      strip.setPixelColor(n, c);
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }

  static void setColor(Adafruit_NeoPixel & strip, uint8_t r, uint8_t g, uint8_t b) {
    for (int n = 0; n < strip.numPixels(); n++) {
      strip.setPixelColor(n, strip.Color(r, g, b));
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }

  static void setColor(Adafruit_NeoPixel & strip, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    for (int n = 0; n < strip.numPixels(); n++) {
      strip.setPixelColor(n, strip.Color(r, g, b, w));
      strip.show(); // This sends the updated pixel color to the hardware.
    }
  }

  static void chase(Adafruit_NeoPixel & strip, uint32_t c) {
    for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
      strip.setPixelColor(i, c); // Draw new pixel
      strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
    }
  }

  static void chase(Adafruit_NeoPixel & strip, uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
      strip.setPixelColor(i, strip.Color(r, g, b));
      strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
    }
  }

  static void chase(Adafruit_NeoPixel & strip, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
      strip.setPixelColor(i, strip.Color(r, g, b, w));
      strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
    }
  }
