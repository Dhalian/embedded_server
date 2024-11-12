#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>

// Informations Wi-Fi
const char* ssid = "NETGEAREA758B-g026";
const char* password = "boulerouge62219!";

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200); // Initialisation de la communication série

    // Connexion Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connexion Wi-Fi en cours...");
    }
    Serial.println("Connecté au Wi-Fi");

    // Initialiser LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Erreur de montage LittleFS");
        return;
    }

    // Configurer le serveur HTTP pour servir le fichier index.html
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Initialiser OTA
    ArduinoOTA.onStart();
    ArduinoOTA.onEnd([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) { // Mise à jour du firmware
            type = "firmware";
        } else { // Mise à jour du système de fichiers
            type = "filesystem";
        }
        Serial.println("Fin de la mise à jour : " + type);
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progression de la mise à jour : %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Erreur OTA : ");
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Erreur d'authentification");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Erreur de début");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Erreur de connexion");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Erreur de réception");
        } else if (error == OTA_END_ERROR) {
            Serial.println("Erreur de fin");
        }
    });

    server.begin(); // Démarrer le serveur
}

void loop() {
    ArduinoOTA.handle(); // Gérer les requêtes OTA
}
// second commit sur master
// encore un commit