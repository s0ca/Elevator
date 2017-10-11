//TODO: Changer le contenu du paquet UDP / Faire un WebSocketsServer pour balancer du TCP
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

int switchPin = 5; // D1 = GPIO5 (ne pas utilser D0/GPIO16 = Reset)
int switchValue; 
unsigned int localPort=8888; //on défini le port pour les packets UDP 
WiFiUDP Udp;
// Infos du point d'acès
const char *ssid = "elevator_test";
const char *password = "azertyuiop";
//serveur web
ESP8266WebServer server(80);

// message test aller à http://192.168.4.1
// une fois la connection au point d'accès faite
//
void handleRoot() {
  server.send(200, "text/html", "<h1>Ascenceur connect&eacute;</h1>");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Initialisation en cours ...");
  // Enlever le parametre "password" si on veux que le point d'accès soir ouvert.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Adresse IP : ");
  Serial.println(myIP);
  
  // serveur Web
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server OK");

  // Defini le boutton comme une entrée
  pinMode(switchPin, INPUT_PULLUP);
  //digitalWrite(switchPin, HIGH);
  // écoute UDP
  Udp.begin(localPort);
}

void loop() {
  server.handleClient();
  //Lecture de l'état du bouton
  switchValue = digitalRead(switchPin);
  if(switchValue != 0)
    { Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write("okay");
      Udp.endPacket();
      delay(500);
      Serial.println("Ascenceur Operationnel"); 
    }
  else
    { Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write("mort");
      Udp.endPacket();
      delay(500);
      Serial.println("Ascenceur Defectueux");
     }
}
