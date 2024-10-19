/* *********************************************************************************
 * ESP8266 QRcode
 * dependency library :
 *   ESP8266 Oled Driver for SSD1306 display by Daniel Eichborn, Fabrice Weinberg
 *
 * SDA --> D6
 * SCL --> D7
***********************************************************************************/



// SSD1306  display(0x3c, &Wire);
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
namespace QR__{

#include <qrcode.h>
#include <SSD1306.h>
void Qr(String text) {

  SSD1306 display(SCREEN_ADDRESS, 14, 12);
  QRcode qrcode(&display);
  display.init();
  display.clear();
  // display.begin();
  // display.clearDisplay();
  display.display();


  // enable debug qrcode
  qrcode.debug();

  // Initialize QRcode display using library
  qrcode.init();
  // create qrcode
  qrcode.create(text);
  // display.invertDisplay();
}
}