#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);
const int touchPin = T0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(100);

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>触摸传感器仪表盘</title>";
  html += "<script>setInterval(async()=>{let r=await fetch('/data');let j=await r.json();document.getElementById('val').innerText=j.value;},200);</script>";
  html += "</head><body><h2>实时触摸数值</h2><p>当前值: <span id='val'>--</span></p></body></html>";
  server.send(200, "text/html", html);
}

void handleData() {
  int touchValue = touchRead(touchPin);
  String json = "{\"value\":" + String(touchValue) + "}";
  server.send(200, "application/json", json);
}