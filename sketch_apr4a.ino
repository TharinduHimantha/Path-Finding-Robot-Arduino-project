#include <SoftwareSerial.h>

// Define Bluetooth serial pins
SoftwareSerial BTSerial(15, 16); // RX, TX

int trigPin1=12, echoPin1=2;
int trigPin2=19, echoPin2=18;

void setup() {
  // put your setup code here, to run once:
  BTSerial.begin(9600);
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  //delay(11000);

}

void loop() {
  // put your main code here, to run repeatedly:
  int a =  Measure(trigPin1, echoPin1); 
  Serial.println(a);
  BTSerial.println(a);
  delay(50);
  int b = Measure(trigPin2, echoPin2);
  Serial.println(b);
  BTSerial.println(b);
  BTSerial.println("");
  Serial.println("");
  delay(500);

}

int Measure(int trigPin, int echoPin){
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo time
  int duration = pulseIn(echoPin, HIGH, 30000);
  return duration/100;
}
