#include <Keyboard.h>

int motorAnode1 = 6;
int motorCathode1 = 9;
int motorAnode2 = 10;
int motorCathode2 = 11;
bool running = true;
int INTR_T = 500;
bool doing;
int maxSpeed = 220;
int turnSpeed = 80;

void setup() {
  Serial.begin(9600);
  pinMode(motorAnode1, OUTPUT);
  pinMode(motorCathode1, OUTPUT);
  pinMode(motorAnode2, OUTPUT);
  pinMode(motorCathode2, OUTPUT);
  doing = false;
}

void loop() {
  if(Serial.available() > 0 && !doing){
    char instr = Serial.read();
    Serial.println(instr);
    switch(instr){
      case 'a':
        goForward(INTR_T);
        break;
      case 'b':
        turnLeft(INTR_T / 2);
        break;
      case 'd':
        turnRight(INTR_T / 2);
        break;
      case 'e':
        goBackward(INTR_T);
        break;
      default:
        Serial.println("Unknown command");
    }
  }
}

void goForward(int time){
  doing = true;
  setMotors(maxSpeed, maxSpeed);
  delay(time);
  halt();
  doing = false;
}

void goBackward(int time){
  doing = true;
  setMotors(-maxSpeed, -maxSpeed);
  delay(time);
  halt();
  doing = false;
}

void turnLeft(int time){
  doing = true;
  setMotors(maxSpeed, turnSpeed);
  delay(time);
  halt();
  doing = false;
}

void turnRight(int time){
  doing = true;
  setMotors(turnSpeed, maxSpeed);
  delay(time);
  halt();
  doing = false;
}
void halt(){
  setMotors(0, 0);
}

void halt(int time){
  halt();
  delay(time);
}

void setMotors(int motor1, int motor2){
  if(motor1 > 0){
    analogWrite(motorAnode1, motor1);
    analogWrite(motorCathode1, 0);
  }else{
    analogWrite(motorAnode1, 0);
    analogWrite(motorCathode1, -motor1);
  }
  if(motor2 > 0){
    analogWrite(motorAnode2, 0);
    analogWrite(motorCathode2, motor2);
  }else{
    analogWrite(motorAnode2, -motor2);
    analogWrite(motorCathode2, 0);
  }
}

