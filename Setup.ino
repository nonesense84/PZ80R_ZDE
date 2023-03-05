void setup() {
  // Festlegen, welche Eingaenge als Ausgang dienen sollen
  for (byte pin = 0; pin <  9; pin++)pinMode(pin, OUTPUT);
  // Festlegen, welche Eingaenge als Ausgang dienen sollen
  for (byte pin = 9; pin < 13; pin++)pinMode(pin, INPUT_PULLUP);// Festlegen, welche Eingaenge als Eingang dienen sollen (Mit Pullup)
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  // Aktivieren von Ethernet, I²C-Bus und ggf serieller Kommunikation
  // Serial.begin(115200);   // Kann zu Testzwecken verwendet werden. Achtung: Die 7-Segmentanzeigen funktionieren dann nicht!
  Ethernet.begin(mac, shield_ip);   // Ethernet die Mac- und IP-Adresse geben
  Wire.begin();                     // I²C-Buis starten
  delay(1000);
  Serial.println("Connecting");
  while (!zusi3Connected) {
    if (!client.connect(server_ip, port_zusi3)) {
      Serial.println("Connecting to Zusi3 failed.");
      delay(1000);
    }
    else {
      Serial.println("Zusi3 connected");
      zusi3Setup();
      zusi3Connected = true;
    }
  }
}
