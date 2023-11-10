#include <SoftwareSerial.h>
#include <List.hpp>

SoftwareSerial BTSerial(8, 9);   //bluetooth module Tx:Digital 2 Rx:Digital 3

int trigPin1 = 2;
int echoPin1 = 3;
int trigPin2 = 4;
int echoPin2 = 5;
int trigPin3 = 6;
int echoPin3 = 7;

long duration1, distance1;
long duration2, distance2;
long duration3, distance3;

List<bool> first, second, third;

bool is_first = false;
bool is_second = false;
bool is_third = false;

String message;

int threshold = 10;
String val = "";


void setup() {
  Serial.begin(115200);  //computer <-> arduino
  BTSerial.begin(115200); // arduino <-> HC-06
  for (int i=0;i<threshold;i++){
    first.add(false);
    second.add(false);
    third.add(false);
  }
}

void loop() {
  if (BTSerial.available())
    val = BTSerial.read();
  if (val == "49"){
    BTSerial.print(message);
    Serial.println(message);
  }
  Serial.println(val);
  // Serial.println(message);


  
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT);  
  pinMode(echoPin2, INPUT); 
  pinMode(trigPin3, OUTPUT); 
  pinMode(echoPin3, INPUT);

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1= duration1*0.034/2;

  first.addLast(distance1 <= 10 && distance1 > 3);
  first.removeFirst();

  bool first_all_true = true;
  for (int i = 0; i< threshold; i++){
    first_all_true = first_all_true && first[i];
  }

  bool first_all_false = true;
  for (int i = 0; i< threshold; i++){
    first_all_false = first_all_false && !first[i];
  }

  if (is_first){
    if (first_all_false){
      is_first = !is_first;
    }
  } else{
    if (first_all_true){
      is_first = !is_first;
    }
  }

  delay(20);

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2= duration2*0.034/2;

  second.addLast(distance2 <= 10 && distance2 > 3);
  second.removeFirst();

  bool second_all_true = true;
  for (int i = 0; i< threshold; i++){
    second_all_true = second_all_true && second[i];
  }

  bool second_all_false = true;
  for (int i = 0; i< threshold; i++){
    second_all_false = second_all_false && !second[i];
  }

  if (is_second){
    if (second_all_false){
      is_second = !is_second;
    }
  } else{
    if (second_all_true){
      is_second = !is_second;
    }
  }

  delay(20);

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3= duration3*0.034/2;

  third.add(distance3 <= 10 && distance3 > 3);
  third.removeFirst();
  
  bool third_all_true = true;
  for (int i = 0; i< threshold; i++){
    third_all_true = third_all_true && third[i];
  }

  bool third_all_false = true;
  for (int i = 0; i< threshold; i++){
    third_all_false = third_all_false && !third[i];
  }

  if (is_third){
    if (third_all_false){
      is_third = !is_third;
    }
  } else{
    if (third_all_true){
      is_third = !is_third;
    }
  }

  delay(20);

  message = String(is_first) + String(is_second) + String(is_third) + "\n";
}