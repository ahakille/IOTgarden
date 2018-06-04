#include <DallasTemperature.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#define ONE_WIRE_BUS 10               
#define SSID "NETGEAR74"     // "SSID-WiFiname"
#define PASS "freshmoon280" // "password"
#define IP "api.nppc.se"      // thingspeak.com ip

String msg = "GET api/values/post?key=4uK5damAYHw%26N8I"; //Your ThingSpeak API key
//Instances
SoftwareSerial esp8266(2,3); //Rx,Tx
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//Variables
unsigned long lastTimeMillis = 0;
int chk;
float temp;
int SensorId;
String SensorValue;
float Celcius;
int pResistor,sensorid4 = 4;
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
  
  delay(100);
}

void loop()
{

 	pResistor = analogRead(A5);
    sensors.requestTemperatures();
    Serial.print("Temperature for the device 1 (index 0) is: ");
	Celcius = sensors.getTempCByIndex(0) - 2.49;
	Serial.println(Celcius);
	Serial.println("P= ");
	Serial.println(pResistor);
 //   Serial.println("M= ");
 //   Serial.println(SensorValue);
 //   delay(1000);
	SensorValue = String(analogRead(A0));
    UpdateSensor(1,SensorValue);
	delay(10000);
	SensorValue = String(analogRead(A1));
	UpdateSensor(2, SensorValue);
	delay(10000);
	SensorValue = String(analogRead(A5));
	UpdateSensor(4, SensorValue);
	delay(10000);
	SensorValue = String(Celcius);
	UpdateSensor(5, SensorValue);
  delay(600000);
}

// POST SENSORVALUE
void UpdateSensor(int sensorId ,String sensorValue )
{

	String test = "sensorid=" + sensorId;
	test += "&sensordata=" + sensorValue;
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  esp8266.println(cmd);
  cmd = "GET /api/values/post?key=4uK5damAYHwN8I&sensorid="; 
   //field 1 for temperature
  cmd += sensorId;
  cmd += "&sensordata=" + sensorValue +" HTTP/1.1\r\n";
  cmd += "Host: api.nppc.se\r\n";
  cmd += "Connection: close\r\n\r\n";

  delay(200);
  count = cmd.length();
  Serial.println("AT+CIPSEND="+count);
  esp8266.println("AT+CIPSEND="+count);

  delay(1000);

   
    Serial.println(cmd);
    esp8266.println(cmd);
	printResponse();
 delay(5000);
 esp8266.println("AT+CIPCLOSE");
 
}
void printResponse() 
{
  while (esp8266.available())
  {
    Serial.print(esp8266.readStringUntil('\n')); 
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
    esp8266.println("AT+RST");
    printResponse();
    delay(20000);
//  Serial.println("AT+CWMODE=3");
  esp8266.println("AT+CWMODE=3");
  printResponse();
  delay(5000);
  String cmd="AT+CWJAP=\"";
  printResponse();
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  esp8266.println(cmd);
  printResponse();
  delay(10000);
  
    return true;
  
 
}
