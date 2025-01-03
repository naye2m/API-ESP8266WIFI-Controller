# ESP8266 Wi-Fi Controller API

This project provides an interface for controlling devices using an ESP8266 Wi-Fi module. 
It includes the necessary code, configuration files, and web assets to set up and manage 
an IoT device using HTTP-based communication.

## Features

- **Wi-Fi Configuration**: Manage Wi-Fi settings for the ESP8266.
- **HTTP Request Handling**: Send and receive HTTP requests to control the device.
- **Web Interface**: An included HTML file provides a user-friendly interface.
- **QR Code Integration**: Generate or process QR codes for device setup or access.
- **Image Display on ESP**: 
  - Load images from the web interface and display them on the ESP's display.
  - Uses custom compression algorithms to compress and transfer images, achieving speeds of up to 150ms per image.
- **Adaptive Network Hosting**: 
  - Automatically attempts to connect to a router's Wi-Fi network.
  - Hosts the web interface on the router's network if available.
  - Falls back to hosting the web interface on the ESP's own network if no router connection is detected.
- **Image Management**: 
  - Save, edit, and modify images directly through the web interface for seamless display on the ESP device.

## File Structure

- `.gitignore`: Defines files to be ignored by Git.
- `APIcontroller_2.0.ino`: Arduino sketch for the ESP8266.
- `___qrcode.h`: QR code-related functionality.
- `build.sh`: Build or deployment script.
- `helper.h`: Helper functions for modular code management.
- `httpreqfunc.h`: Handles HTTP request and response functionality.
- `httptext.h`: Contains constants or text for HTTP communication.
- `index.html`: Web-based user interface for device control.
- `wifiConfig.h`: Configuration for managing Wi-Fi settings.

## Requirements

- **Hardware**: ESP8266 Wi-Fi module or compatible device. ( I am using esp8266 )
- **Software**: Arduino IDE or compatible development environment.
- **Dependencies**: Ensure all necessary libraries for HTTP, QR code generation, and Wi-Fi are installed.

## Getting Started

1. Clone the repository or download the project files.
2. Open `APIcontroller_2.0.ino` in the Arduino IDE.
3. Modify `wifiConfig.h` with your Wi-Fi credentials.
4. Upload the sketch to your ESP8266 device.
5. Access the web interface by navigating to the device's IP address.

## Build Instructions

Use the `build.sh` script for automating the website build and minifing process:
```bash
./build.sh
```

## License
This project is open-source and available under the MIT License. 
