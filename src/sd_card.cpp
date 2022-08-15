#include "FS.h"
#include "SPI.h"
#include "SD_MMC.h"

bool initSDCard()
{
  Serial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin())
  {
    Serial.println("MicroSD Card Mount Failed");
    return false;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No MicroSD Card found");
    return false;
  }

  return true;
}

char *readFile(const char *path)
{
  Serial.printf("reading file: %s\n", path);

  File file = SD_MMC.open(path);
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

  Serial.println("Buffer filled\n");
  buffer[count] = 0; //< Added
  char *convert = (char *)buffer;

  Serial.println("Closing file\n");
  file.close();
  return convert;
}
