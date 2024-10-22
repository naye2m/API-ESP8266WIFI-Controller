#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#ifndef WiFi_h
#include <ESP8266WiFi.h>
#endif

#include <../../APIcontroller_2.0/httptext.h>
#include <../../APIcontroller_2.0/wifiConfig.h>



// Create a web server on port 80
ESP8266WebServer server(80);

// OLED Display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



// QRcode qrcode (&display);
// Pin for the built-in LED (GPIO2)
const int ledPin = 2;

// 128x64 2D array for display control
bool displayArray[SCREEN_WIDTH][SCREEN_HEIGHT];
void func();
// Setup function to initialize the board
void setup() {

  Serial.begin(115200);

  // Setup LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // Turn off LED initially

  // Initialize the OLED display
  Wire.begin(14, 12);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Loop forever if display fails to initialize
  }
  display.setTextSize(2);  // Set font size
  display.setTextColor(WHITE);

  // display.invertDisplay(true)
  // display.dim(true)
  // display.
  // display.
  for (int i = 0; i < 1 * 60; i++) {
    display.clearDisplay();
    display.setCursor(64, 32);
    display.print("Hello\n");
    display.setRotation(i);
    // display;
    display.display();
    delay(1000 / 60);
  }
  display.setRotation(0);
  display.clearDisplay();
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);  // Set font size
  display.setTextColor(WHITE);
  display.setCursor(0, 0);     // Set text position
  display.print("#Booting#");  // Print the received message
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();

  // Connect to WiFi
  display.setCursor(0, 0);              // Set text position
  display.print("#WIFI#\nconnecting");  // Print the received message
  display.display();

  IPAddress Self_IPv4 = connectWIFI();
  Serial.println("IP ADDRESS : ");
  Serial.println(Self_IPv4);
  Serial.println("Connecting to WiFi...");

  delay(1000);
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);             // Set text position
  display.print("#WIFI#\nconnected");  // Print the received message
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  Serial.println("WiFi connected");
  Serial.println(Self_IPv4);  // Print the IP address



  // API Endpoints
  server.on("/", handleRoot);                                     // Default route
  server.on("/cleanDisplay", cleanDisplay);                       // Clean display
  server.on("/led/on", handleLEDOn);                              // Turn on LED
  server.on("/led/off", handleLEDOff);                            // Turn off LED
  server.on("/led/blink", handleLEDBlink);                        // Blink LED
  server.on("/led/brightness", handleLEDBrightness);              // Adjust LED brightness (PWM)
  server.on("/display", HTTP_POST, handleDisplay);                // Display text (POST)
  server.on("/pixel", HTTP_POST, handlePixel);                    // Set individual pixel
  server.on("/shape", HTTP_POST, handleShape);                    // Draw shape (lines, rectangles, etc.)
  server.on("/animateText", HTTP_POST, animateText);              // Animate text
  server.on("/setArray", HTTP_POST, handle2DArray);               // Set 2D array for display control
  server.on("/clearBeforeRender", HTTP_POST, clearBeforeRender);  // Option to clear before rendering
  server.on("/func", HTTP_POST, func);                            // Option to clear before rendering

  // Start the server
  display.clearDisplay();
  server.begin();
  Serial.println("Server started");
  display.setCursor(0, 0);               // Set text position
  display.print("#Server#\n#started#");  // Print the received message
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);                              // Set text position
                                                        // int drot = display.getRotation();  //0
  display.setTextSize(1);                               // Set font size
  display.clearDisplay();                               //
  display.print("WIFI\nconnected\n\nIPv4 (local):\n");  // Print the IP addr
  display.print(Self_IPv4);                             // Print the IP addr
  display.print("\n\n\n      Credit:");                 //f
  display.print("@naye2m");                             // Print the IP addr
  display.display();
  delay(1000);
}
#include <../../APIcontroller_2.0/httpreqfunc.h>

// Loop function to handle HTTP requests
void loop() {
  server.handleClient();  // Handle incoming client requests
}

// Root handler
void handleRoot() {
  server.send(200, "text/html", homeHTML);
  cleanDisplay();
}

// Handle turning on the LED
void handleLEDOn() {
  digitalWrite(ledPin, LOW);  // LED on
  server.send(200, "text/plain", "LED is ON");
}

// Handle turning off the LED
void handleLEDOff() {
  digitalWrite(ledPin, HIGH);  // LED off
  server.send(200, "text/plain", "LED is OFF");
}

// Handle LED blinking
void handleLEDBlink() {
  int times = 5;
  if (server.hasArg("times")) {
    times = server.arg("times").toInt();
  }

  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);
  }
  server.send(200, "text/plain", "LED blinked " + String(times) + " times");
}

// Handle LED brightness (PWM control)
void handleLEDBrightness() {
  if (server.hasArg("level")) {
    int brightness = server.arg("level").toInt();  // Accept brightness level (0-255)
    analogWrite(ledPin, brightness);
    server.send(200, "text/plain", "LED brightness set to " + String(brightness));
  } else {
    server.send(400, "text/plain", "Brightness level not provided");
  }
}

