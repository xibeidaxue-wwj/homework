#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);
const int ledPin = 2;
const int touchPin = T0;   // GPIO4 触摸引脚

bool armed = false;
bool alarming = false;
unsigned long alarmStartTime = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(100);

  server.on("/", handleRoot);
  server.on("/arm", []() { armed = true; alarming = false; digitalWrite(ledPin, LOW); server.send(200, "text/plain", "Armed"); });
  server.on("/disarm", []() { armed = false; alarming = false; digitalWrite(ledPin, LOW); server.send(200, "text/plain", "Disarmed"); });
  server.begin();
}

void loop() {
  server.handleClient();
  
  // 触摸检测
  if (armed && !alarming) {
    int touchVal = touchRead(touchPin);
    if (touchVal < 30) {  // 被触摸
      alarming = true;
      alarmStartTime = millis();
    }
  }
  
  // 报警状态：LED 快速闪烁（每秒5次）
  if (alarming) {
    unsigned long now = millis();
    bool ledOn = (now / 100) % 2;  // 每100ms翻转一次 → 10Hz闪烁
    digitalWrite(ledPin, ledOn ? HIGH : LOW);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>安防报警器</title></head><body>";
  html += "<h2>ESP32 安防系统</h2>";
  html += "<button onclick='fetch(\"/arm\")'>🔒 布防</button><br><br>";
  html += "<button onclick='fetch(\"/disarm\")'>🔓 撤防</button>";
  html += "<p id='status'>状态: " + String(armed ? "布防中" : "撤防") + "</p>";
  html += "<script>setInterval(()=>fetch('/').then(r=>r.text()).then(html=>{let p=document.getElementById('status');p.innerText=html.match(/状态: (.+?)</)?.[1]||''}),1000);</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}