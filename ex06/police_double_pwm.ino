// 警车双闪：两个LED交替渐变
const int ledPinA = 2;
const int ledPinB = 4;
int brightnessA = 0;
int brightnessB = 255;
int fadeAmount = 5;
unsigned long lastUpdate = 0;
const int updateInterval = 10;

void setup() {
  ledcSetup(0, 5000, 8);
  ledcAttachPin(ledPinA, 0);
  ledcSetup(1, 5000, 8);
  ledcAttachPin(ledPinB, 1);
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    brightnessA += fadeAmount;
    brightnessB -= fadeAmount;
    if (brightnessA >= 255 || brightnessA <= 0) {
      fadeAmount = -fadeAmount;
      brightnessA = constrain(brightnessA, 0, 255);
      brightnessB = constrain(brightnessB, 0, 255);
    }
    ledcWrite(0, brightnessA);
    ledcWrite(1, brightnessB);
  }
}