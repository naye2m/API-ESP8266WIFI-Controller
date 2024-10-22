#include <string>
#include "c_types.h"
#include "HardwareSerial.h"
#include "./helper.h"

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
    for (int x = SCREEN_WIDTH; x >= -int(text.length() * 6 * size); x -= speed) {
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

void mainDecoder(String binData, int length)
/*
  todo : add functinalities for chenging behavior of zeros and ones
 */
{
  int width = 128;   // Width of the display
  int height = 64;   // Height of the display
  int x = 0, y = 0;  // Starting positions for the display

  // Initialize the display
  display.clearDisplay();  // Clear display before rendering

  // Inner function to update display pixel by pixel
  // auto innerFunction = [&](int xPos, int yPos, int pixelState) {
  //   display.drawPixel(xPos, yPos, pixelState);  // Draw pixel in white
  //   // todo
  //   // if (pixelState == 1)
  //   // {
  //   // display.drawPixel(xPos, yPos, WHITE);  // Draw pixel in white
  //   // }
  //   // else
  //   // {
  //   //   display.drawPixel(xPos, yPos, BLACK);  // Draw pixel in black
  //   // }
  // };

  // Loop through the binary data byte by byte
  for (int i = 0; i < length; i++) {
    char byte = binData[i];  // Get the current byte
    for (int bit = 0; bit < 8; bit++) {
      int pixelState = (byte >> (7 - bit)) & 1;  // Extract each bit
      // innerFunction(x, y, pixelState);          // Update the display
      display.drawPixel(x, y, pixelState);  // Update the display

      x++;
      if (x >= width) {
        x = 0;
        y++;
        if (y >= height) {
          break;  // Stop if we exceed display bounds
        }
      }
    }
  }

  display.display();  // Render the display
}
void handleByteArr2Display() {
  /*
  todo : add padding option for skipping bytes start and end
  todo : add multiple way to display like pos's, st end points
   */
  if (!server.hasArg("data"))
    // Send an error if no data received
    server.send(400, "text/plain", "No bytearr data received");
  Serial.println("////////st/////////////////////////////////////////////////////////");

  uint8_t skipZeros = server.hasArg("skip0s") ? 1 : 0;
  uint8_t _0 = server.hasArg("_0") ? server.arg("_0").toInt() : BLACK;
  uint8_t _1 = server.hasArg("_1") ? server.arg("_1").toInt() : WHITE;
  // todo default escape 16
  uint8_t escape_char = (uint8_t) (server.hasArg("escape_char") ? server.arg("escape_char").toInt() & 0xff : 16);
  // Convert the UTF-8 string from server.arg() to Latin-1
  std::string utf8_data = server.arg("data").c_str();  // Get the argument as std::string

  // Convert to ISO-8859-1 (Latin-1) using a function that returns std::string
  std::string latin1_data = utf8_to_iso_8859_1(utf8_data);  // Ensure this returns std::string

  // Convert std::string to Arduino String
  String c_input = latin1_data.c_str();
  String input = String(c_input);  // Use c_str() to get the C-style string

  // Compute array size (min of input length or screen dimensions)
  // int arraySize = SCREEN_WIDTH * SCREEN_HEIGHT / 8;
  int arraySize = server.hasArg("fullscreen") ? SCREEN_WIDTH * SCREEN_HEIGHT / 8 : min((int)input.length(), (int)SCREEN_WIDTH * SCREEN_HEIGHT / 8);

  // Only clear the display if we have data to process
  if (server.hasArg("clear"))
    display.clearDisplay();

  for (int i = 0; i < arraySize; i++) {
    uint8_t px_byte = c_input[i];
    if (px_byte == escape_char)
      continue;
    if (px_byte != 0) {
      Serial.printf("\n bytelok(i):%i, px_byte: 0x%x %b \"%c\"\n", i, px_byte, px_byte, px_byte);
    }
    for (int jx = 0; jx < 8; jx++) {
      int j = (i * 8) + jx;
      int x = j % SCREEN_WIDTH;
      int y = j / SCREEN_WIDTH;
      // Draw the pixel based on input (directly map to display)
      if (!skipZeros) {
        if ((px_byte >> (7 - jx)) & 1) {
          display.drawPixel(x, y, _1);
          Serial.printf("\n x:%i y:%i px:0x%x %c\n", x, y, px_byte, px_byte);
        } else {
          display.drawPixel(x, y, _0);
          Serial.print(0);
        }
      } else if ((px_byte >> (7 - jx)) & 1)
        display.drawPixel(x, y, _1);
    }
  }

  // Update the display once, after all pixels are drawn
  display.display();

  Serial.println(input);
  Serial.println(input.length());
  // Send success response
  server.send(200, "text/plain", "Printed bytearr to disp");
  Serial.println("////////end/////////////////////////////////////////////////////////");
  // server.send(200, "text/plain", "2D array applied to display");
}

void set_ByteArray_to_Display() {
  if (!server.hasArg("ByteArray")) {
    server.send(400, "text/plain", "ByteArray required");
    return;
  }
  mainDecoder(server.arg("ByteArray"), (64 * 128 / 8));
  server.send(200, "text/plain", "ByteArray displayed");
}

void print2Serial() {
  if (!server.hasArg("text")) {
    server.send(400, "text/plain", "text required");
    return;
  }
  if (!Serial)
    Serial.begin(115200);
  Serial.println(server.arg("text"));
  server.send(200, "text/plain", "text displayed in serial 115200");
}
// Function to handle function routing
void func() {
  if (!server.hasArg("func")) {
    server.send(400, "text/plain", "arg func required");
    return;
  }
  const String func_name = server.arg("func");
  const char *funcStr = func_name.c_str();  // Convert String to C-style string once for efficiency

  if (strcmp(funcStr, "disp") == 0) {
    disp2();
  } else if (strcmp(funcStr, "ledblink") == 0) {
    handleLEDBlink2();
  } else if (strcmp(funcStr, "ledbrightness") == 0) {
    handleLEDBrightness2();
  } else if (strcmp(funcStr, "pixel") == 0) {
    handlePixel2();
  } else if (strcmp(funcStr, "shape") == 0) {
    handleShape2();
  } else if (strcmp(funcStr, "animateText") == 0) {
    animateText2();
  } else if (strcmp(funcStr, "2darray") == 0) {
    handle2DArray2();
  } else if (strcmp(funcStr, "clearbeforerender") == 0) {
    clearBeforeRender2();
  } else if (strcmp(funcStr, "qr") == 0) {
    qr();
  } else if (strcmp(funcStr, "bytearray") == 0) {
    set_ByteArray_to_Display();
  } else if (strcmp(funcStr, "serialprint") == 0) {
    print2Serial();
  } else if (strcmp(funcStr, "bytearr") == 0) {
    handleByteArr2Display();
  } else {
    server.send(400, "text/plain", "Unknown function: " + func_name);
  }
}
