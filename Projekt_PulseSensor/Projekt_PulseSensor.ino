/*
 File: Projekt_PulseSensor
 Author: Dasten Mohamad Amin
 Description: Pulsesensor that shows BPM through OLED display, speaker and OLED lamp
 Date: 2023/12/13
 */

// List of define and include for the pulsesensor
#define USE_ARDUINO_INTERRUPTS true
#define sensor A0
#define Highpulse 540

#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 srituhobby = Adafruit_SSD1306(128, 64, &Wire);

// Define constants for pulsesensor
const int OUTPUT_TYPE = SERIAL_PLOTTER;
const int PULSE_INPUT = A0;
const int PULSE_BLINK = LED_BUILTIN;
const int PULSE_FADE = 5;    // OLED lamp blinks on pin5 with heartbeat
const int THRESHOLD = 550;   // Adjust this number to avoid noise when idle
const int PIN_SPEAKER = 9;    // speaker on pin9 makes a beep with heartbeat

// Variables needed for creating pulse wavelength on OLED display
int sX = 0;
int sY = 60;
int x = 0;

// Variables needed for pulse wavelength data
int Svalue;
int value;
long Stime = 0;
long Ltime = 0;
int count = 0;
int Bpm = 0;

// Library needed for pulsesensor function
PulseSensorPlayground pulseSensor;

void setup() { {
  // Starts Serial plotter or monitor with 115200 baud
 Serial.begin(115200);
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

// Checks if pulsesensor is working
  if (!pulseSensor.begin()) {

// Changes lighting of OLED lamp if it fails
    for(;;) {
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}
// Starts the OLED screen
  srituhobby.begin(SSD1306_SWITCHCAPVCC, 0x3C);// Address 0x3C for 128x32
  delay(1000);
// Clears the OLED screen
  srituhobby.clearDisplay();
}

void loop() {
  // Creates value for the sensor
  Svalue = analogRead(sensor);
  Serial.println(Svalue);
  // Maps the serial value
  value = map(Svalue, 0, 1024, 0, 45);

  int y = 60 - value;

 // Restarts OLED wavelength once it reaches the end of screen
  if (x > 128) {
    x = 0;
    sX = 0;
    srituhobby.clearDisplay();
  }

  srituhobby.drawLine(sX, sY, x, y, WHITE);
  sX = x;
  sY = y;
  x ++;

  // Displays BPM on OLED screen
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

    // Shows the BPM on OLED display after a minute has passed
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

  // Delays pulsesensor
  delay(20);
  pulseSensor.outputSample();

  // If heartbeat is detected produces tone through speaker
  if (pulseSensor.sawStartOfBeat()) {
    pulseSensor.outputBeat();
    tone(PIN_SPEAKER,932);    // tone(pin,frequency)
  }

  // If no heartbeat is detected stops tone
  if(pulseSensor.isInsideBeat() == false){
    noTone(PIN_SPEAKER);
  }

}
