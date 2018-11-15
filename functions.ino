void Step() {                                   // The step function
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);
}

void togglePower() {                            // the power outlet function
  if (powerStatus == false) {
    digitalWrite(relaisPin, HIGH);
    powerStatus = true;
    Serial.print("Power On");
  }
  else {
    digitalWrite(relaisPin, LOW);
    powerStatus = false;
    Serial.print("Power Off");
  }
}

void readMidi() {                               // The MIDI function
    if (mySerial.available() > 0) {             // Is there any MIDI waiting to be read?
        midiByte = mySerial.read();             // read MIDI byte
    switch (state) {
      case STATE_NONE:                          // Filtering input and channel
        
        midiChannel = midiByte & B00001111;     // remove channel info
        midiCommand = midiByte & B11110000;
        if (midiChannel == filterChannel)
        {
          if (midiCommand == MIDI_NOTE_ON)
          {
            state = STATE_NOTE_ON;
          }
        }
        break;

      case STATE_NOTE_ON:
        midiNote = midiByte;
        state = STATE_NOTE;
        break;

      case STATE_NOTE:                          // Filtering notes and velocity
        midiVelocity = midiByte;
        state = STATE_NONE;
        if (midiNote == filterNoteC && midiVelocity > 0)            
        {
          digitalWrite(ledPin, HIGH);
          actionTime = millis();
          actionOn = true;
          Serial.print("Midi Received");
          togglePower();
        }
        else if (midiNote == filterNoteD && midiVelocity > 0)
        {
          digitalWrite(ledPin, HIGH);
          actionTime = millis();
          actionOn = true;
          Serial.print("Midi Received");
          Step();
        }
        else if (midiNote == filterNoteE && midiVelocity > 0)
        {
          digitalWrite(ledPin, HIGH);
          actionTime = millis();
          actionOn = true;
          Serial.print("Midi Received");
        }
        else if (midiNote == filterNoteF && midiVelocity > 0)
        {
          digitalWrite(ledPin, HIGH);
          actionTime = millis();
          actionOn = true;
          Serial.print("Midi Received");
        }

        break;
    }
  }
  if (actionOn)                                 // turn action off after predetermined time
  {
    if ((millis() - actionTime) > actionDuration)
    {
      actionOn = false;
      digitalWrite(ledPin, LOW);
    }
  }
}
