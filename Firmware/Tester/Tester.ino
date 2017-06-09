/*
  Using the AS726x Spectral Sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: March 8th, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Does basic test to read raw data and blink status and illumination LEDs.

  Hardware Connections:
  Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  Plug the AS726x onto the shield
  Serial.print it out at 115200 baud to serial monitor.

*/

#include <Wire.h>


byte sensorVersion = 0; //Keeps track of whether we have an AS7262 or AS7263
#define SENSORTYPE_AS7262 0x3E
#define SENSORTYPE_AS7263 0x3F

void setup()
{
  Serial.begin(115200);
  //while (!Serial);
  Serial.println("AS726x Read Example");

  Wire.begin();
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void loop()
{
  //Wire.begin();
  
  sensorVersion = as726xSetup();
  if (sensorVersion == 0)
  {
    Serial.println("Sensor failed to respond. Check wiring.");
    //while (1); //Freeze!
  }
  else
  {
    if (sensorVersion == SENSORTYPE_AS7262) Serial.println("AS7262 online!");
    if (sensorVersion == SENSORTYPE_AS7263) Serial.println("AS7263 online!");

    enableIndicator();
    delay(250);
    disableIndicator();

    //Ow!
    enableBulb();
    delay(10);
    disableBulb();

    takeMeasurements(); //No LED - easier on your eyes
    if (sensorVersion == SENSORTYPE_AS7262)
    {
      //Visible readings
      Serial.print(" Reading: V[");
      Serial.print(getViolet());
      Serial.print("] B[");
      Serial.print(getBlue());
      Serial.print("] G[");
      Serial.print(getGreen());
      Serial.print("] Y[");
      Serial.print(getYellow());
      Serial.print("] O[");
      Serial.print(getOrange());
      Serial.print("] R[");
      Serial.print(getRed());
    }
    else if (sensorVersion == SENSORTYPE_AS7263)
    {
      //Near IR readings
      Serial.print(" Reading: R[");
      Serial.print(getR());
      Serial.print("] S[");
      Serial.print(getS());
      Serial.print("] T[");
      Serial.print(getT());
      Serial.print("] U[");
      Serial.print(getU());
      Serial.print("] V[");
      Serial.print(getV());
      Serial.print("] W[");
      Serial.print(getW());
    }
    Serial.println("]");

  }

  //delay(500);
}
