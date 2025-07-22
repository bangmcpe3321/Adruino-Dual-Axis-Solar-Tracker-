# Arduino Dual-Axis Solar Tracker with Bluetooth Data

This project implements a dual-axis solar tracker using an Arduino. It automatically adjusts its vertical (tilt) position to track a light source and allows for manual horizontal (pan) adjustment via a Bluetooth-enabled application. Additionally, it measures ambient temperature and humidity using a BME280 sensor and sends this data to the user's app on demand.

## Features

- **Automatic Vertical (Tilt) Tracking:** Uses two Light Dependent Resistors (LDRs) to automatically tilt the solar panel towards the brightest light source.
- **Manual Horizontal (Pan) Control:** Control the pan (left/right) movement of the tracker remotely via a Bluetooth application.
- **On-Demand Sensor Data:** Request and receive real-time temperature and humidity data from the onboard BME280 sensor.
- **Bluetooth Communication:** Utilizes a standard Bluetooth module (like HC-05/HC-06) for wireless communication.

## Hardware Requirements

- Arduino Uno (or any compatible board)
- 2 x Standard Servo Motors
- 2 x Light Dependent Resistors (LDRs)
- 1 x BME280 Temperature & Humidity Sensor
- 1 x Bluetooth Module (e.g., HC-05, HC-06)
- Resistors for the LDR voltage dividers (e.g., 2 x 10kΩ)
- Jumper Wires
- Breadboard
- 5V Power Supply

## Software & Libraries

### Arduino
The sketch requires the following libraries. You can install them through the Arduino IDE's Library Manager (`Sketch > Include Library > Manage Libraries...`).

- `<Servo.h>` (Usually included with the Arduino IDE)
- `<SoftwareSerial.h>` (Usually included with the Arduino IDE)
- `<Wire.h>` (Usually included with the Arduino IDE)
- `Adafruit BME280 Library` by Adafruit
- `Adafruit Unified Sensor` by Adafruit

### Android

The project includes an Android application (`adr.apk`) for controlling the tracker and receiving data.

## Wiring & Connections

| Component           | Arduino Pin      |
| ------------------- | ---------------- |
| Pan Servo Signal    | Pin 2            |
| Tilt Servo Signal   | Pin 3            |
| LDR (Upper)         | Pin A0           |
| LDR (Lower)         | Pin A1           |
| Bluetooth Module RX | Pin 10 (Arduino TX) |
| Bluetooth Module TX | Pin 11 (Arduino RX) |
| BME280 SDA          | Pin A4 (SDA)     |
| BME280 SCL          | Pin A5 (SCL)     |

*Note: This project uses the HC-06 Bluetooth module. Remember to connect the power (VCC) and ground (GND) for all components appropriately. The BME280 sensor address used in the code is `0x76`.*

## How It Works

### Automatic Tilt Control
The system reads analog values from two LDRs positioned vertically. By comparing the light intensity values, it calculates an error. If the error exceeds a defined tolerance, the tilt servo moves incrementally to balance the light exposure on both sensors, thus pointing towards the light source.

### Manual Pan & Data Control
The tracker listens for commands sent over Bluetooth:
- **`L` or `l`:** Pans the tracker to the left.
- **`R` or `r`:** Pans the tracker to the right.
- **`G` or `g`:** Triggers the BME280 sensor to read the current temperature and humidity. The data is then formatted as a string (`T:xx.xx,H:yy.yy`) and sent back over Bluetooth.

## Setup & Installation

1.  **Assemble the Hardware:** Connect all the components according to the wiring table above.
2.  **Install Libraries:** Open the Arduino IDE and install the required libraries listed under the "Software & Libraries" section.
3.  **Upload the Sketch:** Open the `BlueCon.ino` file in the Arduino IDE, select your board and port, and upload the code.
4.  **Install the Android App:** Transfer the `adr.apk` file to your Android device and install it. You may need to enable installation from unknown sources in your device settings.
5.  **Connect via App:** Power the circuit. Open the app on your phone, scan for the HC-06 Bluetooth module, and connect to it.

## Usage

Once connected, you can send commands from your app to control the tracker:
- Use buttons or a joystick in your app to send 'L' and 'R' to pan the device.
- The device will automatically handle the vertical tilt.
- Use a "Get Data" button in your app to send 'G' and display the received temperature and humidity.

## Project Files

- `BlueCon.ino`: The main Arduino source code for the project.
- `Ar.png`: Project image or diagram.
- `blocks.png`: Project image or diagram.
