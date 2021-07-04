#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_ssd1306syp.h>
#define SDA_PIN 14
#define SCL_PIN 12
int dark = 0; //黑暗模式标记
int counter = 0;
int hours;
Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 3600, 60000); //ntp服务器设置，阿里云  
void timedisplay()
{
  display.clear();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.print("IP:");
  display.setCursor(25, 0);
  display.println(WiFi.localIP());
  display.print(" ");
  display.print(Soc_cont(voltage));
  display.println("%");
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print(response.substring(13, 23));
  display.setCursor(20, 40);
  display.print(timeClient.getFormattedTime());
  display.update();
}
void sleepdisplay()
{
  display.clear();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print("Time To");
  display.setCursor(30, 40);
  display.print("Sleep~~");
  display.update();
}
void diaplaymodeset()
{
  if ((hours >= 23 || hours <= 6) && dark == 0)
  {
    display.initializedark();
    dark = 1;
  }
  else if ((hours > 6 && hours < 23) && dark == 1)
  {
    display.initialize();
    dark = 0;
  }
}
void setup()
{
  display.initialize();
  dark = 0;
  Serial.begin(115200);
  delay(100);

  display.clear();
  display.setTextSize(2);
  display.setCursor(17, 10);
  display.println("Tingting");
  display.setTextSize(1);
  display.setCursor(20, 40);
  display.println("Project::BOX");
  display.update();
  delay(5000);

  if (!AutoConfig())
  {

    display.clear();
    display.setTextSize(2);
    display.setCursor(10, 10);
    display.println("SmartConfig");
    display.update();
    SmartConfig();
  }

  display.clear();
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("CONNECTED");
  display.update();
  delay(1000);
  }
  void loop()
{
  timeClient.update();
  hours = timeClient.getHours() + 7;
  diaplaymodeset();
  if (hours == 0 && timeClient.zhengHours() == 0)
  {
    for (int i = 0; i <= 10; i++)
    {
      timedisplay();
      delay(200);
      sleepdisplay();
      delay(200);
    }
  }
  else if (timeClient.zhengHours() == 0)
  {
    for (int i = 0; i <= 10; i++)
    {
      timedisplay();
      delay(200);
      display.clear();
      display.update();
      delay(200);
    }
  }
  else
    timedisplay();
  delay(200);
}
