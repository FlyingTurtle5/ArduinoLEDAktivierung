/*
 * 
 */

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
// initialize LED digital pin as an output.
//pinMode(LED_BUILTIN, OUTPUT);
int redPin = D7;
int greenPin = D6;
int bluePin = D5;

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived[");
  Serial.print(topic);
  Serial.print("]");
  for(int i = 0; i<length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
  setColor(0, 255, 0); //Green
  delay(1000);
}

void reconnect(){
  //Loop until we're reconnected
  while(!client.connected()){
    Serial.print("Attemting MQTT connection...");
    if(client.connect("client")){
      Serial.println("connected");
      client.publish("/status/devices","I'm here");
      client.subscribe("/status/in");
    }else{
      Serial.print("failed=,rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(115200);
  Serial.println("hallo Welt");

  WiFi.mode(WIFI_STA);
  WiFi.hostname("irgendwas");
  WiFi.begin("hsb-labor","6MVfNSqdMr5SZo6d");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  client.setServer("192.168.206.22",1883);
  client.setCallback(callback);
}

void loop() {
  setColor(0, 0, 255); // Blue
  if(!client.connected()){
    setColor(255,0,0); // Red
    delay(1000);
    reconnect();
  }
  client.loop();
}
