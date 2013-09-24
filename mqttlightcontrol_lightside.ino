/*
 * Digital pin On or Off based on MQTT message
 * (c) 2013 Jon Archer
 *
 */

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <OneWire.h>

#define MQTT_SERVER "192.168.1.3"

byte mac[]= { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
OneWire ds(2);
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);

char message_buff[30];

int led1Pin   = 3;   // Red LED,   connected to digital pin 3
int led0Pin  = 5;

EthernetClient ethClient;  // Ethernet object
PubSubClient client( MQTT_SERVER, 1883, callback, ethClient); // MQTT object

void setup() {
  byte i;
  byte dsAddress[8];
  
  
  Serial.begin(9600);
  delay( 500 );
  Serial.print ("Searching for DS18B20...");
  ds.reset_search(); // Start the search with the first device
  
  if ( !ds.search(dsAddress) )
  {
    Serial.println("none found. Using specified MAC Address.");
  } else {
    Serial.print( "Success! \nSetting MAC address...." );

  mac[1] = dsAddress[3];
  mac[2] = dsAddress[4];
  mac[3] = dsAddress[5];
  mac[4] = dsAddress[6];
  mac[5] = dsAddress[7];
  }
  Serial.print( "Ethernet MAC =" );
  for( i = 0; i < 6; i++ )
  {
    Serial.write( ' ' );
    Serial.print( mac[i], HEX );
  }
  Serial.println();
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }
Serial.print("IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  if (client.connect("arduinoClient")) {      // connecting to MQTT server
    client.subscribe("device/1/led");             // subscribe to topic for led 1
    client.subscribe("device/0/led");             // subscribe to topic for led 0
    Serial.println("Connected to MQTT");      // Milestone for connection debugging
  }
  pinMode( led1Pin, OUTPUT);
  pinMode( led0Pin, OUTPUT);

}

void loop()
{
  client.loop();                              // loop for ever waiting for MQTT event
}

// handles message arrived on subscribed topic
//Serial prints are for debugging purposes and can safely be removed
//not sure what is going on with the suffixed d on the payloads, need to work that one out.
void callback(char* topic, byte* payload, unsigned int length) {
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
