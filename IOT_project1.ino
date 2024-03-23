#include <TinyGPS.h>

#include <TinyGPS++.h>
#include <TinyGPSPlus.h>


#include <ESP8266WiFi.h>




#include <SoftwareSerial.h> // Bibliothèque pour le GPS NEO-M6
#include <DFRobotDFPlayerMini.h> // Bibliothèque pour le DFPlayer
#include <TinyGPS++.h>


#define RX_PIN 14 // Pin RX pour le module GPS
#define TX_PIN 12 // Pin TX pour le module GPS
#define MP3_RX 13 // Pin RX pour le DFPlayer
#define MP3_TX 15 // Pin TX pour le DFPlayer

SoftwareSerial gpsSerial(RX_PIN, TX_PIN); // Création d'un objet Serial pour le GPS NEO-M6
SoftwareSerial mp3Serial(MP3_RX, MP3_TX); // Création d'un objet Serial pour le DFPlayer

DFRobotDFPlayerMini mp3Player; // Création d'un objet DFPlayer
TinyGPS gps;  // The TinyGPS++ object

// Coordonnées GPS pour chaque endroit
float Bibliotheque_lat = 33.0313723;
float Bibliotheque_lon = -7.6170786;

float Buvette_lat = 33.0297026;
float Buvette_lon = -7.6155857;

float Mosquee_lat = 33.0307654;
float Mosquee_lon = -7.6155002;

float Administration_lat = 33.0305656;
float Administration_lon = -7.6173891;

float AmphiCenral_lat = 33.0308256;
float AmphiCenral_lon = -7.6166086;

float Atelier_lat = 33.0312792;
float Atelier_lon = -7.6157181;

float CentreDoctorales_lat = 33.0297667;
float CentreDoctorales_lon = -7.6172138;

float NB_lat = 33.0304472;
float NB_lon = -7.6154241;



void setup() {
  Serial.begin(9600); // Initialisation de la communication série pour le débogage
  gpsSerial.begin(9600); // Initialisation de la communication série pour le GPS
  mp3Serial.begin(9600); // Initialisation de la communication série pour le DFPlayer

  mp3Player.begin(mp3Serial); // Initialisation du DFPlayer

  // Configuration des pins de sortie pour les haut-parleurs
  pinMode(2, OUTPUT); // Par exemple, D4 peut être utilisé pour contrôler le mini haut-parleur
}




void loop() {
 
  if (gpsSerial.available()) {
    String gpsData = gpsSerial.readStringUntil('\n'); // Lecture des données GPS
Serial.println(gpsData);
    // Analyse des données GPS
    // Exemple: Si les coordonnées GPS correspondent à la bibliothèque, jouer le fichier audio correspondant
    float latitude, longitude;
    if (parseGPS(gpsData, latitude, longitude)) {
      // Comparaison des coordonnées GPS actuelles avec celles de la bibliothèque
     
     
      if (isNearLocation(latitude, longitude, Bibliotheque_lat, Bibliotheque_lon)) {
        playAudio(1); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, Buvette_lat, Buvette_lon)) {
        playAudio(2); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, Mosquee_lat, Mosquee_lon)) {
        playAudio(3); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, Administration_lat, Administration_lon)) {
        playAudio(4); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, AmphiCenral_lat, AmphiCenral_lon)) {
        playAudio(5); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, Atelier_lat, Atelier_lon)) {
        playAudio(6); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, CentreDoctorales_lat, CentreDoctorales_lon)) {
        playAudio(7); // Jouer le fichier audio de la bibliothèque
      }
      if (isNearLocation(latitude, longitude, NB_lat, NB_lon)) {
        playAudio(8); // Jouer le fichier audio de la bibliothèque
      }





     
    }
  }
}



bool parseGPS(String data, float& latitude, float& longitude) {
  // Exemple de format des données GPS : $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

  // Vérifier si les données commencent par "$GPGGA"
  if (data.indexOf("$GPGGA") != -1) {
    // Diviser les données en segments en utilisant les virgules comme délimiteur
    String segments[15];
    int startIndex = 0;
    int endIndex = data.indexOf(',');
    for (int i = 0; i < 14; i++) {
      segments[i] = data.substring(startIndex, endIndex);
      startIndex = endIndex + 1;
      endIndex = data.indexOf(',', startIndex);
    }
    // Extraire la latitude et la longitude
    if (segments[2].length() >= 9 && segments[4].length() >= 10) {
      // Convertir les données en degrés décimaux
      latitude = segments[2].substring(0, 2).toFloat() + segments[2].substring(2).toFloat() / 60;
      if (segments[3] == "S") {
        latitude = -latitude; // Inverser la latitude si elle est au sud
      }
      longitude = segments[4].substring(0, 3).toFloat() + segments[4].substring(3).toFloat() / 60;
      if (segments[5] == "W") {
        longitude = -longitude; // Inverser la longitude si elle est à l'ouest
      }
      return true; // Les données sont analysées avec succès
    }
  }
  return false; // Échec de l'analyse des données GPS
}




bool isNearLocation(float current_lat, float current_lon, float target_lat, float target_lon) {
  // Distance minimale acceptable entre la position actuelle et la cible (en degrés)
  const float MIN_DISTANCE = 0.001; // Ajustez cette valeur selon vos besoins

  // Calculer la distance entre les deux points en utilisant la formule de la distance euclidienne
  float distance = sqrt(sq(current_lat - target_lat) + sq(current_lon - target_lon));

  // Vérifier si la distance est inférieure à la distance minimale acceptable
  if (distance < MIN_DISTANCE) {
    return true; // La position est suffisamment proche de la cible
  } else {
    return false; // La position n'est pas suffisamment proche de la cible
  }
}




void playAudio(const int filename) {
  // Arrêter la lecture en cours
  mp3Player.stop();

  // Jouer le fichier audio spécifié
  mp3Player.playMp3Folder(filename);
 
  // Ajoutez un délai pour permettre à l'audio de démarrer
  delay(1000);
}
