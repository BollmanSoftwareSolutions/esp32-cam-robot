#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SD.h"
#include "web_server.h"

// Replace with your network credentials
const char *ssid = "WIN_400433";
const char *password = "4nbw4pmb8x";
bool validSD;
bool light;
int pwm;
char *quality;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
WebServerWrapper wrapper(80);

void initWiFi(const char *ssid, const char *password);
const char *readFile(fs::FS &fs, const char *path);
bool initSDCard();

void initInterfaceEndpoints(AsyncWebServer server);

void setup()
{
  Serial.begin(115200);
  validSD = initSDCard();
  if (validSD)
  {
    initSettings();
  }

  initWiFi(ssid, password);
  wrapper.initInterfaceEndpoints();
  wrapper.initGet("/info", infoHandler);
  wrapper.initGet("/direction", directionHandler);
  wrapper.initGet("/pwm", pwmHandler);
  wrapper.initGet("/quality", qualityHandler);
  wrapper.initGet("/light", lightHandler);

  wrapper.begin();
}

void loop()
{
}

void initSettings()
{
  const char *settings = readFile(SD, "/settings.json");

  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3) + 60;
  DynamicJsonDocument jsonDocument(capacity);
  deserializeJson(jsonDocument, settings);

  ssid = jsonDocument["sensor"];
  password = jsonDocument["sensor"];
}

void directionHandler(AsyncWebServerRequest *request)
{
  int args = request->args();
  std::string outputs("{");
  for (int i = 0; i < args; i++)
  {
    outputs.append("\"");
    outputs.append(request->argName(i).c_str());
    outputs.append("\":\"");
    outputs.append(request->arg(i).c_str());
    outputs.append("\",");
  }
  outputs.append("}");

  request->send(200, "text/plain", outputs.c_str());
}

void pwmHandler(AsyncWebServerRequest *request)
{
  int args = request->args();
  std::string outputs("{");
  for (int i = 0; i < args; i++)
  {
    outputs.append("\"");
    outputs.append(request->argName(i).c_str());
    outputs.append("\":\"");
    outputs.append(request->arg(i).c_str());
    outputs.append("\",");
  }
  outputs.append("}");

  request->send(200, "text/plain", outputs.c_str());
}

void qualityHandler(AsyncWebServerRequest *request)
{
  int args = request->args();
  std::string outputs("{");
  for (int i = 0; i < args; i++)
  {
    outputs.append("\"");
    outputs.append(request->argName(i).c_str());
    outputs.append("\":\"");
    outputs.append(request->arg(i).c_str());
    outputs.append("\",");
  }
  outputs.append("}");

  request->send(200, "text/plain", outputs.c_str());
}

void lightHandler(AsyncWebServerRequest *request)
{
  light = !light;
  request->send(200, "text/plain", light ? "true" : "false");
}

void infoHandler(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", "Connection Success!!!");
}