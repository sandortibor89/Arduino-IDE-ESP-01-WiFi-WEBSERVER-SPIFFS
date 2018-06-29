#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <FS.h>

DNSServer dnsServer;
ESP8266WebServer webServer(80);

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("OkosAP");

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "*", WiFi.softAPIP());

  if (!SPIFFS.begin()) {while(1);}

  //webServer.serveStatic("/", SPIFFS, "/index.html");
  
  //vagy
  
  webServer.onNotFound([]() {
    File file = SPIFFS.open("/index.html", "r");
    webServer.streamFile(file, "text/html");
    file.close();
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
