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



int led0Pin  = 6;
int led1Pin   = 7; 
int value; 
int state;
int fadeamount;
char message_buff[100];

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
  }
  delay(10);
  Serial.println();
  if (client.connect("arduinoClient")) {      // connecting to MQTT server
    client.subscribe("house/room1/1");             // subscribe to topic for room 1 light 1
    client.subscribe("house/room1/2");             // subscribe to topic for room 1 light 2
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
  int i = 0;
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);
  
  
  if (strcmp(topic,"house/room1/1")==0) {
    
//    Serial.println((char*)payload);
    Serial.println(msgString);
    if (msgString.equals("2-40")){
//    if (strcmp((char*)payload,"2-402")==0){
      Serial.println("2-40 pressed");
      for(value=0; value<=255; value+=5) // fade in (from min to max) 
      { 
        analogWrite(led0Pin, value);           // sets the value (range from 0 to 255) 
        delay(50);                            // waits for 30 milli seconds to see the dimming effect 
       }

  
}
   else     if (msgString.equals("2-41")){
      Serial.println("2-41 pressed");
      for(value = 255; value >=0; value-=5)   // fade out (from max to min) 
      { 
        analogWrite(led0Pin, value); 
        delay(30); 
      }  
}
   else     if (msgString.equals("stat")){
      Serial.println("status requested");
      state = digitalRead(led0Pin);
      if(state == 1){
        client.publish("house/room1/1/stat","1");
        Serial.println("Light is on");
      }
      else if (state == 0){
        client.publish("house/room1/1/stat","0");
        Serial.println("Light is off");
      }
    }

}

}
