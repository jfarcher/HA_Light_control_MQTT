/*
 * Ethernet LED on or Off with MQTT 
 * (c) 2013 Jon Archer
 *
 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define MQTT_SERVER "192.168.1.3"

byte mac[]= { 0x91, 0xA3, 0xAA, 0x00, 0x01, 0x03 };
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);

char message_buff[30];

int led1Pin   = 3;   // Red LED,   connected to digital pin 3
int led0Pin  = 5;

EthernetClient ethClient;  // Ethernet object
PubSubClient client( MQTT_SERVER, 1883, callback, ethClient); // MQTT object

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip,gateway,subnet);  // Get connected!
  Serial.print("IP: ");                       // A little debug.. show IP address
  Serial.println(Ethernet.localIP());

  if (client.connect("arduinoClient")) {      // connect to MQTT server
    client.subscribe("device/1/led");             // subscribe to topic for led 1
    client.subscribe("device/0/led");             // subscribe to topic for led 0
    Serial.println("Connected to MQTT");      // let us know this has happened
  }
  pinMode( led1Pin, OUTPUT);
  pinMode( led0Pin, OUTPUT);

}

void loop()
{
  client.loop();                              // loop for ever waiting for MQTT event
}

// handles message arrived on subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print ("New message from broker on:");
  Serial.println (topic);
  Serial.print("Payload:");
  Serial.write(payload, length);
  Serial.println();
if (strcmp(topic,"device/1/led")==0) {
Serial.println((char*)payload);
    if (strcmp((char*)payload,"2-42d")==0){
      Serial.println("2-42 pressed");
      digitalWrite(led0Pin, HIGH);
}
   else  if (strcmp((char*)payload,"2-43d")==0){
      Serial.println("2-43 pressed");
      digitalWrite(led0Pin, LOW);
}

}
if (strcmp(topic,"device/0/led")==0) {
Serial.println((char*)payload);
    if (strcmp((char*)payload,"2-40d")==0){
      Serial.println("2-40 pressed");
      digitalWrite(led1Pin, HIGH);
}
   else  if (strcmp((char*)payload,"2-41d")==0){
      Serial.println("2-41 pressed");
      digitalWrite(led1Pin, LOW);
}

}
}
