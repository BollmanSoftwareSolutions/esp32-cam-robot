/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-microsd-card/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

bool initSDCard()
{
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return false;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return false;
  }

  Serial.printf("SD Card Type [MMC=1|SDSC=2|SDHC=3|UNK=???]: %i\n", cardType);

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  return true;
}

const char *readFile(fs::FS &fs, const char *path)
{
  File file = fs.open(path);
  if (!file)
  {
    Serial.println("no file");
    return 0x00;
  }
  int count = file.size();
  unsigned char *buffer = (unsigned char *)ps_malloc(count + 1); //< Updated

  Serial.printf("Bytes: %d\n", count);
  Serial.printf("Count: %d\n", sizeof(buffer));

  for (int i = 0; i < count; i++)
  {
    buffer[i] = (unsigned char)file.read();
  }
  buffer[count] = 0; //< Added
  const char *convert = (const char *)buffer;

  file.close();
  return convert;
}
