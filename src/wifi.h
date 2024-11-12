
#include <Arduino.h>
#include <ESP8266WiFi.h>

    void configWiFi(bool useStaticIP, IPAddress staticIP, IPAddress gateway, IPAddress subnet) {
    // Informations de connexion au réseau Wi-Fi
    const char *ssid = "NETGEAREA758B-g026";
    const char *password = "boulerouge62219!";

    // Démarrage de la connexion Wi-Fi
    Serial.begin(115200);
    Serial.println("\nDémarrage de la connexion Wi-Fi...");

    // Vérifie si l'ESP8266 est déjà connecté
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Déjà connecté au réseau Wi-Fi!");
        Serial.print("Adresse IP de l'ESP8266 : ");
        Serial.println(WiFi.localIP());
        return;
    }

    // Configuration de l'IP statique ou DHCP
    if (useStaticIP)
    {
        Serial.println("Configuration en IP fixe...");
        WiFi.config(staticIP, gateway, subnet);
    }
    else
    {
        Serial.println("Configuration via DHCP...");
        WiFi.config(0U, 0U, 0U); // Remet la configuration sur DHCP
    }

    // Connexion au réseau Wi-Fi
    unsigned long startTime = millis(); // Enregistre le temps de départ
    WiFi.begin(ssid, password);
    delay(1000);
    // Attente de la connexion
    while (WiFi.status() != WL_CONNECTED)
    {     
        Serial.print(".");
        if (millis() - startTime > 10000) // Arrête la connexion après 10 secondes
        {
            Serial.println("\nImpossible de se connecter au réseau Wi-Fi!");
            break;
        }
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Impossible de se connecter au réseau Wi-Fi!");
        return;
    }
    else
    {
        Serial.println("\nConnexion réussie au réseau Wi-Fi!");
        Serial.print("Adresse IP de l'ESP8266 : ");
        Serial.println(WiFi.localIP());
    }
}






