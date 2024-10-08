#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
const char *ssid = "MonWifi";
const char *password = "amogus666";
// Set LED GPIO
const int ledPin = 2; // Led intégrée à l'ESP32
// Stores LED state
String ledState;
// Potentiomètre relié à GPIO 34 (Analog ADC1_CH6)
const int potPin = 34;

AsyncWebServer server(80);
//Remplace l'espace réservé repéré par des % dans le fichier index.html par
String(mesure);
String processor(const String& var){
float mesure;
mesure = analogRead(potPin);
Serial.println(var);
Serial.println(String(mesure));
return String(mesure);
}
void setup(){
Serial.begin(115200);
pinMode(2, OUTPUT);
//---------------------------SPIFFS-------------------
if(!SPIFFS.begin()) /* Démarrage du gestionnaire de fichiers SPIFFS */
{
Serial.println("Erreur SPIFFS...");
return;
}

//-----------------------WIFI-----------------------------
WiFi.begin(ssid, password); /* Connexion au réseau Wifi */
Serial.print("Tentative de connexion...");
while(WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(100);
}
Serial.println("\n");
Serial.println("Connexion etablie!");
Serial.print("Adresse IP: ");
Serial.println(WiFi.localIP());

//--------------------------SERVEUR--------------------------
/* Lorsque le serveur est actif , la page index.html est chargée */
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/index.html", String(), false, processor);
});
// Chemin pour chager le fichier style.css
server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/style.css", "text/css");
});
// Page pour allumer la led
server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
digitalWrite(ledPin, HIGH);
request->send(SPIFFS, "/index.html", String(), false, processor);
});
// Page pour éteindre la led
server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
digitalWrite(ledPin, LOW);
request->send(SPIFFS, "/index.html", String(), false, processor);
});
// Démarrage du serveur
server.begin();
}
void loop(){
}