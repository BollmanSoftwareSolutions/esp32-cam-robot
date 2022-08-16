#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "SD.h"
#include "web_server.h"
#include "SD_MMC.h"

// Replace with your network credentials
const char *ssid = "";     //= "WIN_400433";
const char *password = ""; // = "4nbw4pmb8x";
bool validSD = false;
bool light = false;
int pwm = 192;
const char *quality = "VGA";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
WebServerWrapper wrapper(80);

char *readFile(const char *path);
void initInterfaceEndpoints(AsyncWebServer server);
bool initSDCard();

void setup()
{
  Serial.begin(115200);
  validSD = initSDCard();
  Serial.printf("valid SD card: %s\n", validSD ? "true" : "false");

  initSettings();

  initWiFi(ssid, password);
  wrapper.initGet("/info", infoHandler);
  wrapper.initGet("/direction", directionHandler);
  wrapper.initGet("/pwm", pwmHandler);
  wrapper.initGet("/quality", qualityHandler);
  wrapper.initGet("/light", lightHandler);
  wrapper.initInterfaceEndpoints();

  wrapper.begin();
}

void loop()
{
}

void initSettings()
{
  if (validSD && SD_MMC.exists("/settings.json"))
  {
    Serial.println("getting settings from SD card");

    char *settings = readFile("/settings.json");

    StaticJsonDocument<32> doc;

    DeserializationError error = deserializeJson(doc, settings);

    if (error)
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    ssid = doc["ssid"];         // "WIN_400433"
    password = doc["password"]; // "4nbw4pmb8x"
  }
  else
  {
    ssid = "WIN_400433";
    password = "4nbw4pmb8x";
  }
  Serial.printf("ssid: %s\n", ssid);
  Serial.printf("password: %s\n", password);
}

void initWiFi(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
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