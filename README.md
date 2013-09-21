mqttlightcontrol_lightside
==========================

Arduino sketch to recieve MQTT based messages and turn on/off digital ports.

This sketch uses the PubSubClient MQTT libraries https://github.com/knolleary/pubsubclient

It reads switch status' from MQTT and changes the status of digital ports accordingly, the idea is to use this to control multiple relays or MOSFETs to turn lights on or off (or any device for that matter).


