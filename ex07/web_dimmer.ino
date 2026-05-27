#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);
const int ledPin = 2;
int pwmValue = 128;

void setup() {
  pinMode(ledPin, OUTPUT);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(ledPin, 0);
  ledcWrite(0, pwmValue);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>无极调光器</title></head><body>";
  html += "<h2>LED 亮度调节</h2>";
  html += "<input type='range' min='0' max='255' value='" + String(pwmValue) + "' onchange='fetch(`/set?value=`+this.value)'><br>";
  html += "<span id='val'>当前亮度: " + String(pwmValue) + "</span>";
  html += "<script>let slider=document.querySelector('input');slider.oninput=function(){document.getElementById('val').innerText='当前亮度: '+this.value;}</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("value")) {
    pwmValue = server.arg("value").toInt();
    ledcWrite(0, pwmValue);
  }
  server.send(200, "text/plain", "OK");
}