#include <DHT.h>              // Biblioteca Sensor Temp y Humedad
#include "NewPing.h"          // include NewPing library


// WiFi
#include <WiFi.h>
#include <AWS_IOT.h>
#include <esp_event.h>

#define WIFI_SSID   "Totalplay-79A6"
#define WIFI_PSSWD  "79A6964Eub25naww"
#define CLIENT_ID   "AtmosphericProbe_1"
#define MQTT_TOPIC  "aws/..."
#define AWS_HOST    "host..."

AWS_IOT aws;


// Variables y definiciones del DHT22
#define DHTPIN  12
#define DHTTYPE 22
DHT dht(DHTPIN, DHTTYPE);
float h_pct, t_C;


// Variables para sensor LM35
#define T_SALA   39
#define T_CUARTO 32
float temp_sala;
float temp_cuarto;

// Sensor ultrasónico
int trigPin = 4;      // trigger pin
int echoPin = 0;      // echo pin
float dist_ultra;
NewPing sonar(trigPin, echoPin);

void setup() {
  Serial.begin(115200);

  InitWiFi();
  
  dht.begin();                  // Inicializar el objeto
}

void loop() {

  h_pct   = dht.readHumidity();
  t_C = dht.readTemperature();

  Serial.print("% Humedad con DHT22: ");
  Serial.println(h_pct);

  dist_ultra = sonar.ping_median(5);
  Serial.print("% Distancia detectada: ");
  Serial.println(dist_ultra);

  temp_sala = ReadTemp(T_SALA);
  temp_cuarto = ReadTemp(T_CUARTO);
  Serial.print("% Temperatura de la sala: ");
  Serial.println(temp_sala);
}


// Función para leer temperatura del integrado LM35
float ReadTemp(int sensor_pin){
  float v, t;
  v = analogRead(sensor_pin);     // Valor entre 0 y 4095, Vref = 3.3V, Res = Vref/4096 = 805.6uV
  v = v*0.0008056;                // V = V*res;
  t = v*100;                      // T = mV/10 = (V*1000)/10 = V/1000

  return t;
}


void InitWiFi(){
  Serial.print("Inicializando WiFi. Conectando a ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PSSWD);       // Intenta conectarse a la red
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n Conectado. \n Listo!");
}
