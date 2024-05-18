#include <Arduino.h>
#include <WebServer.h>
#include <Preferences.h>
#include <OTA.h>
#include <ESPTelnetStream.h>
#include <DebugUtils.h>

#include "credentials.h"
#include <esp_now.h>

#define PREF_KEY "config"
#define PREF_SSID_KEY "ssid"
#define PREF_PASSPHRASE_KEY "passphrase"

String ap_default_psk(AP_PSK); // Set your default PSK in credentials.h if you want
ESPTelnetStream telnet;

WebServer server(80);
Preferences prefs;

TaskHandle_t OTATask, TelnetTask, UiTask, SleepTask;
OTA ota;

// sleep modes, see
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/sleep_modes.html

void initDevBoard(uint8_t redValue = 0, uint8_t greenValue = 0, uint8_t blueValue = 0)
{
  // init RGB-LED
  neopixelWrite(PIN_NEOPIXEL, redValue, greenValue, blueValue);

  // you can initialize the dev board here ...
}

void handleRoot()
{
  if (server.hasArg(PREF_SSID_KEY) && server.arg(PREF_SSID_KEY).length() && server.hasArg(PREF_PASSPHRASE_KEY) && server.arg(PREF_PASSPHRASE_KEY).length())
  {
    const String ssid(server.arg(PREF_SSID_KEY));
    prefs.begin(PREF_KEY, false);
    prefs.putString(PREF_SSID_KEY, ssid);
    prefs.putString(PREF_PASSPHRASE_KEY, server.arg(PREF_PASSPHRASE_KEY));
    prefs.end();

    serialPrint("Credentials saved for: ");
    serialPrintln(ssid);
    telnet.print("Credentials saved for: ");
    telnet.println(ssid);

    char success[150];
    snprintf(
        success, 148,
        "<!DOCTYPE html>\
<head>\
  <title>ESP32-C6 OTA with webserver</title>\
  <meta http-equiv=\"Refresh\" content=\"0; URL=/saved\" />\
</head>\
<body>\
</body>\
</html>");
    server.send(200, F("text/html"), success);

    return;
  }

  prefs.begin(PREF_KEY, true);
  const String ssid(prefs.isKey(PREF_SSID_KEY) ? prefs.getString(PREF_SSID_KEY) : "");
  prefs.end();

  char temp[740];
  snprintf(
      temp, 740,
      "<!DOCTYPE html>\
  <head>\
    <title>ESP32-C6 OTA with webserver</title>\
    <style>\
      body { background-color: #cccccc;font-family: Arial, Helvetica, Sans-Serif;Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>OTA and webserver example</h1>\
    <form method=\"post\">\
      <div>\
        <label for=\"ssid\">SSID</label><br>\
        <input type=\"text\" id=\"ssid\" name=\"ssid\" value=\"%s\" required>\
      </div>\
      <div>\
        <label for=\"passphrase\">Passphrase</label><br>\
        <input type=\"password\" id=\"passphrase\" name=\"passphrase\" minlength=\"8\" required>\
      </div>\
      <div>\
        <input type=\"submit\" value=\"Save\">\
        <input type=\"reset\" value=\"Clear\">\
      </div>\
      </form>\
  </body>\
</html>",
      ssid);
  server.send(200, F("text/html"), temp);
}

void handleSaved()
{
  char success[330];

  snprintf(
      success, 324,
      "<html>\
<head>\
  <title>ESP32-C6 OTA with webserver</title>\
  <style>\
    body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
  </style>\
</head>\
<body>\
  <h1>OTA and webserver example</h1>\
  <div>Credentials saved. ESP is restarting and will be available under a new IP.</div>\
</body>\
</html>");
  server.send(200, F("text/html"), success);

  delay(2000);
  ESP.restart();
}

void handleNotFound()
{
  String message("File Not Found\n\n");
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

void otaTask(void *parameters)
{
  for (;;)
  {
    ota.handle();
    vTaskDelay(portTICK_PERIOD_MS);
  }
}

void telnetTask(void *parameters)
{
  for (;;)
  {
    telnet.loop();
    vTaskDelay(portTICK_PERIOD_MS);
  }
}

void uiTask(void *parameters)
{
  for (;;)
  {
    server.handleClient();
    vTaskDelay(portTICK_PERIOD_MS);
  }
}

void sleepTask(void *parameters)
{
  esp_err_t err = esp_sleep_enable_timer_wakeup(10 * 1000000);
  if (err == ESP_ERR_INVALID_ARG)
  {
    telnet.print("sleep timer wakeup value is too high");
    telnet.println(err);
  }

  for (;;)
  {
    if (ota.isLoading())
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }
    // TODO test this:
    // esp_err_t err = esp_sleep_enable_wifi_wakeup();
    // telnet.println("enable wifi wakeup...");
    // telnet.println(err);

    // esp_now_deinit();x
    neopixelWrite(PIN_NEOPIXEL, 0, 0, 0);
    telnet.println("sleep ...");
    telnet.flush();
    serialFlush();
    err = esp_light_sleep_start(); // ESP_OK or ESP_ERR_INVALID_STATE
    if (err == ESP_OK)
    {
      neopixelWrite(PIN_NEOPIXEL, 0, 255, 0);

      // wake up or failed
      // err = esp_now_init();
      delay(100);
      telnet.println("wake up ...");
    }
    else
    {
      neopixelWrite(PIN_NEOPIXEL, 255, 0, 0);
      telnet.println(err);
    }

    // give other tasks time
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  serialBegin(115200);

  initDevBoard(0, 255, 0);

  prefs.begin(PREF_KEY, true);
  String ssid(prefs.getString(PREF_SSID_KEY).c_str());
  String passphrase(prefs.getString(PREF_PASSPHRASE_KEY).c_str());
  prefs.end();
  serialPrint("SSID: ");
  serialPrintln(ssid);

  ota.begin(ap_default_psk, ssid.c_str(), passphrase.c_str());
  telnet.begin();

  server.on("/", handleRoot);
  server.on("/saved", handleSaved);
  server.onNotFound(handleNotFound);
  server.begin();

  xTaskCreatePinnedToCore(otaTask, "ota", 4096, NULL, 3, &OTATask, 0);

  xTaskCreatePinnedToCore(telnetTask, "debug", 4096, NULL, 2, &TelnetTask, 0);

  xTaskCreatePinnedToCore(uiTask, "ui", 4096, NULL, 2, &UiTask, 0);

  xTaskCreatePinnedToCore(sleepTask, "sleep", 2048, NULL, 1, &SleepTask, 0);
}

void loop()
{
  // telnet.print("core: ");
  // telnet.println(xPortGetCoreID());
  // delay(1000);

  // run only tasks w/o the non priorized loop task
  vTaskDelete(NULL);
}
