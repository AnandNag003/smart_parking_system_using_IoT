#include<LiquidCrystal.h>
#include<SoftwareSerial.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int button=10;
const int IR=6;
const byte tx=3,rx=2;
SoftwareSerial esp8266(rx,tx);
String ssid="Nokia 8";
String password="1234567801";
boolean found= false;
int value;
String api ="3Q2REFN5IWQG1WJZ";
String host="api.thingspeak.com";
String port="80";

void setup() {
  pinMode(button,INPUT_PULLUP);
  lcd.begin(16,2);
  pinMode(IR,INPUT); 
  Serial.begin(9600);
  esp8266.begin(115200);
  Serial.println("IOT PROJECT");
  connectwifi();
  // put your setup code here, to run once:

}

void loop() {
  if(digitalRead(button)==0) //activates the security systems
  {
    lcd.print("AREA PROTECTED");   
    if(digitalRead(IR)==1)
    {
      value=digitalRead(IR);
      senddata();
    }
    else
    {
      lcd.print("AREA BREACHED");
      value=digitalRead(IR);
      senddata();
      
    }
  }
  else
  {
    lcd.print("SECURITY STANDBY");
  }
  // put your main code here, to run repeatedly:

}
void sendcommand(String command,int maxtime,char readreply[])
{
  Serial.print(".at command=>");
  Serial.print(command);
  Serial.print(" ");
  while(maxtime--)
  {
    esp8266.println(command);
    if (esp8266.find(readreply))
    {
      found=true;break;
    }
  }
  if(found)
  {
    Serial.println("OK DONE");
  }
  else
  {
    Serial.println("FAIL");
  }
  found=false;
}
void connectwifi()
{
  sendcommand("AT",5,"OK");
  sendcommand("AT+CWMODE=1",5,"OK");
  sendcommand("AT+CWJAP=\"" + ssid +"\",\""+ password +"\"",20,"OK");
}
void senddata()
{
  String getData1 = "GET /update?api_key="+ api;
  String getData2 = "&field1="+String(value);
  sendcommand("AT+CIPSTART=\"TCP\",\""+host +"\","+ port, 15, "OK"); 
  sendcommand("AT+CIPSEND=" + String (getData1.length()+ getData2.length()+2), 7, ">");
  delay(10);
  esp8266.println(getData1+getData2); Serial.println(getData1 + getData2);
  delay(1000);
  sendcommand("AT+CIPCLOSE=0",5, "OK");
  Serial.println(".................................");
}
