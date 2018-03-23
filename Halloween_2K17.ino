/******************************************************************
 Created with PROGRAMINO IDE for Arduino - 31.10.2017 12:01:00
 Project     : Halloween Costume 2017
 Libraries   : Serial
 Author      : Blayne Robinson
 Description : A mask containing blue and white lighting is controlled by
               sensors hidden on the user's person. A sound sensor beneath
               the user's neck tie measures voice volume and environmental sound.
               A pressure sensor beneath a shoe's sole messures the pressure
               of the user's footstep.
******************************************************************/



#include <SoftwareSerial.h>

//Set PWM Output pins for light controls
const int blueLight = 10;
const int whiteLight = 11;
//Set Input pins
const int footSensor = 5;
const int soundSensor = 6;
//Set button pins
const int buttonSwitch = 7;

//Setting controls how the lights are affected by the sensors
int setting = 0;
//Brightness levels of lights
int blueLevel = 255;
int whiteLevel = 0;

void setup() {
  Serial.begin(9600);
  //Set pins
  pinMode(blueLight, OUTPUT);
  pinMode(whiteLight, OUTPUT);
  pinMode(footSensor, INPUT);
  pinMode(soundSensor, INPUT);
  pinMode(buttonSwitch, INPUT);
}

void loop() {
//Run different lighting program depending on current setting
  switch (setting) {
    case 0:
      soundPressureWhite();
      break;
    case 1:
      footPressureWhite();
      break;
    case 2:
      footBlueSoundWhite();
      break;
  }
  
//Check for button press
  if (analogRead(buttonSwitch) > 500) {
    //Switch Settings
    switchCheck();
  }
  //Monitor lighting brightness
  Serial.println(blueLevel);
  Serial.println(whiteLevel);
  
  //Ensure the brightness can't exceed maximum possible value
  overCheck();
  
  //Set lighting brightness
  analogWrite(whiteLight, whiteLevel);
  analogWrite(blueLight, blueLevel);

}

//SETTINGS//

void footPressureWhite() {
  //Pressure sensor controls white light directly, blue light always on maximum brightness
  int pressureLevel = analogRead(footSensor);
  if (pressureLevel > 400) {
    if (pressureLevel > 999) {
      pressureLevel = 999;
    }
    blueLevel = 255;
    whiteLevel = (round((pressureLevel - 400) * 0.425));
  }
  if (pressureLevel < 400) {
    whiteLevel = 0;
  }
}

void soundPressureWhite() {
  //Sound sensor controls white light while blue light is set at maximum value
  int pressureLevel = analogRead(soundSensor);
  if (pressureLevel > 510) {
    pressureLevel = 510;
  }
  blueLevel = 255;
  //White light brightness fades if a volume requirement threshold is not met to prevent strobe effect
  whiteLevel -= 8;
  if (whiteLevel < 0) {
    whiteLevel = 0;
  }
  if (pressureLevel > 100) {
    whiteLevel = (round(pressureLevel / 2));
  }
}

void footBlueSoundWhite() {
  //Blue light is controlled by pressure sensor, white light is controlled by sound sensor
  int SpressureLevel = analogRead(soundSensor);
  int FpressureLevel = analogRead(footSensor);

  if (SpressureLevel > 510) {
    SpressureLevel = 510;
  }

  if (FpressureLevel > 400) {
    if (FpressureLevel > 999) {
      FpressureLevel = 999;
    }
    FpressureLevel = (round((FpressureLevel - 400) * 0.425));
  } else {
    FpressureLevel = 0;
  }

//Fade both lights until a dim constant is reached

  if (whiteLevel > 20) {
    whiteLevel -= 6;
  }
  if (blueLevel > 20) {
    blueLevel -= 2;
  }
  //If high enough values are read, set the lighting brightness with both sensors
  if (FpressureLevel > 240) {
    blueLevel += (round(FpressureLevel)) - 20;
  }
  if (SpressureLevel > 100) {
    whiteLevel = (round(SpressureLevel / 2)) - 20;
  }
}

////

void switchCheck() {
  //Cycle lighting settings once per button press
  if (setting < 2 ) {
    setting += 1;
    delay(1000);
  } else {
    setting = 0;
    delay(1000);
  }

}

void overCheck() {
  //Prevent lighting from exceeding maximum value
  if (blueLevel > 255) {
    blueLevel = 255;
  }
  if (whiteLevel > 255) {
    whiteLevel = 255;
  }
}

