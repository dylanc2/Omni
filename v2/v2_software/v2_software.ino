/*
Description: Sketch for wireless tongue operated mouse.
Date: November 2020
*/

#include <SoftwareSerial.h>  

const int bluetoothTx = 10;    // TX pin of RN-42, Arduino D10 (configure as RX pin of Ard)
const int bluetoothRx = 9;    // RX pin of RN-42, Arduino D9 (configure as TX pin)
const int button = 7;
const int xAxis = A1;         // joystick X axis
const int yAxis = A0;         // joystick Y axis
int range = 10;               // output range of X or Y movement
int responseDelay = 20;        // response delay of the mouse, in ms
int threshold = range / 5;    // resting threshold
int center = range / 2;       // resting position value

SoftwareSerial BT(bluetoothTx, bluetoothRx); // args SoftwareSerial(RXpin, TXpin)

void mouseCommand(byte buttons, byte x, byte y) {
  BT.write((byte)0xFD);
  BT.write((byte)0x05);
  BT.write((byte)0x02);
  BT.write((byte)buttons);
  BT.write((byte)x);
  BT.write((byte)y);
  BT.write((byte)0x00);
}

int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);
  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023*.6, 0, range);
  /* threshold prevents the mouse from reading small unintentional movements. 
  If the output reading is outside from the rest position threshold, use it: */
  int distance = reading - center;
  if (abs(distance) < threshold) {
    distance = 0;
  }
  // return the scaled distance for this axis:
  return distance;
}

void setup() {
  pinMode(button, INPUT);
  pinMode(bluetoothTx, INPUT); // bluetoothTX is equivalent to arduino RX pin
  pinMode(bluetoothRx, OUTPUT);
  Serial.begin(115200);  // Begin the serial monitor at 115200bps
  BT.begin(115200);  // RN-42 defaults to 115200bps
}

void loop() {
  // read and scale the two axes:
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis)*-1;
  int buttonState = digitalRead(button);
  //int buttonState = 0;

  // move the mouse:
  mouseCommand(buttonState, xReading, yReading);
  delay(responseDelay);
 }
