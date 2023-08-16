/**********************************************************************************
                   Sensor de Gás com o NodeMcu - TAI 2022 

 *  INTEGRANTES:  Gabriel Camargos
                  Gustavo Samuel
                  Laíssa Maria
                  Lucas Gustavo
                  Matheus Ribeiro
                  
 *  ORIENTADORES: Prof. Douglas Emiliano Januário Monteiro
                  Prof. Luiz Augusto Ferreira de Campos Viana
                  Prof. Gilmar Junio Soares Rossi
                  
 *  CÓDIGO BASE: https://iotcircuithub.com/mq2-gas-sensor-rain-detection-blynk-notification/

 **********************************************************************************/

// CONFIGURAÇÃO DE FIRMWARE
#define BLYNK_TEMPLATE_ID "TMPLgB2FbXaP"
#define BLYNK_DEVICE_NAME "Trabalho de Tai 2022"
#define BLYNK_AUTH_TOKEN "AoHX0Ft7dSVziRnl8RKIWeQzN1RGreLQ"

// Credenciais WiFi.
char ssid[] = "Nome da rede WiFi";
char pass[] = "Senha da rede WiFi";

// Definir o GPIO conectado com sensores e LEDs
#define sensor        A0
#define led_green     D5
#define led_red       D7
#define WIFI_LED      D0

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int sensor_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

#define VPIN_BUTTON_1    V1 

BlynkTimer timer;

void checkBlynkStatus() { // Chamado a cada 2 segundos pelo SimpleTimer
  getSensorData();
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, LOW);
    sendSensorData();
    Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, HIGH);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  sensor_Value = map(analogRead(sensor), 0, 1024, 0, 10000);
 
  if (sensor_Value > 5000 ){
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, HIGH);
    delay(300);
  }
 
  else{
    digitalWrite(led_green, HIGH);
    digitalWrite(led_red, LOW);
    delay(300);
  }
}

void sendSensorData()
{  
  Blynk.virtualWrite(VPIN_BUTTON_1, sensor_Value);
  if (sensor_Value > 6000 )
  {
    Blynk.logEvent("gs_detectado", "ATENÇÃO!!! Gás Detectado na sala 213");
  }
}

void setup()
{
  Serial.begin(9600);
 
  pinMode(sensor, INPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  digitalWrite(led_green, LOW);
  digitalWrite(led_red, LOW);
  digitalWrite(WIFI_LED, HIGH);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // Verifica se o servidor Blynk está conectado a cada 2 segundos
  Blynk.config(auth);
  delay(1000);
}

void loop()
{
  Blynk.run();
  timer.run();
}