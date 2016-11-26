#include <Adafruit_NeoPixel.h>

#define PIN D5

#include <ESP8266WiFi.h>
#include "TimeClient.h"
#include <WiFiUdp.h>

long lastUpdate = millis();
long lastSecond = millis();

String hours, minutes, seconds;
int currentSecond, currentMinute, currentHour;

char ssid[] = "xxxxxxx";  //  your network SSID (name)
char pass[] = "xxxxxxx";       // your network password

const float UTC_OFFSET = 0;
TimeClient timeClient(UTC_OFFSET);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  strip.begin();
  strip.setBrightness(128);
  strip.show();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.updateTime();
  updateTime() ;
  lastUpdate = millis();
  lastSecond = millis();
}

void loop()
{
  if ((millis() - lastUpdate) > 1800000) updateTime();

  if ((millis() - lastSecond) > 1000)
  {
    strip.setPixelColor(currentSecond / 2.5, 0, 0, 0);
    strip.setPixelColor(currentMinute / 2.5, 0, 0, 0);
    strip.setPixelColor(currentHour * 2, 0, 0, 0);

    strip.show();
    lastSecond = millis();
    currentSecond++;
    if (currentSecond > 59)
    { currentSecond = 0;
      currentMinute++;
      if (currentMinute > 59) {
        currentMinute = 0;
        currentHour++;
        if (currentHour > 12) currentHour = 0;
      }
    }
    String currentTime = String(currentHour) + ':' + String(currentMinute) + ':' + String(currentSecond);
    Serial.println(currentTime);

    strip.setPixelColor(currentSecond / 2.5, 0, 0, 255);
    strip.setPixelColor(currentMinute / 2.5, 0, 255, 0);
    strip.setPixelColor(currentHour * 2, 255, 0, 0);


    strip.show();
  }

}

void updateTime() {
  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
  seconds = timeClient.getSeconds();
  currentHour = hours.toInt();
  if (currentHour > 12) currentHour = currentHour - 12;
  currentMinute = minutes.toInt();
  currentSecond = seconds.toInt();
  lastUpdate = millis();
}


