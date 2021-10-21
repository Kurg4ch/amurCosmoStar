#include <GyverTM1637.h>

byte channel_one[4] = {_C, _h, _0, _1};
byte channel_two[4] = {_C, _h, _0, _2};

const int CLK = 2;
const int DIO = 3;
const int set_channel_but_pin = A0;

bool channel = false;

const int but1_pin = 3;
const int but2_pin = 4;

GyverTM1637 disp(CLK, DIO);


void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  disp.clear();
  disp.brightness(7);
  disp.displayByte(_S, _e, _t, _C);

}

void loop() {

  disp.clear();
  if(Serial1.read == "W"){
    disp.displayByte(_G, _O, _O, _D);
    delay(1000);
  }

  Serial.println(Serial1.read());
  
  if(digitalRead(set_channel_but)){
    channel = !channel;
    digitalWrite(set_channel_pin, LOW);
    delay(40);
      if(channel)
      Serial1.write("AT+C001");
      if(!channel)
      Serial1.write("AT+C100");
      digitalWrite(set_channel_pin, HIGH);
    }
  if (channel)
    disp.displayByte(channel_one);

  if (!channel)
    disp.displayByte(channel_two);

}
