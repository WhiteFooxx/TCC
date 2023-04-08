#include "heltec.h"
#define BAND    915E6
#include <dht22.h>

dht22 dht01(13);

int PinFanPWM = 25;
double setpoint = 28;
double currentPWM = 0;

String packet;
String info;

float temperature;
float humidity;
double currentTemp;
double currentHumidity;
String Level;

unsigned long timeTempHumid = 0;

const int trigPin = 23;
const int echoPin = 17;

void getTemp();
void sendPacket();
void SensorUltra();

void getTemp_Humid(){

  if ((millis() > timeTempHumid)) {
    timeTempHumid = millis() + 2500;

    dht01.dht22Data();
    temperature = dht01.Temp;
    humidity = dht01.Hum;

      if (temperature != currentTemp || humidity != currentHumidity) { 
        currentTemp = temperature;
  
        currentHumidity = humidity;
      }
  }
  info += String(temperature);
  info += "-";
      
  info += String(humidity); 
  info += "-";
}

void SensorUltra() {

  int range;
  String distancia;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long tempoSom = pulseIn(echoPin, HIGH);
  range = tempoSom / 58;

  Serial.print("Distancia: ");
  Serial.print(range);
  Serial.println(" cm");

  delay(1000);

  if ( range <= 6 )
  {
    distancia = 'R';
    info += distancia;
  }
  else if (range <= 11)
  {
    distancia = 'Y';
    info += distancia;
  }
  else if (range <= 16)
  {
    distancia = 'G';
    info += distancia;
  }
  else
  {
    distancia = 'W';
    info += distancia;
  }
}

void sendPacket()
{
  LoRa.beginPacket();
  LoRa.print(info);
  LoRa.endPacket();
}

void PWMset(){
   currentPWM = (currentTemp - setpoint)*255;
   ledcWrite(0, currentPWM);
   delay(2);

   if(currentPWM > 255){
      currentPWM = 255;
      ledcWrite(0, currentPWM);
      delay(2);
   }

   if(currentPWM < 0){
      currentPWM = 0;
      ledcWrite(0, currentPWM);
      delay(2);
   }
   Serial.println(currentPWM);
}

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);
  Heltec.begin(false, true, true, true, BAND);

  pinMode(PinFanPWM, OUTPUT);
  ledcAttachPin(PinFanPWM, 0);
  ledcSetup(0, 25000, 8);
}

void loop()
{
  info = "";

  getTemp_Humid();
  PWMset();
  SensorUltra();
  sendPacket();
}
