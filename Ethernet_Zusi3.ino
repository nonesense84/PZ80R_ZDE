// Byte Serien zum Verbindungsaufbau
uint8_t Anmeldung[] = { 0x0, 0x0, 0x0, 0x0, 0x1, 0x0,           // Knoten Verbindungsaufbau
                        0x0, 0x0, 0x0, 0x0, 0x1, 0x0,               // Knoten Befehl HELLO
                        0x4, 0x0, 0x0, 0x0, 0x1, 0x0, 0x2, 0x0,         // Protokoll-Version
                        0x4, 0x0, 0x0, 0x0, 0x2, 0x0, 0x2, 0x0,         // Fahrpult
                        0xB, 0x0, 0x0, 0x0, 0x3, 0x0, 'P', 'Z', '8', '0', 'R', ' ', 'Z', 'D', 'E',
                        0x5, 0x0, 0x0, 0x0, 0x4, 0x0, '1', '.', '0',
                        0xFF, 0xFF, 0xFF, 0xFF,                     // Knoten Ende
                        0xFF, 0xFF, 0xFF, 0xFF                  // Knoten Ende
                      };
#define NUM_Anmeldung 60    // Anzahl der Bytes in der Byte Serie Anmeldung
uint8_t Abfrage[] = {   0x0, 0x0, 0x0, 0x0, 0x2, 0x0,           // Knoten Client-Anwendung 02
                        0x0, 0x0, 0x0, 0x0, 0x3, 0x0,               // Knoten Befehl NEEDED_DATA
                        0x0, 0x0, 0x0, 0x0, 0xA, 0x0,                   // Knoten Untergruppe Fuehrerstandsanzeigen
                        0x4, 0x0, 0x0, 0x0, 0x1, 0x0, 0x65, 0x0,            // Zugsicherung
                        0xFF, 0xFF, 0xFF, 0xFF,                         // Knoten Ende
                        0xFF, 0xFF, 0xFF, 0xFF,                     // Knoten Ende
                        0xFF, 0xFF, 0xFF, 0xFF                  // Knoten Ende
                    };
#define NUM_Abfrage 38    // Anzahl der Bytes in der Byte Serie Abfrage
// Byte Serie der zu uebertragenen Zugdaten
uint8_t telegramZd[] = {  0x0, 0x0, 0x0, 0x0, 0x02, 0x0,       // Knoten Client-Anwendung 02
                          0x0, 0x0, 0x0, 0x0, 0x0A, 0x1,            // Knoten Befehl INPUT
                          0x0, 0x0, 0x0, 0x0, 0x02, 0x0,                // Knoten Zugbeeinflussung einstellen
                          0x0, 0x0, 0x0, 0x0, 0x02, 0x0,                    // Knoten System aus der Indusi-Familie – Einstellungen
                          0x0, 0x0, 0x0, 0x0, 0x06, 0x0,                        // Knoten Modus Aktive Zugdaten
                          0x4, 0x0, 0x0, 0x0, 0x01, 0x0, 0x8C, 0x0,                 // BRH-Wert 140
                          0x4, 0x0, 0x0, 0x0, 0x02, 0x0, 0x08, 0x0,                 // BRA-Wert   8
                          0x4, 0x0, 0x0, 0x0, 0x05, 0x0, 0x01, 0x0,                 // Zugehoerige Zugart (0:Automatisch | 1: Noch_unbestimmt | 2:U | 3:M | 4:O | 5:S-Bahn)
                          0x4, 0x0, 0x0, 0x0, 0x06, 0x0, 0x00, 0x0,                 // Modus (0:undefiniert | 5:Ersatzzugdaten | 6:Normalbetrieb)
                          0xFF, 0xFF, 0xFF, 0xFF,                               // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF,                           // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF,                       // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF,                   // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF                // Knoten Ende
                       };
#define NUM_telegramZd 82    // Anzahl der Bytes in der Byte Serie der Zugdaten
// Byte Serie mit dem Status des Stoerschalters
uint8_t telegramSt[] = {  0x0, 0x0, 0x0, 0x0, 0x02, 0x0,       // Knoten Client-Anwendung 02
                          0x0, 0x0, 0x0, 0x0, 0x0A, 0x1,            // Knoten Befehl INPUT
                          0x0, 0x0, 0x0, 0x0, 0x02, 0x0,                // Knoten Zugbeeinflussung einstellen
                          0x0, 0x0, 0x0, 0x0, 0x02, 0x0,                    // Knoten System aus der Indusi-Familie – Einstellungen
                          0x4, 0x0, 0x0, 0x0, 0x08, 0x0, 0x02, 0x0,             // Stoerschalter
                          0xFF, 0xFF, 0xFF, 0xFF,                           // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF,                       // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF,                   // Knoten Ende
                          0xFF, 0xFF, 0xFF, 0xFF                // Knoten Ende
                       };
#define NUM_telegramSt 48    // Anzahl der Bytes in der Byte Serie mit dem Stoerschalter

union { // Datentyp zur Abfrage des Knotens
  byte b[4];
  int i;
} dataLng;

union { // Datentyp zur Abfrage eines words (2 Byte Variable)
  byte b[2];
  int16_t i;
} dataGroup;

union { // Datentyp zur Abfrage eines floats
  byte b[4];
  float f;
  int i;
  int16_t i16;
} nutzdata;

