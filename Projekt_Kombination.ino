#define USE_ARDUINO_INTERRUPTS true
#define sensor A0
#define Highpulse 540

#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 srituhobby = Adafruit_SSD1306(128, 64, &Wire);

const int OUTPUT_TYPE = SERIAL_PLOTTER;
const int PULSE_INPUT = A0;
const int PULSE_BLINK = LED_BUILTIN;
const int PULSE_FADE = 5;
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle
const int PIN_SPEAKER = 9;    // speaker on pin9 makes a beep with heartbeat

int sX = 0;
int sY = 60;
int x = 0;
int Svalue;
int value;
long Stime = 0;
long Ltime = 0;
int count = 0;
int Bpm = 0;

PulseSensorPlayground pulseSensor;

void setup() { {
 Serial.begin(115200);
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);
  
  if (!pulseSensor.begin()) {

    for(;;) {
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

  Serial.begin(115200);
  srituhobby.begin(SSD1306_SWITCHCAPVCC, 0x3C);// Address 0x3C for 128x32
  delay(1000);
  srituhobby.clearDisplay();
}

void loop() {
  Svalue = analogRead(sensor);
  Serial.println(Svalue);
  value = map(Svalue, 0, 1024, 0, 45);

  int y = 60 - value;

  if (x > 128) {
    x = 0;
    sX = 0;
    srituhobby.clearDisplay();
  }

  srituhobby.drawLine(sX, sY, x, y, WHITE);
  sX = x;
  sY = y;
  x ++;

  BPM();

  srituhobby.setCursor(0, 0);
  srituhobby.setTextSize(2);
  srituhobby.setTextColor(SSD1306_WHITE);
  srituhobby.print("BPM :");
  srituhobby.display();

}

void BPM() {

  if (Svalue > Highpulse) {
    Stime = millis() - Ltime;
    count++;

    if (Stime / 1000 >= 60) {
      Ltime = millis();
      Serial.println(count);
      srituhobby.setCursor(60, 0);
      srituhobby.setTextSize(2);
      srituhobby.setTextColor(SSD1306_WHITE);
      srituhobby.print(count);
      srituhobby.print("   ");
      srituhobby.display();
      count = 0;
    }
  }

  delay(20);
  pulseSensor.outputSample();

  if (pulseSensor.sawStartOfBeat()) {
    pulseSensor.outputBeat();
    tone(PIN_SPEAKER,932);    // tone(pin,frequency)
  }

  if(pulseSensor.isInsideBeat() == false){
    noTone(PIN_SPEAKER);
  }

}
