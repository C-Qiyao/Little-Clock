#include <ESP8266WiFi.h>
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
void SmartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig...");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    delay(500); // wait for a second
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;
    }
  }
}
bool AutoConfig()
{
  WiFi.begin();
  //如果觉得时间太长可改
  for (int i = 0; i < 10; i++)
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
      Serial.println("WIFI SmartConfig Success");
      Serial.printf("SSID:%s", WiFi.SSID().c_str());
      Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
      Serial.print("LocalIP:");
      Serial.print(WiFi.localIP());
      Serial.print(" ,GateIP:");
      Serial.println(WiFi.gatewayIP());

      return true;
    }
    else
    {
      Serial.print("WIFI AutoConfig Waiting......");
      Serial.println(wstatus);

      display.clear();
      display.setTextSize(2);
      display.setCursor(10, 10);
      display.println("AutoConfig Waiting");
      display.update();
      delay(1000);
    }
  }
  Serial.println("WIFI AutoConfig Faild!");

  display.clear();
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("EspTouch");
  display.update();
  delay(500);
  return false;
}
void timedisplay()
{
  display.clear();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.print("IP:");
  display.setCursor(25, 0);
  display.println(WiFi.localIP());
  display.setTextSize(2);
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
