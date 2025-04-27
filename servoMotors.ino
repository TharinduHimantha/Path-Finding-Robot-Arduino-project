#include <Servo.h>

Servo servo1,servo2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  
  pinMode(3,OUTPUT);
  pinMode(11,OUTPUT);

  servo1.attach(3);
  servo2.attach(11);
  delay(500);

  servo1.write(0);
  servo2.write(180);
  delay(10000);

}

void loop() {
  // put your main code here, to run repeatedly:
   Serial.println(analogRead(A3));
  Serial.println(analogRead(A4));
  Serial.println();
  delay(750);


  for(int c =0; c<=180; c+=5){
    servo1.write(c);
    servo2.write(180-c);
    delay(10);
  }

  servo1.write(0);
  servo2.write(180);
  delay(1000);

}
