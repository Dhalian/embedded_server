#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

// Informations de connexion Wi-Fi
const char* ssid = "{SSID}";
const char* password = "{PASSWORD}";

// Configuration de l'adresse IP fixe
IPAddress ip(192, 168, 1, 36);        // Adresse IP fixe
IPAddress gateway(192, 168, 1, 1);     // Adresse de la passerelle
IPAddress subnet(255, 255, 255, 0);    // Masque de sous-réseau
IPAddress dns(8, 8, 8, 8);             // Serveur DNS

void setup() {
  Serial.begin(115200);
  
  // Connexion au Wi-Fi avec IP statique
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);  // Configure l'IP fixe avant la connexion
  
  // Reconnexion automatique après une perte de connexion ou un redémarrage
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true); // Sauvegarde les infos de connexion dans la mémoire flash
  
  WiFi.begin(ssid, password);
  
  // Attendre la connexion avec un timeout de 30 secondes
  int timeout = 30;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(1000);
    Serial.print(".");
    timeout--;
  }
  
  // Vérification si connecté ou non
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnecté au Wi-Fi avec IP fixe : ");
    Serial.println(WiFi.localIP());  // Affiche l'IP actuelle
  } else {
    Serial.println("\nÉchec de la connexion au Wi-Fi");
  }

  // Initialisation de OTA
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Début de la mise à jour : " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nFin de la mise à jour");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progression : %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erreur[%u] : ", error);
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

  ArduinoOTA.begin();
  Serial.println("OTA prêt");
}

void loop() {
  // Vérifie si le Wi-Fi est toujours connecté et essaie de se reconnecter si nécessaire
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi déconnecté, tentative de reconnexion...");
    WiFi.disconnect();
    delay(1000);  // Pause avant de retenter la reconnexion
    WiFi.begin(ssid, password);  // Tentative de reconnexion
    delay(5000);  // Pause après la tentative de reconnexion
  }

  ArduinoOTA.handle();  // Vérifie si une mise à jour OTA est en cours
}
