// 触摸自锁开关：摸一下LED亮，再摸一下灭
const int touchPin = T0;   // GPIO4 触摸引脚
const int ledPin = 2;
bool lastTouch = false;
bool ledState = false;
unsigned long lastDebounce = 0;
const int debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  int touchVal = touchRead(touchPin);
  bool touched = (touchVal < 30);      // 阈值
  if (touched != lastTouch && millis() - lastDebounce > debounceDelay) {
    lastDebounce = millis();
    if (touched) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);
    }
  }
  lastTouch = touched;
}