#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include <WiFi.h>

#define ROSSERIAL_ARDUINO_TCP

#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Empty.h>

#define countPin 4

const char *ssid = "Eurobot-WiFi";
const char *password = "robotseverywhere";


int count = 0;
int prevCount = 0;
boolean drawn = false;

std_msgs::Int16 msgInt;

ros::Publisher sendCherry("kobe/sendCherry", &msgInt);
void getCallback(const std_msgs::Empty &msg)
{
  msgInt.data = count;
  sendCherry.publish(&msgInt);
}

IPAddress server(10, 42, 0, 100);
const uint16_t serverPort = 11411;

ros::NodeHandle nh;


ros::Subscriber<std_msgs::Empty> sub("kobe/getCherry", &getCallback);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // High speed I2C




void setup(void)
{
  Serial.begin(115200);
  u8g2.begin();
  pinMode(countPin, INPUT);

  WiFi.begin(ssid, password);
  unsigned long start = millis();
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED && millis()-start<10000)
  {
    delay(500);
    Serial.println("...");
  }

 if (WiFi.status() == WL_CONNECTED){
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  nh.advertise(sendCherry);
  nh.subscribe(sub);
 }

}

void loop(void)
{
  if (WiFi.status() == WL_CONNECTED){
  nh.spinOnce();
  delay(10);
  }

  // Serial.println(count);

  if (!digitalRead(countPin))
  {
    count += 1;
    while (!digitalRead(countPin))
      ;
  }

  if (prevCount != count)
  {
    drawn = false;
    prevCount = count;
  }

  if (!drawn)
  {
    Serial.println("drawing");
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_inr24_mn);
    char buf[4];
    sprintf(buf, "%d", count);
    u8g2.drawStr(70 - (20 * ((count > 9 ? 1 : 0) + (count > 99 ? 1 : 0))), 44, buf);
    u8g2.sendBuffer();
    drawn = true;
  }

}