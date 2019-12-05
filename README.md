# Christmas IOT LEDSTRIP and music controller
A project to control an ledstrip and an 8 ohm speaker playing christmas music from my phone
The reason for using two microcontrollers is because the library for plying the music files from the sd card doesn't work on the nodemcu i therefore use a arduino nanu for controllig the music and a nodemcu for controller the ledstrips and runnning an http server

## What i learned
- Playing music from an arduino with an 8ohm speaker
- Sending commands to a wifi connected nodeMCU
	- An arduino like board build on the ESP8266
- Setting up communication between to microcontrollers using serial
