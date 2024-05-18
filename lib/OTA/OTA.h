#ifndef OTA_H
#define OTA_H

class OTA
{
public:
  OTA();
  virtual ~OTA(){};
  void begin(const String &password, const char *station_ssid = "", const char *station_passphrase = "");
  void handle();
  bool isLoading();

private:
  void startAP(const String &passphrase);
  boolean startSTA(const char *station_ssid, const char *station_passphrase, const String &password);
  String getHostname();
};

#endif