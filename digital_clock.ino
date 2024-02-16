#include <FastLED.h>

#define NUM_LEDS 46
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

unsigned long ledTimer = 0;
int fadeSpeedRed = 10, r = 0;
uint8_t gHue = 0;
int base = 0, count = 0, printNumOne = 0;
unsigned int seconds = 0, minutes = 03, hours = 6;
static uint32_t lastTime, now;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(100);
  now = millis();
}

void loop() {
  Serial.println("      ");
  printTime();
  seconds += 1;
  if (seconds == 60) {
    minutes += 1;
    seconds = 0;
  }
  if (minutes == 60) {
    hours += 1;
    minutes = 0;
  }
  if (hours == 13) {
    hours = 1;
    printNumOne == 0;
  }
  while ((now - lastTime) < 1000) {
    now = millis();
  }
  lastTime = now;
}

void printTime(){
  int timeVal = (hours * 100) + minutes;
  int numOne, numTwo, numThree;
  Serial.println(timeVal);
  if (timeVal >= 1000) {
    timeVal = timeVal - 1000;
    printNumOne = 1;
  } else {
    printNumOne = 0;
  }
  Serial.println(printNumOne);
  
  count = 0;
  while (timeVal >= 100) {
    timeVal = timeVal - 100;
    count ++;
  }
  numOne = count;
  Serial.println(numThree);
  
  count = 0;
  while (timeVal >= 10) {
    timeVal = timeVal - 10;
    count ++;
  }
  numTwo = count;
  Serial.println(numTwo);
  
  count = 0;
  while (timeVal >= 1) {
    timeVal = timeVal - 1;
    count ++;
  }
  numThree = count;
  Serial.println(numThree);
  
  FastLED.clear();
  if (printNumOne == 1) {
    printOne();
  } 
  leds[18] = CRGB::Pink;
  leds[19] = CRGB::Pink;
  FastLED.setBrightness(120);
  printNum(1, numOne);
  FastLED.setBrightness(70);
  printNum(2, numTwo);
  FastLED.setBrightness(100);
  printNum(3, numThree);
  FastLED.show();
}

void printOne() {
  for (int i = 0; i < 5; i++){
    leds[i] = CRGB::Purple;
  }
}

void printNum(int location, int num) {
  if (location == 1) {
    base = 5;
    if (num == 0) {  // NUMBER ZERO
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
      leds[base + 5] = CRGB::Green;
      leds[base + 7] = CRGB::Green;
    } else if (num == 1) {  // NUMBER ONE
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 2) { // NUMBER TWO
      for (int i = base; i < base + 5; i++){
        if (i != base + 1) {
          leds[i] = CRGB::Green;
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 11) {
          leds[i] = CRGB::Green;
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 3) {  // NUMBER THREE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1 && i != base + 3) {
          leds[i] = CRGB::Green;
        }
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 4) {  // NUMBER FOUR
      for (int i = base; i < base + 3; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
      leds[base + 6] = CRGB::Green;
    } else if (num == 5) { // NUMBER FIVE
      for (int i = base; i < base + 5; i++){
        if (i != base + 3) {
          leds[i] = CRGB::Green;
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 9) {
          leds[i] = CRGB::Green;
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 6) {  // NUMBER SEVEN
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 5; i < base + 7; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 10; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 7) {  // NUMBER SEVEN
      leds[base] = CRGB::Green;
      leds[base + 7] = CRGB::Green;
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 8) {  // NUMBER EIGHT
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Green;
      }
    } else if (num == 9) {  // NUMBER NINE
      for (int i = base; i < base + 3; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 6; i < base + 8; i++){
        leds[i] = CRGB::Green;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Green;
      }
    } else {
      leds[base + 2] = CRGB::Green;
      leds[base + 6] = CRGB::Green;
      leds[base + 10] = CRGB::Green;
    }
  } else if (location == 2) {
    base = 20;
    if (num == 0) {  // NUMBER ZERO
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
      leds[base + 5] = CRGB::Red;
      leds[base + 7] = CRGB::Red;
    } else if (num == 1) {  // NUMBER ONE
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 2) { // NUMBER TWO
      for (int i = base; i < base + 5; i++){
        if (i != base + 1) {
          leds[i] = CRGB::Red;
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 11) {
          leds[i] = CRGB::Red;
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 3) {  // NUMBER THREE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1 && i != base + 3) {
          leds[i] = CRGB::Red;
        }
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 4) {  // NUMBER FOUR
      for (int i = base; i < base + 3; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
      leds[base + 6] = CRGB::Red;
    } else if (num == 5) { // NUMBER FIVE
      for (int i = base; i < base + 5; i++){
        if (i != base + 3) {
          leds[i] = CRGB::Red;
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 9) {
          leds[i] = CRGB::Red;
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 6) {  // NUMBER SEVEN
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 5; i < base + 7; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 10; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 7) {  // NUMBER SEVEN
      leds[base] = CRGB::Red;
      leds[base + 7] = CRGB::Red;
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 8) {  // NUMBER EIGHT
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Red;
      }
    } else if (num == 9) {  // NUMBER NINE
      for (int i = base; i < base + 3; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 6; i < base + 8; i++){
        leds[i] = CRGB::Red;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Red;
      }
    } else {
      leds[base + 2] = CRGB::Red;
      leds[base + 6] = CRGB::Red;
      leds[base + 10] = CRGB::Red;
    }
  } else if (location == 3) {
    base = 33;
  Serial.print(location);
  Serial.print(", ");
  Serial.println(num);
    if (num == 0) {  // NUMBER ZERO
      Serial.println("ZERO");
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
      leds[base + 5] = CRGB::Blue;
      leds[base + 7] = CRGB::Blue;
    } else if (num == 1) {  // NUMBER ONE
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 2) { // NUMBER TWO
      for (int i = base; i < base + 5; i++){
        if (i != base + 3) {
          leds[i] = CRGB::Blue;
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 9) {
          leds[i] = CRGB::Blue;
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 3) {  // NUMBER THREE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1 && i != base + 3) {
          leds[i] = CRGB::Blue;
        }
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 4) {  // NUMBER FOUR
      for (int i = base + 2; i < base + 5; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
      leds[base + 6] = CRGB::Blue;
    } else if (num == 5) { // NUMBER FIVE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1) {
          leds[i] = CRGB::Blue;
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 11) {
          leds[i] = CRGB::Blue;
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 6) {  // NUMBER SIX
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 6; i < base + 8; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 8; i < base + 11; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 7) {  // NUMBER SEVEN
      leds[base + 4] = CRGB::Blue;
      leds[base + 5] = CRGB::Blue;
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 8) {  // NUMBER EIGHT
      for (int i = base; i < base + 5; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = CRGB::Blue;
      }
    } else if (num == 9) {  // NUMBER NINE
      for (int i = base + 2; i < base + 5; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 5; i < base + 7; i++){
        leds[i] = CRGB::Blue;
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = CRGB::Blue;
      }
    } else {
      leds[base + 2] = CRGB::Blue;
      leds[base + 6] = CRGB::Blue;
      leds[base + 10] = CRGB::Blue;
    }
  }
}
