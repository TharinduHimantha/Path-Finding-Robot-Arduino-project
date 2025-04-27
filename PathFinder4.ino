#include <Servo.h>
#include <stdlib.h>
#include <SoftwareSerial.h>

// Define Bluetooth serial pins
SoftwareSerial BTSerial(15, 16); // RX, TX

int myVariable = 0;
String inputString = "";  // To store incoming characters
bool inputComplete = false; // Flag when input is complete
int bluetoothSwitch = 13;

//Servo related
Servo servo1,servo2;
short servoPin1=3, servoPin2=11;

// Motor related
short leftF=6, leftB=7;  // turn right
short rightF=8, rightB=9;  //turn left
short pwmPinL=5;
short pwmPinR=10;
short startPin;

int baseSpeed1=150;

//UltraSound related
int trigPin1=12, echoPin1=2;
int trigPin2=19, echoPin2=18;


//pid related
double Kp = 0.01;


void setup() {
  // put your setup code here, to run once:

//bluetooth
  BTSerial.begin(9600);
  pinMode(bluetoothSwitch, INPUT);

//Motor related
  pinMode(leftF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightF, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(pwmPinL, OUTPUT);
  pinMode(pwmPinR, OUTPUT);

  digitalWrite(leftF, 1);
  digitalWrite(leftB, 0);

  digitalWrite(rightF, 1);
  digitalWrite(rightB, 0);

//Servo related
 pinMode(servoPin1,OUTPUT);
  pinMode(servoPin2,OUTPUT);

  servo1.attach(servoPin1);    //left
  servo2.attach(servoPin2);
  delay(500);

  servo1.write(0);
  servo2.write(180);
  delay(3000);

//Ultrasound related
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);    //left
  pinMode(echoPin2, INPUT);

  while( digitalRead(bluetoothSwitch) == 1){
    goForward(0, 0);
    delay(3000);
  }


}



void loop() {
  // put your main code here, to run repeatedly:

  int correction = 0;
  if( digitalRead(bluetoothSwitch) == 1 ){
    goForward(0, correction);
    receiveBluetoothInt();
  }

  if( Measure(trigPin2, echoPin2) < 10){
      correction = Scan();
  }

  goForward(baseSpeed1, correction);


}

int PID(int error){
  BTSerial.println(Kp*error);
  return Kp * error;
}

int Scan(){
  int error= 0;
  for(int c=0; c<=120; c+=5){

    if(c%30==0){
      servo1.write(c);
      error += Measure(trigPin2, echoPin2)*c;
      delay(40);
      servo2.write(180-c);
      error += Measure(trigPin1, echoPin1)*(-c);
      delay(40);
    }
    else{
      servo1.write(c);
      servo2.write(180-c);
      delay(5);
    }

    
  }

  servo1.write(0);
  servo2.write(180);

  return PID(error);

}


int Measure(int trigPin, int echoPin){
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo time
  int duration = pulseIn(echoPin, HIGH, 30000)/100;
  duration = (duration > 15) ? 15 : duration;
  return duration;
}



void goForward(int baseSpeed, int correction){

  analogWrite(pwmPinR, baseSpeed - correction);
  BTSerial.print("Lefft :");
  BTSerial.println(baseSpeed- correction);
  int speed = (baseSpeed != 0) ? baseSpeed+15 : 0;
  analogWrite(pwmPinL, speed + correction);
  BTSerial.print("Right :");
  BTSerial.println(speed + correction);
  
}

void receiveBluetoothInt() {
  inputString = "";           // Reset the input
  inputComplete = false;

  // Collect characters until newline
  while (true) {
    if (BTSerial.available()) {
      char incomingChar = BTSerial.read();
      if (incomingChar == '\n' || incomingChar == '\r') {
        inputComplete = true;
        break;
      } else {
        inputString += incomingChar;
      }
    }
  }

  if (inputComplete) {
    Kp = inputString.toInt()/100; // Convert received String to int
  }
}
