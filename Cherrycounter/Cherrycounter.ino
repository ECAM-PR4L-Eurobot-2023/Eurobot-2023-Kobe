#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>


#define countPin 4
#define REBOUND_TIME 50


uint32_t timeBetweenCounter = 0;
int count = 0;
int prevCount = 0;
boolean drawn = false;

int score = 25;



U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // High speed I2C

void countCherry()
{
  if ((millis() - timeBetweenCounter) > REBOUND_TIME) {
    timeBetweenCounter = millis();
    count += 1;
    if (count>=20){
      count = 20;
    }
  }
}


void setup(void)
{
  Serial.begin(115200);
  u8g2.begin();
  pinMode(countPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(countPin), countCherry, FALLING);

}

void loop(void)
{
  if(prevCount != count){
    drawn = false;
    prevCount = count;
  }


  if(!drawn ){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_inr24_mn);
    char bufCherries[13];
    char bufScore[11];
    sprintf(bufCherries, "Cherries :%d", count);
    if (count!=0){
      sprintf(bufScore, "Score :%d", score+5+count);
    }
    else{
      sprintf(bufScore, "Score :%d", score);
    }
    
    // u8g2.drawStr(70-(20* ((count>9? 1: 0)+(count>99? 1: 0))),10,buf);
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0, 20,bufCherries);
    u8g2.drawStr(0, 50, bufScore);

    u8g2.sendBuffer();
  }

}