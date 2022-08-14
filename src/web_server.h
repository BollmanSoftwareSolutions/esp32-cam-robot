#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "html.h"
#include "SD_MMC.h"

class WebServerWrapper
{
public:
  AsyncWebServer server;

  ~WebServerWrapper()
  {
  }

  WebServerWrapper(uint16_t port)
      : server(port)
  {
  }

  void begin()
  {
    server.begin();
  }

  void end()
  {
    server.end();
  }

  void reset() // remove all writers and handlers, with onNotFound/onFileUpload/onRequestBody
  {
    server.reset();
  }

  void initInterfaceEndpoints()
  {
    if (SD_MMC.exists("/ui/index.html"))
    {
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SD_MMC, "/ui/index.html", "text/html"); });

      server.serveStatic("/ui/", SD_MMC, "/ui/");
    }
    else
    {
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(200, "text/html", "hello"); }); // getIndexHTML()); });
    }
  }

  void initGet(const char *endpoint, ArRequestHandlerFunction func)
  {
    server.on(endpoint, HTTP_GET, func);
  }
};

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