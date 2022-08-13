#include <ArduinoJson.h>

DynamicJsonDocument deserialize(char* json)
{
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
  DynamicJsonDocument jsonDocument(capacity);

  deserializeJson(jsonDocument, json);
  return jsonDocument;
  // const char *json = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.75608,2.302038]}";
  // const char *sensor = jsonDocument["sensor"];
  // long time = jsonDocument["time"];
  // double latitude = jsonDocument["data"][0];
  // double longitude = jsonDocument["data"][1];
}