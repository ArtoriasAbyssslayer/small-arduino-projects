/*
 Demo of Sensor Network Mangement with Arduino 
 Ultrasoninc Sensor HC-SR04
 Tilt sensor
 Editor :  Harry Filis
*/
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <NewPing.h>
#include <stdio.h>
#include <stdlib.h>
#define NOTE_F3  175
#define NOTE_G3  196
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define redPin 11  // attach pin d11 Arduino to pin 
#define bluePin 9  
#define greenPin 10
#define tiltSensorPin A0
#define passiveBuzzerPin 5 //controls the gate of the transistor
#define max_dist 400    
/*Initialize lcd with i2c*/
LiquidCrystal_I2C lcd(0x27, A4, A5);
/*Initialize the NewPing object named Sonar*/
NewPing Sonar(trigPin,echoPin,max_dist);
/*Initialize servo motor object*/
Servo myServo;
void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(tiltSensorPin,INPUT);
  pinMode(passiveBuzzerPin,OUTPUT);
  Serial.begin(9600);
  Serial.println("Sensor Network Demo with Arduino UNO");
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  myServo.attach(6);
}
/*----------------------------READ_Sesnor_Dist function unrolling ~ Sonar.pingcm() --------------------------------
class Sonar{
  ........
}

long duration;
int distance;
int Sonar :: ping_cm() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  return distance;
}

-------------------------------------------------------------Unrolling end--------------------------
*/


int Read_Sensor_Dist(){
    //return Sonar.ping_cm();
  long duration;
  int distance;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  return distance;
}
//function that set Color of rgb led
void RGB_Color(int r, int g, int b){
  
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);  
}
void distance_RGB_Indicator(int dist){
  if(dist==0){
    RGB_Color(0,0,0);
    delay(150);
  }else if(dist < 10){
    RGB_Color(0,255,0);
    delay(150);
  }else if(10<=dist && dist<40){
    RGB_Color(0,0,255);
    delay(150);
  }else if(dist < 400){
    RGB_Color(255,0,0);
    delay(150);
  }else{
    RGB_Color(0,0,0);
    delay(100);
  }
}
void distance_Sound_Indicator(int dist){
    if(dist < 40 && dist > 0){
    analogWrite(passiveBuzzerPin,NOTE_F3*200);
    delay(dist*5);
    analogWrite(passiveBuzzerPin,0);
  }else if(dist > 40 && dist < 70){
    analogWrite(passiveBuzzerPin,NOTE_G3*200);
    delay(250+dist);
    analogWrite(passiveBuzzerPin,0);  
  }else{
    delay(100);
    analogWrite(passiveBuzzerPin,0);
  }
}
int scan_around(int *i){
  //Rotates motor from 15 degrees to 165 and reads sensor.
  if(i < 165){
    myServo.write(i);
  }//Rotates motor form 165 degrees to 15 and reads sensor.
  else if(i == 165 ){
    i = 15;
    myServo.write(i);
    return i;
  }
}


/* Notes for color setting.
  RGB_color(255, 0, 0); // Red
  delay(1000);
  RGB_color(0, 255, 0); // Green
  delay(1000);
  RGB_color(0, 0, 255); // Blue
  delay(1000);
  RGB_color(255, 255, 125); // Raspberry
  delay(1000);
  RGB_color(0, 255, 255); // Cyan
  delay(1000);
  RGB_color(255, 0, 255); // Magenta
  delay(1000);
  RGB_color(255, 255, 0); // Yellow
  delay(1000);
  RGB_color(255, 255, 255); // White
  delay(1000);
  
*/
int i = 15;
void loop() {
  int dist;
  scan_around(i);
  if(i == 165){
    i = scan_around(i);
  }
  i+=5;
  dist = Read_Sensor_Dist();
  distance_RGB_Indicator(dist);
  distance_Sound_Indicator(dist);
    Serial.print(i);
    Serial.print("*");
    Serial.print(dist);
    Serial.print("#");
//  if(dist <= max_dist){
//    Serial.print(dist);
//    Serial.print("cm");
//    Serial.println();
//  }
  int ReadTiltSensor = analogRead(tiltSensorPin);
  //Serial.println(ReadTiltSensor);
  if(ReadTiltSensor < 900 && ReadTiltSensor > 10){
    analogWrite(passiveBuzzerPin,50*200);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("The structure is");
    lcd.setCursor(0,1);
    lcd.print("Flipped Over!");
    delay(1000);
  }
  if(i%2 == 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance :");
    lcd.print(dist);
    lcd.print("cm");
    delay(200);
  }else{
    lcd.setCursor(0,1);
    lcd.print("UpdtDistance :");
    lcd.print(dist);
    lcd.print("cm");
    delay(200);
  }
  delay(10);
}
