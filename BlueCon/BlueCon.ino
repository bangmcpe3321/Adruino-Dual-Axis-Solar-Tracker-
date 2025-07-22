#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// --- (Pin Definitions and other variables are unchanged) ---
// Servo for panning (left/right) - MANUAL CONTROL
Servo panServo;
Servo tiltServo;
const int panServoPin = 2;

// Servo for tilting (up/down) - AUTOMATIC CONTROL
// Servo tiltServo;
const int tiltServoPin = 3;

// LDRs for vertical (tilt) tracking
const int ldrUpPin = A0;
const int ldrDownPin = A1;

// Bluetooth Module Pins (Software Serial)
const int btRxPin = 10;
const int btTxPin = 11;
SoftwareSerial btSerial(btRxPin, btTxPin); // RX, TX

// --- BME280 Sensor Object ---
Adafruit_BME280 bme; // I2C

// --- Tuning & State Variables ---
int servoPanPos = 90;
int servoTiltPos = 90;
int tolerance = 20;
const int servoPanMinAngle = 10;
const int servoPanMaxAngle = 170;
const int servoTiltMinAngle = 10;
const int servoTiltMaxAngle = 170;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  panServo.attach(panServoPin);
  panServo.write(servoPanPos);
  
  tiltServo.attach(tiltServoPin);
  tiltServo.write(servoTiltPos);
  
  Serial.println("Dual-Axis Solar Tracker - On-Demand Data");
  Serial.println("-------------------------------------------");
  Serial.println("Send 'L'/'R' for pan.");
  Serial.println("Send 'G' to request Temp/Humid data.");
  Serial.println("-------------------------------------------");
  
  delay(1000);
}

void loop() {
  // Always check for incoming commands from the app
  handleBluetoothCommands();

  // The automatic tilt control continues to run independently
  handleAutomaticTilt();
  
  // A small delay is still good for system stability
  delay(50); 
}

// Renamed from handleManualPan to handle all commands
void handleBluetoothCommands() {
  if (btSerial.available() > 0) {
    char command = btSerial.read();

    // --- Manual Pan Control ---
    if (command == 'L' || command == 'l') {
      servoPanPos -= 5;
      servoPanPos = constrain(servoPanPos, servoPanMinAngle, servoPanMaxAngle);
      panServo.write(servoPanPos);
    } 
    else if (command == 'R' || command == 'r') {
      servoPanPos += 5;
      servoPanPos = constrain(servoPanPos, servoPanMinAngle, servoPanMaxAngle);
      panServo.write(servoPanPos);
    }
    // --- On-Demand Data Request ---
    else if (command == 'G' || command == 'g') {
      sendSensorData(); // Call the function to read and send data
    }
    
    // Clear the buffer to prevent accidental repeated commands
    while(btSerial.available()) {
      btSerial.read();
    }
  }
}

void handleAutomaticTilt() {
  int upVal = analogRead(ldrUpPin);
  int downVal = analogRead(ldrDownPin);
  int tiltError = upVal - downVal;

  if (tiltError > tolerance) {
    servoTiltPos++; 
  } else if (tiltError < -tolerance) {
    servoTiltPos--;
  }

  servoTiltPos = constrain(servoTiltPos, servoTiltMinAngle, servoTiltMaxAngle);
  tiltServo.write(servoTiltPos);
}

// This function now only runs when called by the command handler
void sendSensorData() {
  // Detach servos to prevent twitching during Bluetooth transmission
  panServo.detach();
  tiltServo.detach();

  // Read sensor values
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();

  // Re-attach servos immediately so they can resume normal operation
  panServo.attach(panServoPin);
  tiltServo.attach(tiltServoPin);
  
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from BME280 sensor!");
    btSerial.println("Error: Failed to read sensor"); // Send error to app
    return;
  }

  // Create the data string
  String dataString = "T:" + String(temp, 2) + ",H:" + String(hum, 2);

  // Send the data string over Bluetooth
  btSerial.println(dataString);

  // Print to Serial Monitor for debugging
  Serial.print("Sent to App on request: ");
  Serial.println(dataString);
}