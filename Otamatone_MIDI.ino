#include "MIDIUSB.h"

#define CHANNEL 1
#define NOTE 65
#define VELOCITY 127

#define SENSOR_MIN 210
#define SENSOR_MAX 810

#define FLEX_MIN 100
#define FLEX_MAX 1023

#define CC_CHAN_MOD_MSG 1

bool isNoteOn = false;
bool isMouthOpened = false;

int last_flex = 0;

void setup() {
  
}

void loop() {
  int a = analogRead(A0);
  //int b = analogRead(A1);
  
  if (SENSOR_MIN <= a and a <= SENSOR_MAX)
  {
    int sensor = map(a, SENSOR_MIN, SENSOR_MAX, 0, 127);
    
    if (isNoteOn == false)
    {
      noteOn(CHANNEL, NOTE, VELOCITY);
      isNoteOn = true;
    }

    int maxbit = (sensor == 127) ? 127 : 0;
    pitchBendChange(CHANNEL, maxbit, sensor);
  }
  else
  {
    if (isNoteOn == true)
    {
      noteOff(CHANNEL, NOTE, VELOCITY);
      pitchBendChange(CHANNEL, 0, 64);
      isNoteOn = false;
    }
  }

//  if (FLEX_MIN <= b and b <= FLEX_MAX)
//  {
//    int flex = map(b, FLEX_MIN, FLEX_MAX, 1, 127);
//    if (isMouthOpened == false)
//    {
//      isMouthOpened = true;
//    }
//    if (flex != last_flex)
//    {
//      controlChange(CHANNEL, CC_CHAN_MOD_MSG, flex);
//      last_flex = flex;
//    }
//  }
//  else
//  {
//    if (isMouthOpened == true)
//    {
//      controlChange(CHANNEL, CC_CHAN_MOD_MSG, 0);
//      isMouthOpened = false;
//    }
//  }
}

void pitchBendChange(byte channel, byte lvalue, byte mvalue)
{
  midiEventPacket_t event = {0x0E, 0xE0 | channel, lvalue, mvalue};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}
