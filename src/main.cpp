#include <Arduino.h>
#include <PubSubClient.h>
#include "Gsender.h"
#include <SFE_BMP180.h>
#include <Wire.h>

#include "ESPBASE.h"

long lastReconnectAttempt = 0;
String StatusTopic;
String sChipID;

ESPBASE Esp;
SFE_BMP180 pressure;

void setup() {
  Serial.begin(115200);
  char cChipID[10];
  sprintf(cChipID,"%08X",ESP.getChipId());
  sChipID = String(cChipID);

  Esp.initialize();
  StatusTopic = String(DEVICE_TYPE) + "/" + config.DeviceName + "/status";
  customWatchdog = millis();
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
//    while(1); // Pause forever.
  }
  Serial.println("Done with setup");
/*  test mail
  Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
  String subject = "Subject is optional!";
  if(gsender->Subject(subject)->Send("5303133307@tmomail.net", "Setup test")) {
      Serial.println("Message send.");
  } else {
      Serial.print("Error sending message: ");
      Serial.println(gsender->getError());
  } */
}

void loop() {
  Esp.loop();
}

String getSignalString()
{
  String signalstring = "";
  byte available_networks = WiFi.scanNetworks();
  signalstring = signalstring + sChipID + ":";
 
  for (int network = 0; network < available_networks; network++) {
    String sSSID = WiFi.SSID(network);
    if(network > 0)
      signalstring = signalstring + ",";
    signalstring = signalstring + WiFi.SSID(network) + "=" + String(WiFi.RSSI(network));
  }
  return signalstring;    
}

void sendStatus()
{
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char c_payload[length];
  memcpy(c_payload, payload, length);
  c_payload[length] = '\0';
  
  String s_topic = String(topic);
  String s_payload = String(c_payload);
  Serial.print(s_topic + ":" + s_payload);
}

void mqttSubscribe()
{
    if (Esp.mqttClient->connected()) 
    {
        //subscribe to topics here
    }
}


