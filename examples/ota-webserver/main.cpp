#include <Arduino.h>
#include <OTA.h>
#include <ESPTelnetStream.h>
#include <WebServer.h>

#include "credentials.h"

OTA ota;
String ap_default_psk(AP_PSK); // Set your default PSK in credentials.h if you want
ESPTelnetStream telnet;

WebServer server(80);

const char NEWLINE = '\n';
const char CARRIAGE_RETURN = '\r';

void initDevBoard(uint8_t redValue = 0, uint8_t greenValue = 0, uint8_t blueValue = 0)
{
  // init RGB-LED
  neopixelWrite(PIN_NEOPIXEL, redValue, greenValue, blueValue);
}

void handleRoot()
{
  if (server.hasArg("name"))
  {
    char success[350];
    const String message(F("<!DOCTYPE html>\
<head>\
  <title>ESP32-C6 OTA with webserver</title>\
  <style>\
    .highlight {\
      display: grid;\
      height: 100dvh;\
      width: 100dvw;\
      place-items: center;\
      font-size: 2rem;\
      font-family: Verdana, sans-serif;\
    }\
  </style>\
</head>\
<body>\
<div class=\"highlight\">Hello %s</div>\
</body>\
</html>"));
    snprintf(
        success, message.length(),
        message.c_str(),
        server.arg("name"));
    server.send(200, F("text/html"), success);

    return;
  }

  char temp[540];
  snprintf(
      temp, 536,
      "<!DOCTYPE html>\
  <head>\
    <title>ESP32-C6 OTA with webserver</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>OTA and webserver example</h1>\
    <form method=\"post\">\
      <div>\
        <label for=\"name\">Your name</label><br>\
        <input type=\"text\" id=\"name\" name=\"name\" required>\
      </div>\
      <div>\
        <input type=\"submit\" value=\"Save\">\
        <input type=\"reset\" value=\"Clear\">\
      </div>\
      </form>\
  </body>\
</html>");
  server.send(200, "text/html", temp);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup()
{
  initDevBoard(255, 0, 0);

  const char *MY_SSID = "";
  const char *PASSPHRASE = "";
  // TODO show UI and save in preferences, load on start and put it here
  //  on UI - posibility to reset credentials to show AP
  ota.begin(ap_default_psk, MY_SSID, PASSPHRASE);
  telnet.begin();

  server.on(F("/"), handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop()
{
  ota.handle();
  telnet.loop();
  server.handleClient();
  delay(2); // give cpu time to do things
}