int ebene = 0;
int16_t ebene1Status = 0;
int16_t ebene2Status = 0;
int16_t ebene3Status = 0;
int16_t ebene4Status = 0;
int16_t ebene5Status = 0;

#define MAX_NUTZDATA 4

uint8_t clientForceRead() {
  while (!client.available()) {}
  return client.read();
}

//Hauptprogramm
void zusi3Setup() {
  // Byte Serien zum Zusi Server schreiben
  client.write (Anmeldung, NUM_Anmeldung);  // Bei Zusi anmelden
  client.write (Abfrage, NUM_Abfrage);      // Zusi sagen, welche Daten man braucht
}

void setBra(byte bra) { // Schreibt BRA in die Byte Serie der Zugdaten und uebertraegt sie an Zusi
  Serial.println(bra);
  telegramZd[44] = bra;
  client.write (telegramZd, NUM_telegramZd);
}
void setBrh(byte brh) { // Schreibt BRH in die Byte Serie der Zugdaten und uebertraegt sie an Zusi
  word brh_korr = brh * 10; // Die vom Einsteller kommenden BRH mussen noch mit 10 multipliziert werden.
  telegramZd[36] = 0x00ff & brh_korr;
  telegramZd[37] = brh_korr >> 8;
  client.write (telegramZd, NUM_telegramZd);
}
void setStoerschalter(byte stoer) { // Schreibt den Status des Stoerschalters in die Byte Serie fuer diesen und uebertraegt sie an Zusi
  telegramSt[30] = stoer;
  client.write (telegramSt, NUM_telegramSt);
}
// Die folgende Funktion muss staendig in loop() aufgerufen werden. Hier werden von Zusi ankommende Daten ausgewertet.
void zusi3LoopRoutine() {
  if (!client.connected()) {    // Ist die Verbindung verloren gegangen, starten wir den Arduino neu.
    resetFunc();
  }
  if (client.available()) {     // Sind daten verfuegbar, verarbeiten wir sie.
    dataLng.b[0] = clientForceRead();
    dataLng.b[1] = clientForceRead();
    dataLng.b[2] = clientForceRead();
    dataLng.b[3] = clientForceRead();

    if (dataLng.i == 0) { // Begin eines Knoten (Alle vier Bytes in dataLng sind 0x00)
      ebene++;
      dataGroup.b[0] = clientForceRead();
      dataGroup.b[1] = clientForceRead();
      switch(ebene){
        case 1: ebene1Status = dataGroup.i; break;
        case 2: ebene2Status = dataGroup.i; break;
        case 3: ebene3Status = dataGroup.i; break;
        case 4: ebene4Status = dataGroup.i; break;
        case 5: ebene5Status = dataGroup.i; break;
      }
    } else if (dataLng.i == -1) { // Ende eiens Knoten (Alle vier Bytes in dataLng sind 0xff)
      ebene--;
      if (ebene < 0) { // AUSNAHMEFEHLER: Sollte nicht auftreten, Hilfsweise Arduino neustarten
        resetFunc ();
      }
    } else if (dataLng.i == 1)  { // AUSNAHMEFEHLER: Sollte nicht auftreten, Hilfsweise Arduino neustarten
      resetFunc ();
    } else { // Ist dataLng weder 0, 1 noch -1, dann haben wir ein Atribut, welches folgend decodioert wird
      dataGroup.b[0] = clientForceRead();
      dataGroup.b[1] = clientForceRead();
      nutzdata.i = 0;                           // Loeschen der Nutzdaten Variable
      for (int i = 0; i < dataLng.i - 2; i++) { // Schleife zum Auslesen der Nutzdaten
        byte b = clientForceRead();
        if (i < MAX_NUTZDATA) {
          nutzdata.b[i] = b;
        }
      }

      //AUSWERTUNG
      if ((ebene >= 1) && (ebene1Status == 0x0001)) {       // Verbindungsaufbau
      }
      if ((ebene >= 1) && (ebene1Status == 0x0002)) {       // Fahrpult
        if ((ebene >= 2) && (ebene2Status == 0x000A)) {     // Befehl DATA_FTD - Fuehrerstandsanzeigen
          if (ebene >= 3) {
            switch (ebene3Status) {
              case 0x0065:                                  // Status Zugbeeinflussung
                if (ebene >= 4) {
                  switch (ebene4Status) {
                    case 0x0002:                            // System aus der Indusi-Familie - Einstellungen
                      if (ebene >= 5) {
                        switch (ebene5Status) {
                          case 0x0006:                      // Aktive Zugdaten
                            if (ebene == 5) {
                              switch (dataGroup.i) {        // BRH-Wert (Bremshundertstel)
                                case 0x0001:
                                  brhFromZusi = nutzdata.b[0];
                                  brhFromZusi = brhFromZusi | nutzdata.b[1] << 8;
                                  Serial.print("BRH: "); Serial.println(brhFromZusi); // Zu Testzwecken. Serial.begin(115200) muss aktiviert sein
                                  break;
                                case 0x0002:                // BRA-Wert (Bremsart)
                                  braFromZusi = nutzdata.b[0];
                                  Serial.print("BRA: "); Serial.println(braFromZusi); // Zu Testzwecken. Serial.begin(115200) muss aktiviert sein
                                  break;
                              }
                            }
                        }
                      }
                  }
                }
                break;
            }
          }
        }
      }
    }
  }
}
