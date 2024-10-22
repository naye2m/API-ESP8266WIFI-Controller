#ifndef WiFi_h
#include <ESP8266WiFi.h>
#endif
// WiFi Configuration
// const char* ssid = "Shuvo";
// const char* password = "shuvo2009";
const char* ssid = "NAYEEM__0";
const char* password = "11111111";
IPAddress connectWIFI() {
  IPAddress Self_IPv4;
  WiFi.begin(ssid, password);
  delay(2000);
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(5000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.softAP("Hacker hei bhai hacker hei", "");  // You can also omit the password for an open network
    delay(1000);
    Self_IPv4 = WiFi.softAPIP();
    while (WiFi.status() != WL_CONNECTED) continue;
  } else
    Self_IPv4 = WiFi.localIP();
  return Self_IPv4;
}