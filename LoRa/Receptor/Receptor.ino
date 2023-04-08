#include "heltec.h"
#define BAND    915E6 

#include <WiFi.h>
#include "time.h"
#include <FirebaseESP32.h>
 
#define FIREBASE_HOST "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  
#define FIREBASE_AUTH "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" 
#define WIFI_SSID "XXXXXXX" 
#define WIFI_PASSWORD "XXXXXXXXXXX"

const char* ntpServer = "in.pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = -14400;
 
FirebaseData firebaseData;
FirebaseJson json;

String packSize = "--";
String packet;

String temp;
String humid;
String level;

String date;
String hour;

unsigned long sendDataPrevMillis = 0;

void LoRaDataPrint();
void cbk(int packetSize);

void attFirebase(){
  if ((millis() - sendDataPrevMillis > 600000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    json.clear().add("temp", temp);
    json.add("humid", humid);
    json.add("hour", hour);
    json.add("date", date);
    Firebase.pushJSON(firebaseData,"/Data", json);
   }

   json.clear().set("/Temp", temp);
   json.set("/Humid", humid);
   json.set("/level", level);  
   if(Firebase.updateNode(firebaseData,"/Control",json)){
   }
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read(); //Atribui um caractere por vez a váriavel packet 
  }

  temp = packet.substring(0,4);
  humid = packet.substring(6,10);
  level = packet.substring(12,13);
  
  //LoRaDataPrint();
}

//Função tempo
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  char dateChar[22];
  char hourChar[22];
  strftime(dateChar, 22, "%D", &timeinfo);
  strftime(hourChar, 22, "%T", &timeinfo);

  date = String(dateChar);
  hour = String(hourChar);
}

void setup()
{
  pinMode(LED,OUTPUT);
  
  Serial.begin(115200);
  delay(100);
  Serial.println("P1");
  Heltec.begin(false, true, true, true, BAND);
  Serial.println("P2");
  LoRa.receive();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  Serial.println("------------------------------------");
  Serial.println("Connected...");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
}

void loop()
{
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) { 
    cbk(packetSize);
  }  

  delay(100); 
  printLocalTime();
  attFirebase();
  delay(10);

}
