/**
 * \file
 *       ESP8266 run MQTT and REST API simultaneously
 * \author
 *       Yang Wu
 */
#include <StackList.h>
#include "espduino.h"
#include "mqtt.h"
#include "rest.h"
#include "scheduler.h"

ESP esp(&Serial1, &Serial, 4);
MQTT mqtt(&esp);
REST rest(&esp);
SCHEDULER sch(3, 100); /* Configure the scheduler */

boolean wifiConnected = false;
//boolean mqttReady = false;
int dummyData =25;

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      Serial.println("WIFI CONNECTED");
      mqtt.connect("192.168.2.201", 1884, false);
      wifiConnected = true;
      //or mqtt.connect("host", 1883); /*without security ssl*/
    } else {
      wifiConnected = false;
      mqtt.disconnect();
    }

  }
}

void mqttConnected(void* response)
{
  Serial.println("----------------------- mqtt Connected-------------------------------");
  mqtt.subscribe("/topic/0"); //or mqtt.subscribe("topic"); /*with qos = 0*/
  mqtt.subscribe("/topic/1");
 /* mqtt.subscribe("/topic/2");
  mqtt.subscribe("/topic/3");
  mqtt.subscribe("/topic/4");
  mqtt.subscribe("/topic/5");
  mqtt.subscribe("/topic/6");
  mqtt.subscribe("/topic/7");*/

}
void mqttDisconnected(void* response)
{

}
void mqttData(void* response)
{
  Serial.println("----------------------- mqtt Received-------------------------------");
  RESPONSE res(response);

  Serial.print("Received: topic=");
  String topic = res.popString();
  Serial.println(topic);

  Serial.print("data=");
  String data = res.popString();
  Serial.println(data);

}
void mqttPublished(void* response)
{

}
void setup() {
  Serial.println("ARDUINO: setup the serial communication");
  Serial1.begin(19200);
  Serial.begin(19200);
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  Serial.println("ARDUINO: setup rest client");
  if(!rest.begin("api.thingspeak.com")) {
    Serial.println("ARDUINO: failed to setup rest client");
    while(1);
  }


  if(!mqtt.begin("DVES_duino", "", "", 120, true)) {
    Serial.println("ARDUINO: fail to setup mqtt");
    while(1);
  }
  
  /*setup mqtt events */
  mqtt.connectedCb.attach(&mqttConnected);
  mqtt.disconnectedCb.attach(&mqttDisconnected);
  mqtt.publishedCb.attach(&mqttPublished);
  mqtt.dataCb.attach(&mqttData);

  Serial.println("ARDUINO: setup mqtt lwt");
  mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

  Serial.println("SCHEDULER: Install the method into scheduler");
  sch.addThread(200, &MQTTsend);
  sch.addThread(600, &RESTupdate);
  sch.addThread(3, &MQTTlisten);

  /*setup wifi*/
  Serial.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  esp.wifiConnect("BCRLovs","23456");
  Serial.println("ARDUINO: system started");
  
  
}


void loop() {
  if(wifiConnected) {
    /*if(mqttReady)
    {
      mqtt.publish("/topic/0", "data0");
      delay(15*1000);
    }*/
    sch.RoundRobin();
  }else
  {
    esp.process();
  }
}

void MQTTsend()
{
  mqtt.publish("/topic/0", "data0");
}

void MQTTlisten()
{
  Serial.print(".");
  esp.process();
}

void RESTupdate()
{
  char response[266];
  char buff[64];
  sprintf(buff, "/update?api_key=GPVP0E6QQVWU47LZ&field1=%d", dummyData);
  Serial.println(buff);
  rest.get((const char*)buff);
  Serial.println("ARDUINO: send get");

  if(rest.getResponse(response, 266) == HTTP_STATUS_OK){
    Serial.println("ARDUINO: GET successful");
    Serial.println(response);
    dummyData+=5;
  }
  return;
}

