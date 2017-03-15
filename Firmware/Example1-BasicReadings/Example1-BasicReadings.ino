/*
  AS726x Spectral Sensor
  By: Nathan Seidle
  SparkFun Electronics
  Date: March 8th, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Read the various spectrums.

  Hardware Connections (Breakoutboard to Arduino):
  3.3V = 3.3V
  GND = GND
  SDA = A4
  SCL = A5

  Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>

#define AS726X_ADDR 0x49 //7-bit unshifted default I2C Address

//Register addresses
#define AS72XX_SLAVE_STATUS_REG 0x00
#define AS72XX_SLAVE_WRITE_REG 0x01
#define AS72XX_SLAVE_READ_REG 0x02

#define AS72XX_SLAVE_TX_VALID 0x02
#define AS72XX_SLAVE_RX_VALID 0x01

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("AS726x Read Example");

  Wire.begin();

  byte incoming = virtualReadRegister(0x01);
  Serial.print("incoming: 0x");
  Serial.println(incoming, HEX);

  byte temp = virtualReadRegister(0x06);
  Serial.print("temp: 0x");
  Serial.println(temp, HEX);

  //configureCCS811(); //Turn on sensor

  //unsigned int result = getBaseline();
}

void loop()
{
  delay(1000); //Wait for next reading
}

//Returns the baseline value
//Used for telling sensor what 'clean' air is
//You must put the sensor in clean air and record this value
unsigned int getBaseline()
{
  Wire.beginTransmission(AS726X_ADDR);
  //  Wire.write(CSS811_BASELINE);
  Wire.endTransmission();

  Wire.requestFrom(AS726X_ADDR, 2); //Get two bytes

  byte baselineMSB = Wire.read();
  byte baselineLSB = Wire.read();

  unsigned int baseline = ((unsigned int)baselineMSB << 8) | baselineLSB;

  return (baseline);
}

//Read a virtual register from the AS726x
byte virtualReadRegister(byte virtualAddr)
{
  byte status;

  while (1)
  {
    // Read slave I2C status to see if the read buffer is ready.
    status = readRegister(AS72XX_SLAVE_STATUS_REG);

    if ((status & AS72XX_SLAVE_TX_VALID) == 0)
      // No inbound TX pending at slave. Okay to write now.
      break;

    delay(500);
  }
  
  // Send the virtual register address (setting bit 7 to indicate a pending write).
  writeRegister(AS72XX_SLAVE_WRITE_REG, virtualAddr);

  while (1)
  {
    // Read the slave I²C status to see if our read data is available.
    status = readRegister(AS72XX_SLAVE_STATUS_REG);
    if ((status & AS72XX_SLAVE_RX_VALID) != 0)
      // Read data is ready.
      break;

    delay(500);
  }

  // Read the data to complete the operation.
  byte incoming = readRegister(AS72XX_SLAVE_READ_REG);
  return(incoming);
}

//Reads from a give location from the AS726x
byte readRegister(byte addr)
{
  Wire.beginTransmission(AS726X_ADDR);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom(AS726X_ADDR, 1);
  if(Wire.available())
  {
    return (Wire.read());
  }

  return (0xFE);
}

//Write a value to a spot in the AS726x
void writeRegister(byte addr, byte val)
{
  Wire.beginTransmission(AS726X_ADDR);
  Wire.write(addr);
  Wire.write(val);
  Wire.endTransmission();
}
