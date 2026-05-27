// 触摸调速呼吸灯：每次触摸切换速度档位（慢->中->快）
const int touchPin = T0;
const int ledPin = 2;
int speedLevel = 1;          // 1=慢，2=中，3=快
bool lastTouch = false;
unsigned long lastDebounce = 0;
const int debounceDelay = 50;

int brightness = 0;
int fadeAmount = 5;
int stepDelay = 30;          // 初始慢速
unsigned long lastUpdate = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(ledPin, 0);
  setSpeed(speedLevel);
}

void loop() {
  // 触摸检测
  int touchVal = touchRead(touchPin);
  bool touched = (touchVal < 30);
  if (touched != lastTouch && millis() - lastDebounce > debounceDelay) {
    lastDebounce = millis();
    if (touched) {
      speedLevel = (speedLevel % 3) + 1;
      setSpeed(speedLevel);
    }
  }
  lastTouch = touched;

  // 呼吸效果
  unsigned long now = millis();
  if (now - lastUpdate >= stepDelay) {
    lastUpdate = now;
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
      brightness = constrain(brightness, 0, 255);
    }
    ledcWrite(0, brightness);
  }
}

void setSpeed(int level) {
  switch(level) {
    case 1: stepDelay = 30; break;
    case 2: stepDelay = 15; break;
    case 3: stepDelay = 5;  break;
  }
}