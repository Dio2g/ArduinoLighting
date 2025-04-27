#include <FastLED.h>
#define NUM_LEDS 184
#define DATA_PIN 3
#define BRIGHTNESS 50

CRGB leds[NUM_LEDS];

const int micOffset = 273;
const float micGain = 0.4;
const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
float average = 0;
float prevAverage = 0;
const float decayFactor = 0.15;
int newPositionRight;
int newPositionLeft;

CRGB color = CRGB::Blue;
CRGB color2 = CRGB::Goldenrod;

void setup() {
    delay(2000);
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    total -= readings[readIndex];
    readings[readIndex] = constrain(abs(analogRead(A0) - micOffset) * micGain, 0, 255);
    total += readings[readIndex];
    readIndex = (readIndex + 1) % numReadings;

    average = total / numReadings;
    average = average * decayFactor + prevAverage * (1-decayFactor);
    prevAverage = average;

    newPositionRight = (NUM_LEDS/2 + 1) + average;
    newPositionLeft = (NUM_LEDS/2 - 1) - average;

    for (int i = NUM_LEDS/2+1; i < NUM_LEDS; i++) {
        float gradientValue = map(i, NUM_LEDS/2, NUM_LEDS, 0, 255);
        leds[i] = i <= newPositionRight ? blend(color, color2, gradientValue) : CRGB::White;
    }

    for (int i = NUM_LEDS/2-1; i >= 0; i--) {
        float gradientValue = map(i, NUM_LEDS/2, 0, 0, 255);
        leds[i] = i >= newPositionLeft ? blend(color, color2, gradientValue) : CRGB::White;
    }

    FastLED.show();
}
