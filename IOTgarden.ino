#include <SoftwareSerial.h>
#include <stdlib.h>
#define DEBUG true
#define DHTPIN 7                 
#define SSID "NETGEAR74"     // "SSID-WiFiname"
#define PASS "freshmoon280" // "password"
#define IP "api.thingspeak.com"      // thingspeak.com ip

String msg = "GET /update?key=1VTNA7N0WI2DZBSY"; //Your ThingSpeak API key
SoftwareSerial esp8266(2,3); //Rx,Tx
unsigned long lastTimeMillis = 0;

//Variables
int chk;
float temp;
int hum;
int sensorValue;
int sensorValue1;
int sensorValue2;
int pResistor;
int error;
String count; 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  delay(100);

  Serial.println("AT");
  esp8266.println("AT");

  delay(1000);
 
  connectWiFi();
  
  delay(1000);
}

void loop()
{

  //lcd.autoscroll(); 
 // start: //label
   // error=0;
   // sensorValue = analogRead(A0);
   // sensorValue1 = analogRead(A1);
   // sensorValue2 = analogRead(A2);


   // Serial.println("M= ");
   // Serial.println(sensorValue);
  //  delay(1000);
	  Getinfo();
   // updateTemp();
    //Resend if transmission is not completed
  // if (error==1)
    //{
     // goto start; //go to label "start"
   // } 
 // delay(36000);
}


void updateTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  cmd = msg;
  cmd += "&field1=";    //field 1 for temperature
  cmd += sensorValue;
  cmd += "&field2=";    //field 1 for temperature
  cmd += sensorValue1;
  cmd += "&field3=";    //field 1 for temperature
  cmd += sensorValue2;
  cmd += "\r\n";
  delay(200);
  count = cmd.length();
  Serial.println("AT+CIPSEND="+count);
  esp8266.println("AT+CIPSEND="+count);
  //Serial.println(cmd.length());
  //esp8266.println(cmd.length());
  delay(5000);

   
    Serial.println(cmd);
    esp8266.println(cmd);



}
void printResponse() 
{
  while (esp8266.available())
  {
    Serial.println(esp8266.readStringUntil('\n')); 
  }
}
void Getinfo()
{
  if (millis() - lastTimeMillis > 30000) {
    lastTimeMillis = millis();
 esp8266.println("AT+CIPMUX=1");
  delay(1000);
  printResponse();

  esp8266.println("AT+CIPSTART=4,\"TCP\",\"api.nppc.se\",80");
  delay(1000);
  printResponse();

  String cmd = "GET /index.html HTTP/1.0";
  esp8266.println("AT+CIPSEND=4," + String(cmd.length() + 4));
  delay(1000);

  esp8266.println(cmd);
  delay(1000);
  esp8266.println("");}
  
  if (esp8266.available()) 
  {
    Serial.write(esp8266.read());
  }

}




boolean connectWiFi()
{
  Serial.println("AT+CWMODE=3");
  esp8266.println("AT+CWMODE=3");

  delay(2000);
  String cmd="AT+CWJAP=\"";

  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);

  delay(5000);
  
    return true;
  
 
}
