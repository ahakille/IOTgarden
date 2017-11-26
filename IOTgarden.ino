#include <DallasTemperature.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#define ONE_WIRE_BUS 10               
#define SSID "NETGEAR74"     // "SSID-WiFiname"
#define PASS "XX" // "password"
#define IP "api.thingspeak.com"      // thingspeak.com ip

String msg = "GET /update?key=XX"; //Your ThingSpeak API key
//Instances
SoftwareSerial esp8266(2,3); //Rx,Tx
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//Variables
unsigned long lastTimeMillis = 0;
int chk;
float temp;
int hum;
int mSensorValue;
int mSensorValue1;
int mSensorValue2;
float Celcius;
int pResistor;
int error;
String count; 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  delay(100);
  sensors.begin();
  Serial.println("AT");
  esp8266.println("AT");
  printResponse();
  delay(1000);
  pinMode(A5,INPUT);
 
 // connectWiFi();
  
  delay(1000);
}

void loop()
{

  start: //label
    error=0;
  //  mSensorValue = analogRead(A0);
  //  mSensorValue1 = analogRead(A1);
  //  mSensorValue2 = analogRead(A2);
	pResistor = analogRead(A5);
    sensors.requestTemperatures();
    Serial.print("Temperature for the device 1 (index 0) is: ");
	Celcius = sensors.getTempCByIndex(0) - 2.49;
	Serial.println(Celcius);

	Serial.println("P= ");
	Serial.println(pResistor);
    Serial.println("M= ");
    Serial.println(mSensorValue);
    delay(1000);
    //  Getinfo();
    //updateTemp();
   // Resend if transmission is not completed
   if (error==1)
    {
      goto start; //go to label "start"
    } 
 // delay(36000);
}

// POST SENSORVALUE
void updateTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  cmd = msg;
  cmd += "&field1=";    //field 1 for temperature
  cmd += mSensorValue;
  cmd += "&field2=";    //field 1 for temperature
  cmd += mSensorValue1;
  cmd += "&field3=";    //field 1 for temperature
  cmd += mSensorValue2;
  cmd += "\r\n";
  delay(200);
  count = cmd.length();
  Serial.println("AT+CIPSEND="+count);
  esp8266.println("AT+CIPSEND="+count);
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
  printResponse();
  delay(2000);
  String cmd="AT+CWJAP=\"";
  printResponse();
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  printResponse();
  delay(5000);
  
    return true;
  
 
}
