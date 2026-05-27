// 使用 millis() 实现 SOS 信号：三短闪、三长闪、三短闪，停顿2秒，循环
const int ledPin = 2;
// 序列：亮/灭时间（毫秒）  短:300亮300灭，长:600亮300灭，最后停顿2000
const int times[] = {300,300, 300,300, 300,300,   // 三短闪
                     600,300, 600,300, 600,300,   // 三长闪
                     300,300, 300,300, 300,300,   // 三短闪
                     2000};                       // 停顿
const int steps = sizeof(times)/sizeof(times[0]);
int step = 0;
unsigned long previousMillis = 0;
bool ledOn = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  previousMillis = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - previousMillis >= times[step]) {
    previousMillis = now;
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn ? HIGH : LOW);
    step++;
    if (step >= steps) step = 0;
  }
}