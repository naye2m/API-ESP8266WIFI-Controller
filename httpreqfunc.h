
// display.drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
// display.drawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color)
// display.drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
// display.drawPixel
// display.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
// display.drawChar()
// display.drawBitmap()
// display.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
// display.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
// display.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
// display.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
// display.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
// display.fillScreen(uint16_t color)
// display.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
// display.writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)

// Initialize server and display objects
// ESP8266WebServer server(80);
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function to handle text display on OLED
void disp2() {
  String message = server.hasArg("text") ? server.arg("text") : "";
  int size = server.hasArg("size") ? server.arg("size").toInt() : 1;
  int x = server.hasArg("x") ? server.arg("x").toInt() : 0;
  int y = server.hasArg("y") ? server.arg("y").toInt() : 10;
  int color = server.hasArg("color") ? server.arg("color").toInt() : WHITE;

  if (server.hasArg("clear"))
    display.clearDisplay();

  if (server.hasArg("r"))
    display.setRotation(server.arg("r").toInt() | 0);
  if (server.hasArg("size"))
    display.setTextSize(size);
  if (server.hasArg("color"))
    display.setTextColor(color);
  if (server.hasArg("x") && server.hasArg("y"))
    // display.setTextColor(color);
    display.setCursor(x, y);
  display.print(message);
  display.display();
  server.send(200, "text/plain", "Message displayed: " + message);
}


void qr() {
  if (!server.hasArg("text"))
    server.send(400, "text/plain", "qr is not define");
  String text = server.arg("text");
  // Qr(text);
  server.send(200, "text/plain", "Qr is oot available");
}


// Function to handle LED blinking
void handleLEDBlink2() {
  int times = server.hasArg("times") ? server.arg("times").toInt() : 5;
  int dlow = server.hasArg("dlow") ? server.arg("dlow").toInt() : 500;
  int dhigh = server.hasArg("dhigh") ? server.arg("dhigh").toInt() : 500;

  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, LOW);
    delay(dlow);
    digitalWrite(ledPin, HIGH);
    delay(dhigh);
    delay(500);  // Extra delay between blinks
  }
  server.send(200, "text/plain", "LED blinked " + String(times) + " times");
}

// Function to handle LED brightness
void handleLEDBrightness2() {
  if (server.hasArg("level")) {
    int brightness = server.arg("level").toInt();
    analogWrite(ledPin, brightness);
    server.send(200, "text/plain", "LED brightness set to " + String(brightness));
  } else {
    server.send(400, "text/plain", "Brightness level not provided");
  }
}

// Function to handle drawing an individual pixel
void handlePixel2() {
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

// Function to handle drawing shapes (rectangles, circles)
void handleShape2() {
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

// Function to animate text by moving it across the display
void animateText2() {
  if (!server.hasArg("text")) {
    server.send(400, "text/plain", "Text required");
    return;
  }

  String text = server.arg("text");
  int size = server.hasArg("size") ? server.arg("size").toInt() : 1;
  int speed = server.hasArg("speed") ? server.arg("speed").toInt() : 10;
  int y = server.hasArg("y") ? server.arg("y").toInt() : 10;
  int repeats = server.hasArg("repeats") ? server.arg("repeats").toInt() : 1;

  for (int r = 0; r < repeats; r++) {
    for (int x = SCREEN_WIDTH; x >= -int(text.length() * 6 * size); x-= speed) {
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

// Function to handle 2D array for display control
void handle2DArray2() {
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

// Function to clear the display before rendering new content
void clearBeforeRender2() {
  bool clearBefore = server.hasArg("clear") ? (server.arg("clear") == "true") : false;
  if (clearBefore) {
    display.clearDisplay();
    display.display();
    server.send(200, "text/plain", "Display cleared before rendering");
  } else {
    server.send(400, "text/plain", "Invalid clear flag");
  }
}

// Function to handle function routing
void func() {
  if (!server.hasArg("func")) {  // Check if the func argument exists
    server.send(400, "text/plain", "No func received");
    return;
  }

  String func = server.arg("func");

  if (func == "disp") {
    disp2();
  } else if (func == "ledBlink") {
    handleLEDBlink2();
  } else if (func == "ledBrightness") {
    handleLEDBrightness2();
  } else if (func == "pixel") {
    handlePixel2();
  } else if (func == "shape") {
    handleShape2();
  } else if (func == "animateText") {
    animateText2();
  } else if (func == "2DArray") {
    handle2DArray2();
  } else if (func == "clearBeforeRender") {
    clearBeforeRender2();
  } else if (func == "qr") {
    qr();
  } else {
    server.send(400, "text/plain", "Unknown function: " + func);
  }
}


// void setup() {
//   // Initialize LED pin
//   pinMode(ledPin, OUTPUT);

//   // Initialize the display
//   if (!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
//     Serial.println(F("SSD1306 allocation failed"));
//     for (;;);  // Infinite loop
//   }
//   display.display();
//   delay(2000);  // Pause for 2 seconds

//   // Initialize server routes
//   server.on("/func", HTTP_POST, func2);
//   server.begin();
// }

// void loop() {
//   server.handleClient();
// }
