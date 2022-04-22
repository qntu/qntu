#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,0);
IPAddress subnet(255,255,255,0);
Servo myservo; 
ESP8266WebServer server(80);

int khoangcachvc = 35;// Khoảng cách phát hiện vật cản
const int trigPin = D0;
const int echoPin = D2;
const int S_L =D3;
const int S_R =D4;
const int motorAspeed  = D8;
const int motorA1      = D9;
const int motorA2      = D10;
const int motorB1      = D11;
const int motorB2      = D12;
const int motorBspeed  =D13;
int khoangcach;
String trangthai;

void setup() {
  Serial.begin(115200);
  //Cau hinh access point
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("B1807607");
  Serial.println(WiFi.softAPIP());
  //Cau hinh webserver
  server.on( "/", HTTP_handleRoot );
  server.onNotFound( HTTP_handleRoot );
  server.begin();
  Serial.println("Server started");
  //
  pinMode(S_L,INPUT);
  pinMode(S_R,INPUT);
  pinMode(motorA1, OUTPUT); 
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorAspeed, OUTPUT);
  pinMode(motorBspeed, OUTPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
 
}
void loop() {
  myservo.write(90);
  server.handleClient();
  trangthai = server.arg("State");
  if (trangthai == "19") doline();
  else if (trangthai == "20") vatcan();
  else{
    analogWrite (motorAspeed, 250);
    analogWrite (motorBspeed, 250);
    Serial.println(trangthai);
    if (trangthai == "1") tien();
    else if (trangthai == "2") lui();
    else if (trangthai == "13") trai();
    else if (trangthai == "14") phai();
    else if (trangthai == "23") luitrai();
    else if (trangthai == "24") luiphai();
    else  dung();
  }
}
//
void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
void vatcan(){
  analogWrite(motorAspeed, 150); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 150);// tốc độ động cơ b ban đầu 150 ( 0 - 255)
  khoangcach=dosong();
  Serial.print("giua");
  Serial.println(khoangcach);
  if(khoangcach < khoangcachvc)
  {
    dung();
    delay(100);
    phai();
    delay(1500);
  }
  tien();
}
void doline(){
  Serial.println("doline");
  analogWrite(motorAspeed, 150); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 150);// tốc độ động cơ b ban đầu 120 ( 0 - 255)
  if ((digitalRead(S_L) == 0)&&(digitalRead(S_R) == 0)){tien();}// đi tiến
  if ((digitalRead(S_L) == 1)&&(digitalRead(S_R) == 0)){trai();} // rẻ trái
  if ((digitalRead(S_L) == 0)&&(digitalRead(S_R) == 1)){phai();} // rẻ phải
  if ((digitalRead(S_L) == 1)&&(digitalRead(S_R) == 1)){dung();} // stop

}
int dosong(){
  long duration,distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
  }
void tien(){
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);                       
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW);
  Serial.println("tien");
}
void lui(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH);
  Serial.println("lui");
}
void trai(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW);
  Serial.println("trai");
}
void phai(){
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);                       
  digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH); 
  Serial.println("phai");
}
void luitrai(){
  digitalWrite(motorA1,HIGH);
  digitalWrite(motorA2,LOW);                       
  digitalWrite(motorB1,LOW);
  digitalWrite(motorB2,HIGH); 
  Serial.println("lui trai");
}
void luiphai(){
  digitalWrite(motorA1,LOW);
  digitalWrite(motorA2,HIGH);                       
  digitalWrite(motorB1,HIGH);
  digitalWrite(motorB2,LOW); 
  Serial.println("lui phai");
}
void dung(){
  digitalWrite (motorA1,LOW);
  digitalWrite(motorA2,LOW);                       
  digitalWrite (motorB1,LOW);
  digitalWrite(motorB2,LOW); 
  Serial.println("dung");
}