// Handle displaying text on OLED
void handleDisplay() {
  if (!server.hasArg("plain")) {  // Check if body exists
    server.send(400, "text/plain", "No message received");
    return;
  }

  String message = server.arg("plain");  // Get the text from the request

  int size = 1;
  int x = 0;
  int y = 10;

  if (server.hasArg("size")) size = server.arg("size").toInt();
  if (server.hasArg("x")) x = server.arg("x").toInt();
  if (server.hasArg("y")) y = server.arg("y").toInt();

  display.clearDisplay();
  display.setTextSize(size);  // Set font size
  display.setTextColor(WHITE);
  display.setCursor(x, y);  // Set text position
  display.print(message);   // Print the received message
  display.display();
  server.send(200, "text/plain", "Message displayed: " + message);
}

void cleanDisplay() {
  // no data send for work as helper function
  display.clearDisplay();
  display.display();
  // server.send(200, "text/plain", "Message displayed: " + message);
}

// Handle drawing an individual pixel
void handlePixel() {
  if (server.hasArg("x") && server.hasArg("y") && server.hasArg("state")) {
    int x = server.arg("x").toInt();
    int y = server.arg("y").toInt();
    int state = server.arg("state").toInt();  // 0 = off, 1 = on

    display.drawPixel(x, y, state ? WHITE : BLACK);
    display.display();
    server.send(200, "text/plain", "Pixel (" + String(x) + ", " + String(y) + ") set to " + String(state));
  } else {
    server.send(400, "text/plain", "x, y, and state required");
  }
}

// Handle drawing shapes (rectangles, circles)
void handleShape() {
  if (server.hasArg("type")) {
    String type = server.arg("type");

    if (type == "rect" && server.hasArg("x") && server.hasArg("y") && server.hasArg("w") && server.hasArg("h")) {
      int x = server.arg("x").toInt();
      int y = server.arg("y").toInt();
      int w = server.arg("w").toInt();
      int h = server.arg("h").toInt();

      display.drawRect(x, y, w, h, WHITE);
      display.display();
      server.send(200, "text/plain", "Rectangle drawn at (" + String(x) + ", " + String(y) + ") with size (" + String(w) + ", " + String(h) + ")");
    } else if (type == "circle" && server.hasArg("x") && server.hasArg("y") && server.hasArg("r")) {
      int x = server.arg("x").toInt();
      int y = server.arg("y").toInt();
      int r = server.arg("r").toInt();

      display.drawCircle(x, y, r, WHITE);
      display.display();
      server.send(200, "text/plain", "Circle drawn at (" + String(x) + ", " + String(y) + ") with radius " + String(r));
    } else {
      server.send(400, "text/plain", "Invalid shape parameters");
    }
  } else {
    server.send(400, "text/plain", "Shape type required");
  }
}

// Animate text by moving it across the display
void animateText() {
  if (!server.hasArg("text")) {
    server.send(400, "text/plain", "Text required");
    return;
  }

  String text = server.arg("text");
  int size = server.hasArg("size") ? server.arg("size").toInt() : 1;
  int y = server.hasArg("y") ? server.arg("y").toInt() : 10;
  int repeats = server.hasArg("repeats") ? server.arg("repeats").toInt() : 1;

  for (int r = 0; r < repeats; r++) {
    for (int x = SCREEN_WIDTH; x >= -int(text.length() * 6 * size); x--) {
      display.clearDisplay();
      display.setTextSize(size);
      display.setCursor(x, y);
      display.print(text);
      display.display();
      delay(100);  // Adjust for desired speed
    }
  }

  server.send(200, "text/plain", "Text animation completed");
}

// Handle 2D array for display control
void handle2DArray() {
  if (server.hasArg("plain")) {
    String input = server.arg("plain");
    int arraySize = min((int)input.length(), (int)((long)SCREEN_WIDTH * (long)SCREEN_HEIGHT));

    for (int i = 0; i < arraySize; i++) {
      int x = i % SCREEN_WIDTH;
      int y = i / SCREEN_WIDTH;
      displayArray[x][y] = (input[i] == '1');
    }

    display.clearDisplay();
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      for (int y = 0; y < SCREEN_HEIGHT; y++) {
        if (displayArray[x][y]) {
          display.drawPixel(x, y, WHITE);
        }
      }
    }
    display.display();
    server.send(200, "text/plain", "2D array applied to display");
  } else {
    server.send(400, "text/plain", "No array data received");
  }
}

// Clear before rendering new content
void clearBeforeRender() {
  bool clearBefore = server.hasArg("clear") ? (server.arg("clear") == "true") : false;
  if (clearBefore) {
    display.clearDisplay();
    display.display();
    server.send(200, "text/plain", "Display cleared before rendering");
  } else {
    server.send(400, "text/plain", "Invalid clear flag");
  }
}
