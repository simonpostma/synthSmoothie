#include <SoftwareSerial.h>   //      libraries
#include <Stepper.h>

#define stepperPin1  8        // IN1  these are the Arduino pins that we use to activate coils 1-4 of the stepper motor
#define stepperPin2  9        // IN2
#define stepperPin3  10       // IN3
#define stepperPin4  11       // IN4
#define midiRXPin    2        // Midi receive pin
#define midiTXPin    3        // midi send pin
#define ledPin       13       
#define relaisPin    7

#define stepperDelayTime 60   //  delay time in ms to control the stepper motor delaytime. Our tests showed that 8 is about the fastest that can yield reliable operation w/o missing steps
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
bool powerStatus = false;     // for the power outlet/relais
                              
#define MIDI_NOTE_ON 144      //  Midi message 144 = Note ON
#define MIDI_NOTE_OFF 128

// define MIDI messages
const byte midiNoteOn = 144;
const int filterChannel = 0;  // MIDI channel 1
const int filterNoteC = 60;   // middle C(4)
const int filterNoteD = 62;   // D4
const int filterNoteE = 64;   // E4
const int filterNoteF = 65;   // F4

// received MIDI data
byte midiByte;
byte midiChannel;
byte midiCommand;
byte midiNote;
byte midiVelocity;

// states
#define STATE_NONE 0
#define STATE_NOTE_ON 1
#define STATE_NOTE 2
int state;

// keep Midi action (LED) going for actionDuration milliseconds
bool actionOn;
unsigned long actionTime;
unsigned long actionDuration = 100;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, stepperPin1, stepperPin2, stepperPin3, stepperPin4);

// We will use the SoftwareSerial library instead of the Serial library, as this will let us control which pins our MIDI interface is connected to.
SoftwareSerial mySerial(midiRXPin, midiTXPin); // RX, TX

void setup() {
  state = STATE_NONE;
  actionOn = false;   
  myStepper.setSpeed(50);     // set the stepper speed at 60 rpm:
  mySerial.begin(31250);
  delay(100);                 // give MIDI-device a short time to "digest" MIDI messages
  pinMode(ledPin, OUTPUT);
  pinMode(relaisPin, OUTPUT);

}

void loop() {                 // put your main code here, to run repeatedly:
  readMidi();
}
