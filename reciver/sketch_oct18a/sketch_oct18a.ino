#include <GyverTM1637.h>
#include <GyverButton.h>
#include <SoftwareSerial.h>

byte channel_one[4] = {_C, _h, _0, _1};
byte channel_two[4] = {_C, _h, _0, _2};
byte channel_three[4] = {_C, _h, _0, _3};

const int CLK = 2;
const int DIO = 3;
const int set_channel_but_pin = A0;

int channel = 0;

const int but1_pin = 3;
const int but2_pin = 4;

GyverTM1637 disp(CLK, DIO);
GButton butt1(but1_pin);
GButton butt2(but2_pin);

SoftwareSerial Serial1 (1, 0);

void setup() {
  Serial1.begin(9600);
  disp.clear();
  disp.brightness(7);
  disp.displayByte(_S, _e, _t, _C);

}

void loop() {
  if (butt1.isSingle()) {
    channel += 1;
  }

  if (butt1.isDouble()) {
    if (channel == 1)
      Serial1.print("AT+C001");
    if (channel == 2)
      Serial1.print("AT+C025");
    if (channel == 3)
      Serial1.print("AT+C050");
  }
  if (channel == 1)
    disp.displayByte(channel_one);

  if (channel == 2)
    disp.displayByte(channel_two);

  if (channel == 3)
    disp.displayByte(channel_three);

}
