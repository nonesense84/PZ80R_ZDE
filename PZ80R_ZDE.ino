#include <SPI.h>
#include <EthernetClient.h>
#include <Wire.h>
#define addrPortexpander 63                         // 0x3F Adresse des PCF8574A
#define pinVorwahl A0                               // Pin an dem der Vorwahlschalter angeschlossen ist
#define pinKontrolle A1                             // Pin an dem die Kontrolltaste angeschlossen ist
#define pinStoerschalter A2                         // Pin an dem der Stoerschalter angeschlossen ist
#define pinUebernahme A3                            // Pin an dem die Uebernahmetaste angeschlossen ist
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF}; // Macadresse des Arduino auf dem Ethernet
int port_zusi3 = 1436;                              // TCP-IP Port auf dem Zusi zu erreichen ist
IPAddress shield_ip(192, 168, 178, 144);            // IP-Adresse des Arduinos
IPAddress server_ip(192, 168, 178, 77);             // IP-Adresse des Zusi3-Rechners
EthernetClient client;                              // Der Ethernet client
bool zusi3Connected;                                // Legt fest, ob Zusi verbunden ist, oder nicht
// Die folgenden beiden Arrays dienen der Uerbersetzung des Anzuzeignen Werte auf eine
// Bitkombination zur Ansteuerung der 7-Segment LEDs
byte segment1[10] = {252, 24, 108, 60, 152, 180, 244, 28, 252, 188};
byte segment2[10] = {0, 0, 1, 1, 1, 1, 1, 0, 1, 1};
byte braFromZusi = 0;                               // Die von Zusi kommende Bremsart
word brhFromZusi = 0;                               // Die von Zusi kommenden Bremshundertstel
bool digitTogler;                                   // Muss staendig umschalten. Mal wird das linke Display angesteuert, mal da recht
byte fingerInput = 0;                               // An dem Finger-Encoder eingestellter Wert
bool uebernahmeOld = true;                          // Letzter Zustand der Uebernahmebuttons
byte stoerbetrieb = 1;                              // Der Stoerbetrieb. 1=Normalbetrieb, 2=Stoerbetrieb
void setSegment(byte dispValue) {                   // Funktion zum setzen der 7-Segmentanzeigen. Geht nicht, wenn serielle Ausgabe aktiv ist!
  if (digitTogler) {
    PORTD = segment1[dispValue % 10] & 0b11111100;
    digitalWrite(8, segment2[dispValue % 10]);
  }
  else {
    PORTD = segment1[dispValue / 10] & 0b11111100;
    digitalWrite(8, segment2[dispValue / 10]);
  }
  digitalWrite(1, !digitTogler && !digitalRead(pinKontrolle));
  digitalWrite(0,  digitTogler && !digitalRead(pinKontrolle));
  digitTogler = !digitTogler;
}

byte getThumbWheel() {                              // Funktion zum Einlesen der Finger-Encoder
  fingerInput = 0;
  byte portexpander;
  Wire.requestFrom(addrPortexpander, 1);
  while (Wire.available() > 0) portexpander = Wire.read();
  Wire.endTransmission();
  switch (portexpander & 0b00001111) {
    case  1: fingerInput = 7; break; //7
    case  2: fingerInput = 6; break; //6
    case  4: fingerInput = 5; break; //5
    case  7: fingerInput = 4; break; //4
    case  8: fingerInput = 3; break; //3
    case 11: fingerInput = 2; break; //2
    case 13: fingerInput = 1; break; //1
    case 14: fingerInput = 0; break; //0
    case 15: fingerInput = 8; break; //8
    case 12: fingerInput = 9; break; //9
  }
  switch (portexpander & 0b00110000) {
    case 16: fingerInput += 2 * 10; break; //0
    case 32: fingerInput += 1 * 10; break; //1
    case 48: fingerInput += 0 * 10; break; //2
  }
  return fingerInput;
}
void(* resetFunc) (void) = 0;                       // Reset Funktion
