#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "html.h"
#include "SD_MMC.h"
#include <AsyncElegantOTA.h>

#define FIRMWARE_VERSION "v0.0.1"

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
    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
  }

  void end()
  {
    server.end();
  }

  void reset()
  {
    server.reset();
  }

  void initInterfaceEndpoints()
  {
    server.onNotFound(notFound);

    server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                request->send(200, "text/html", getRebootHTML()); 
                Serial.println("Rebooting ESP32: ");
                ESP.restart(); });

    if (SD_MMC.exists("/ui/index.html"))
    {
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SD_MMC, "/ui/index.html", "text/html"); });

      server.serveStatic("/ui/", SD_MMC, "/ui/");
    }
    else
    {
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(200, "text/html", getRobotIndexHTML()); });
    }

    if (SD_MMC.exists("/"))
    {
      server.onFileUpload(handleUpload);

      server.on("/file-upload", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send_P(200, "text/html", getUploadIndexHTML(), processor); });

      server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(200, "text/plain", listFiles(true)); });

      server.on("/file", HTTP_GET, handleFile);
    }
    else
    {
      server.on("/file-upload", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(200, "text/html", "No SD Card<br/>Cannot upload files"); });
    }
  }

  void initGet(const char *endpoint, ArRequestHandlerFunction func)
  {
    server.on(endpoint, HTTP_GET, func);
  }

private:
  static void notFound(AsyncWebServerRequest *request)
  {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    Serial.println(logmessage);
    request->send(404, "text/plain", "Not found");
  }

  // handles uploads to the filserver
  static void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
  {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    Serial.println(logmessage);

    if (!index)
    {
      logmessage = "Upload Start: " + String(filename);
      // open the file on first call and store the file handle in the request object
      request->_tempFile = SD_MMC.open("/" + filename, "w");
      Serial.println(logmessage);
    }

    if (len)
    {
      // stream the incoming chunk to the opened file
      request->_tempFile.write(data, len);
      logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
      Serial.println(logmessage);
    }

    if (final)
    {
      logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
      // close the file handle as the upload is now done
      request->_tempFile.close();
      Serial.println(logmessage);
      request->redirect("/");
    }
  }

  static void handleFile(AsyncWebServerRequest *request)
  {
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    Serial.println(logmessage);

    if (request->hasParam("name") && request->hasParam("action"))
    {
      const char *fileName = request->getParam("name")->value().c_str();
      const char *fileAction = request->getParam("action")->value().c_str();

      logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);

      if (!SD_MMC.exists(fileName))
      {
        Serial.println(logmessage + " ERROR: file does not exist");
        request->send(400, "text/plain", "ERROR: file does not exist");
      }
      else
      {
        Serial.println(logmessage + " file exists");
        if (strcmp(fileAction, "download") == 0)
        {
          logmessage += " downloaded";
          request->send(SD_MMC, fileName, "application/octet-stream");
        }
        else if (strcmp(fileAction, "delete") == 0)
        {
          logmessage += " deleted";
          SD_MMC.remove(fileName);
          request->send(200, "text/plain", "Deleted File: " + String(fileName));
        }
        else
        {
          logmessage += " ERROR: invalid action param supplied";
          request->send(400, "text/plain", "ERROR: invalid action param supplied");
        }
        Serial.println(logmessage);
      }
    }
    else
    {
      request->send(400, "text/plain", "ERROR: name and action params required");
    }
  }

  // list all of the files, if ishtml=true, return html rather than simple text
  static String listFiles(bool ishtml)
  {
    String returnText = "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
    Serial.println("Listing files stored on SD_MMC");
    File root = SD_MMC.open("/");
    returnText += listDirectoryFiles(root);
    returnText += "</table>";
    root.close();
    return returnText;
  }

  static String listDirectoryFiles(File directory)
  {
    String returnText = "<tr align='left'><td>" + String(directory.name()) + "</td><td>-----</td><td>--------</td></tr>";
    File file = directory.openNextFile();
    while (file)
    {
      if (file.isDirectory())
      {
        returnText += listDirectoryFiles(file);
      }

      returnText += "<tr align='left'><td>" + String(file.name()) + "</td><td>" + humanReadableSize(file.size()) + "</td>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(file.name()) + "\', \'download\')\">Download</button>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(file.name()) + "\', \'delete\')\">Delete</button></tr>";

      file = directory.openNextFile();
    }

    file.close();
    directory.close();
    return returnText;
  }

  // Make size of files human readable
  // source: https://github.com/CelliesProjects/minimalUploadAuthESP32
  static String humanReadableSize(const size_t bytes)
  {
    if (bytes < 1024)
      return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
      return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
      return String(bytes / 1024.0 / 1024.0) + " MB";
    else
      return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
  }

  // parses and processes webpages
  // if the webpage has %SOMETHING% or %SOMETHINGELSE% it will replace those strings with the ones defined
  static String processor(const String &var)
  {
    if (var == "FIRMWARE")
    {
      return FIRMWARE_VERSION;
    }

    if (var == "FREESPIFFS")
    {
      return humanReadableSize((SD_MMC.totalBytes() - SD_MMC.usedBytes()));
    }

    if (var == "USEDSPIFFS")
    {
      return humanReadableSize(SD_MMC.usedBytes());
    }

    if (var == "TOTALSPIFFS")
    {
      return humanReadableSize(SD_MMC.totalBytes());
    }

    return "";
  }
};