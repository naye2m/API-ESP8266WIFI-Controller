#include <ESP8266WiFi.h>

// WiFi Configuration
const char* ssid = "Shuvo";
const char* password = "shuvo2009";
IPAddress connectWIFI() {
  IPAddress Self_IPv4;
  WiFi.begin(ssid, password);

  delay(5000);
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.softAP("Hacker hei bhai hacker hei", "");  // You can also omit the password for an open network
    delay(1000);
    Self_IPv4 = WiFi.softAPIP();
  } else
    Self_IPv4 = WiFi.localIP();

  // Serial.println(Self_IPv4);
  // Serial.println("Connecting...");
  return Self_IPv4;
}