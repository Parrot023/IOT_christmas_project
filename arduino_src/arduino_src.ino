#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

File dir;
int count = 0;

//List of songs on SD card
//A max of 10 songs
File song_list[10];

//Defining variables
int input_state;
int input_state_before;    
int input = 3;
int data;

File song;

void setup(){

  //Setting speakerpin
  tmrpcm.speakerPin = 9;
  //Sarting serial commmuniction
  Serial.begin(9600);

  //Checks to see if SD card failed
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }

   dir = SD.open("/");

   int iterations = 0;

   while (true) {
    
    File new_song = dir.openNextFile();
    tmrpcm.play(new_song.name());
    //Serial.println(new_song.name());

    //Breaking if no more songs
    if (!new_song) {
      Serial.println("No more songs");
      break;
    }

    //Adding song to list if max number of songs have not been reached
    if (iterations > 8) {
      Serial.println("Max number of songs reached");
      break;
    } else {
      Serial.print("Added ");
      Serial.print(new_song.name());
      Serial.println(" to song list");
      song_list[iterations] = new_song;
    }
    
    iterations ++;
    
  }
  
  tmrpcm.setVolume(5);
  song = song_list[0];
  tmrpcm.play(song.name());
  Serial.println("playing ");
  Serial.println(song.name());

  
  
}

void loop(){  

  data = Serial.read();
  //Serial.println(data);
  if (data == 'T') {
    Serial.println("RECIVED IT");
    if (tmrpcm.isPlaying()) {
      tmrpcm.stopPlayback();
      //You need to bring the pin to low.
      //Otherwise you will get noise from the speaker
      digitalWrite(9,LOW);
    } else {
      song = song_list[0];
      tmrpcm.play(song.name());
    }
  }
// 
}



