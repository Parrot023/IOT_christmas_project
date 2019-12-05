
#include <ESP8266WiFi.h>

//For some reason it is needed to write D5 instead of just 5 when using the nodeMCU
//Led pins
int blue = D5; //BLUE
int green = D6; //GREEN
int red = D7; //RED

//Led for showing when connected to wifi
int connection_led_pin = D4;

//pir for motion detection
int pir_pin = D2;
//pin for sending signal to second arduino for playing audio
int audio_pin = D3;

//setting pir state for detecting change
int pir_state = 0;
int pir_state_before = 0;

//To know if the pir sensor is active
//This is for the pir sensor to not detect motion all the time
//And turn on and of the music and the leds
int pir_active = 1;

//Number of milliseconds since pir sensor was last active
int pir_timer = 0;

//Wifi credentials
const char* ssid = "Linksys12891";
const char* password = "5njh1d3ihg";

String message = "";


//Starting server
WiFiServer server(80);


void setup() {

  //Setting pin modes
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(audio_pin, OUTPUT);
  pinMode(connection_led_pin, OUTPUT);
  pinMode(pir_pin, INPUT);
  
  
  //Setting all led pins to low  
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  
  //Starts serial communication
  Serial.begin(9600);

  //10 second delay to make sure serial is running before first print
  delay(10);

  // connecting to wifi -----------------------------------------------------
  Serial.print("Connecting to: ");
  Serial.println(ssid); 

  WiFi.begin(ssid,password);
  
  //attempt to set custom ip does'nt work idk why
  IPAddress ip(192,168,1,200); 

  //------------------------------------------------------------------------
  

  //Printing dot every 0.5 seconds if not connected to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Prints connected and turning on green led when connected
  Serial.println("\nCONNECTED");
  digitalWrite(connection_led_pin, HIGH);

  //begins server and prints info for easy connection from browser
  //from mobilephone or pc etc.
  server.begin();
  Serial.println("Server started");
  Serial.print("URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  //Print out a little guide
  Serial.println("Commands:\n LedStrip:\nled=red to turn ledstrip red\nled=green to turn green\nled=blue to turn blue\nled=off to turn off\nMusic:\nmusic=toggle to toggle music"); 
  Serial.print("Example: type http://");
  Serial.print(WiFi.localIP());
  Serial.print("/led=green in your browser to turn the ledstrip green");

}

void loop() {

  //Update pir states
  pir_state_before = pir_state;
  pir_state = digitalRead(pir_pin);

  //Activates pir sensor every hour
  if (millis() - pir_timer == 1000 * 60 *60) {
    pir_active = 1;
  }

  //checks if the pir sensor went from low to high
  if (pir_state == 1 and pir_state_before == 0) {
    pir_interrupt();
  }

  //Serial.println(digitalRead(pir_pin));

  WiFiClient client = server.available();

  //checks for client on server
  if (!client) {
    return;
  }

  //prints new client if client
  //Serial.println("NEW CLIENT");
  while (!client.available()) {
    delay(1);
  }

  //gets the request
  String request = client.readStringUntil('\r');
  //Serial.println(request);
  client.flush();

  String value = "";

  
  //checks if request matches what we want ----------------------------------------
  if (request.indexOf("/led=blue") != -1) {
    digitalWrite(blue, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    message = "The LEDSTRIP is now turned blue";
  } 

  if (request.indexOf("/led=green") != -1) {
    digitalWrite(green, HIGH);
    digitalWrite(blue, LOW);
    digitalWrite(red, LOW);
    message = "The LEDSTRIP is now turned green";
  } 

  if (request.indexOf("/led=red") != -1) {
    Serial.println("red");
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
    message = "The LEDSTRIP is now turned red";
  } 
  
  if (request.indexOf("/led=off") != -1) {
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    message = "The LEDSTRIP is now turned off";
    
  } 

  if (request.indexOf("/music=toggle") != -1) {
    audio_toggle();
    message = "The music has now changed state";
  } 
  // ------------------------------------------------------------------------------------

  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one (idk why, i guess its for the new line)
  client.println("<!DOCTYPE HTML>");
  //Sending back a message to the client
  client.println("<html><body><p>" + message + "</p></body></html>");

  
}

//Function to run when pir is activated
void pir_interrupt() {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, LOW);
  if (pir_active == 1) {
    audio_toggle();
    pir_active = 0;
    pir_timer = millis();
  }
}

void audio_toggle() {
  //Send T to the Arduino to turn on the music
  //T for toggle
  Serial.print('T');
}




