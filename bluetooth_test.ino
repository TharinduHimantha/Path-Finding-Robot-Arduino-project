#include <SoftwareSerial.h>

// Define Bluetooth serial pins
SoftwareSerial BTSerial(15, 16); // RX, TX

int myVariable = 0;
String inputString = "";  // To store incoming characters
bool inputComplete = false; // Flag when input is complete

void setup() {
  Serial.begin(9600);       // For USB Serial Monitor
  BTSerial.begin(9600);     // Bluetooth HC-05 baud rate
  Serial.println("Waiting for Bluetooth integer input...");
}

void loop() {
  // Check if data is available
  while (BTSerial.available()) {
    char incomingChar = BTSerial.read();
    if (incomingChar == '\n' || incomingChar == '\r') {
      inputComplete = true;  // End of input
    } else {
      inputString += incomingChar;  // Build input string
    }
  }

  if (inputComplete) {
    myVariable = inputString.toInt(); // Convert string to integer
    Serial.print("Received integer: ");
    Serial.println(myVariable);

    // Reset for next input
    inputString = "";
    inputComplete = false;
  }



  delay(50);  // Small delay for stability
}
