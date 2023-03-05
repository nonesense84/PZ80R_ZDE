void loop() {
    // Abfragen ggf. ankekommender Daten von Zusi
  zusi3LoopRoutine();

    // Abfragen des Stoerschalters. An Zusi ist nicht 0 und 1, sondern 1 und 2 zu uebertragen!
  if(!digitalRead(pinStoerschalter)+1 !=stoerbetrieb){
    stoerbetrieb = !digitalRead(pinStoerschalter)+1;
    setStoerschalter(stoerbetrieb);
  }
  // Abfrage des Uebernahmetasters und setzen, bzw. uebertragen der Zugdaten, in Abhaengigkeit des Vorwahlschalters
  if(digitalRead(pinUebernahme) != uebernahmeOld){
    uebernahmeOld = digitalRead(pinUebernahme);
    if ( digitalRead(pinVorwahl) && !uebernahmeOld)setBra(getThumbWheel());
    if (!digitalRead(pinVorwahl) && !uebernahmeOld)setBrh(getThumbWheel());
  }
  // Festlegen, welcher Wert auf der 7-Segmentanzeige angezeigt werden soll
  if (digitalRead(pinVorwahl)) {
    setSegment(braFromZusi);
  }
  else {
    setSegment(brhFromZusi / 10);
  }
}
