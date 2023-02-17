#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#define countPin 4

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C

int count = 0;
int prevCount = 0;
boolean drawn = false;

void setup(void) {
  u8g2.begin();
  pinMode(countPin, INPUT);
}

void loop(void) {
  if(!digitalRead(countPin)){
    count += 1;
    while(!digitalRead(countPin));
  }

  if(prevCount != count){
    drawn = false;
    prevCount = count;
  }

  if(!drawn){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_inr24_mn);
    char buf[4];
    sprintf(buf, "%d", count);
    u8g2.drawStr(70,44,buf);
    u8g2.sendBuffer();
  }
}