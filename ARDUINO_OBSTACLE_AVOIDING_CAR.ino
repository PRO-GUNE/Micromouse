
//ARDUINO OBSTACLE AVOIDING CAR//
// Before uploading the code you have to install the necessary library//
//AFMotor Library https://learn.adafruit.com/adafruit-motor-shield/library-install //
//NewPing Library https://github.com/livetronic/Arduino-NewPing// 
//Servo Library https://github.com/arduino-libraries/Servo.git //
// To Install the libraries go to sketch >> Include Library >> Add .ZIP File >> Select the Downloaded ZIP files From the Above links //


#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 400 
#define MAX_SPEED 140 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;   

boolean goesFast=false;
boolean goesSlow=false;
int distance = 100;
int speedSet = 0;

void setup() {
Serial.begin(9600);
  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {
 
Serial.println(distance);  
 int distanceR = 0;
 int distanceL =  0;
 delay(5);

if((distance<29) && (distance >17))
 {  
  moveSlow();

   }
else if(distance<=17)
 {
  moveStop();
  delay(100);
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  Serial.println(distanceR);
  delay(200);
  distanceL = lookLeft();
  Serial.println(distanceL);
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();    
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }
else
 {
  moveForward();
 }
distance = readPing();
}
int lookRight()
{
    myservo.write(50); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    Serial.print("RIGHT: ");
    Serial.println(distance);   
    return distance;
    delay(100);
}

int lookLeft()
{
    myservo.write(170); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115);
    Serial.print("LEFT: ");
    Serial.println(distance);   
    return distance;
    delay(100);
}

int readPing() { 
  delay(5);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
  
}

void moveStop() {
    motor1.run(RELEASE);
    motor1.setSpeed(0);
    motor2.run(RELEASE);
    motor2.setSpeed(0);
    motor3.run(RELEASE);
    motor3.setSpeed(0);
    motor4.run(RELEASE);
    motor4.setSpeed(0);
    Serial.println("STOP"); 
    delay(200);
  } 
  
void moveSlow() {

 if(!goesSlow)
  {
    goesSlow=true;
    goesFast=false;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);    
   Serial.println("SLOW"); 
   for (speedSet = 0; speedSet < 100; speedSet +=4) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
      for (speedSet = 140; speedSet > 100; speedSet -=20) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}


void moveForward() {

 if(!goesFast)
  {
    goesFast=true;
    goesSlow=false;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);    
    Serial.println("FAST"); 
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
    for (speedSet = 100; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}

void moveBackward() {
    goesFast=false;
    goesSlow=false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
    Serial.println("BACKWARD");
  for (speedSet = 0; speedSet < 100; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
  delay(200);
}  
 
void turnRight() {
  motor1.run(BACKWARD);

    motor2.run(FORWARD);

    motor3.run(FORWARD);

    motor4.run(BACKWARD);

 for (speedSet = 0; speedSet < 160; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }   
  Serial.println("RIGHT");
  delay(400);

} 
 
void turnLeft() {
  motor1.run(FORWARD);

    motor2.run(BACKWARD);

    motor3.run(BACKWARD);

    motor4.run(FORWARD);

 for (speedSet = 0; speedSet < 160; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }   
  Serial.println("LEFT");
  delay(400);
}  
