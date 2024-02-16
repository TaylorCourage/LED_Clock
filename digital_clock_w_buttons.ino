/*////////////////  Digital Clock (w/ Buttons)  //////////////////
//                                                              //
//   Author: Taylor Courage         Date: August 3, 2019        //
//   Version: 2.1.1                 License: GPL                //
//                                                              //
//   Code to control a custom-built digital clock. Allows for   //
//   custom colour and brightness per-digit, and includes       //
//   buttons to set the time.                                   //
//                                                              //
////////////////////////////////////////////////////////////////*/

/*///////////////////////   CHANGELOG   //////////////////////////
//                                                              //
// V2.1.1: Timing Fixes                                         //
//   - Adjusted timer to attempt more accurate timing           //
//                                                              //
// V2.1: Bug fixes                                              //
//   - Fixed a problem where the 2nd last digit has a bright    //
//     in the number 4                                          //
//   - Slowed down the clock (appeared to gain a lot of time,   //
//     very fast)                                               //
//                                                              //
// V2.0: Initial Release                                        //
//   - Fully functional clock!                                  //
//                                                              //
////////////////////////////////////////////////////////////////*/


// I'll be honest with you, this code is pretty sloppy. I threw together V1.0 in about
// 4 hours after building an LED clock with some individually-addressable strip lights 
// I had lying around. It works, it keeps time fairly well, even if it's a mess....

#include <FastLED.h>  // We use FastLED for easy programming and control

#define NUM_LEDS 46  // The number of LEDs used for the clock. This is for 3-5x3 numbers with leading '1', includes separator
#define DATA_PIN 2  // Data pin for strip lights, change as necessary
#define BUTTON_UP 12
#define BUTTON_DN 10  // Signal pins for our three buttons, up, down, and select (middle).
#define BUTTON_MD 11


// Here we'll define our colours. The physical numbers are ordered left-to-right with the separator colour last
#define NUM_ONE_COL CRGB::HotPink
#define NUM_TWO_COL CRGB::LightCoral
#define NUM_THREE_COL CRGB::MediumVioletRed
#define NUM_FOUR_COL CRGB(105,0,105)
#define DOT_COLOUR CRGB::DarkTurquoise

CRGB leds[NUM_LEDS];  //Declare our LED strip


// Here we've got looottttsssss of variables. They're generally pretty self-explanatory.
unsigned long ledTimer = 0;
int fadeSpeedRed = 10, r = 0, timeChange = 0, upLatch = 0, downLatch = 0, stateLatch = 0;
//// The next line controls the brightness of each number
int numOneBright = 205, numTwoBright = 205, numThreeBright = 205, numFourBright = 205, dotBright = 205;
int base = 0, count = 0, printNumOne = 0;
unsigned int seconds = 0, minutes = 00, hours = 11;
static uint32_t lastTime, now;

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);

  // Unless you wanna go crazy with resistors on your buttons, we'll use the internal pullup pin
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DN, INPUT_PULLUP);
  pinMode(BUTTON_MD, INPUT_PULLUP);
  now = millis(); // Starting our timer
}

void loop() {

  // Read our three buttons
  int buttonState = digitalRead(BUTTON_MD);
  int buttonUp = digitalRead(BUTTON_UP);
  int buttonDown = digitalRead(BUTTON_DN);
  

  // Check if the time changing button was pressed
  // We have three states for the timeChange variable, state 0 means the time isn't being changed
  // State 1 means we are changing the hour, and state 2 is for the minute
  
  if (buttonState == LOW && stateLatch != 1) {
    stateLatch = 1;  // I've used a latch to prevent the button from "taking off", makes adjustment easy
    if (timeChange == 0) {
      timeChange = 1;
    } else if (timeChange == 1) {
      timeChange = 2;
    } else if (timeChange == 2) {
      timeChange = 0;
    }
  }
  if (buttonState == HIGH) {  // This resets the button
    stateLatch = 0;
  }

// Check our UP button
  if (buttonUp == LOW && timeChange == 1 && upLatch != 1) {
    hours += 1;
    upLatch = 1;
    if (hours == 13) {
      hours = 1;
    }
  } else if (buttonUp == LOW && timeChange == 2 && upLatch != 1) {
    minutes += 1;
    upLatch = 1;
    if (minutes == 60) {
      minutes = 0;
    }
  } else if (buttonUp == HIGH){
    upLatch = 0;
  }

// Check our DOWN button
  if (buttonDown == LOW && timeChange == 1 && downLatch != 1) {
    hours -= 1;
    downLatch = 1;
    if (hours < 1) {
      hours = 12;
    }
  } else if (buttonDown == LOW && timeChange == 2 && downLatch != 1) {
    if (minutes == 0) {
      minutes = 60;
    }
    minutes -= 1;
    downLatch = 1;
  } else if (buttonDown == HIGH){
    downLatch = 0;
  }

  // Here we'll check the time (ironically) to see if a full second has passed since the last one
  // **NOTE** after extensive testing in V1.0 of this software on identical hardware, it tends to
  // run a tad fast, gaining about two minutes a week. I'm still playing around with this variable,
  // but setting it slightly over 1000ms should help prevent this.
  
  if ((now - lastTime) < 1011) {
    now = millis();
  } else if ((now - lastTime) >= 1011) {
    lastTime = now;
    seconds += 1;
  }

  // Convert out seconds into minutes, minutes into hours, and rolls over hours
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
  if (hours < 1) {
    hours = 12;
  }

  // Finally we'll print the time
  printTime();
  
}

void printTime(){
  int timeVal = (hours * 100) + minutes;  // Let's put our time in one nice friendly variable to work with
  int numOne, numTwo, numThree; // Then let's separate that into three separate easy to work with variables
  
  if (timeVal >= 1000) { // Check if we need the leading '1' digit
    timeVal = timeVal - 1000;
    printNumOne = 1;
  } else {
    printNumOne = 0;
  }
  // Here we'll separate our number of hours
  count = 0;
  while (timeVal >= 100) {
    timeVal = timeVal - 100;
    count ++;
  }
  numOne = count;

  // Next let's break our minutes apart
  count = 0;
  while (timeVal >= 10) {
    timeVal = timeVal - 10;
    count ++;
  }
  numTwo = count;

  count = 0;
  while (timeVal >= 1) {
    timeVal = timeVal - 1;
    count ++;
  }
  numThree = count;
  
  FastLED.clear();  // Clear the display

// Draw our separator dots
  leds[18] = DOT_COLOUR;
  leds[18].fadeLightBy(dotBright);
  leds[19] = DOT_COLOUR;
  leds[19].fadeLightBy(dotBright);

// Let's draw our hours! This also controls the numbers turning on/off when changing the time
  if (printNumOne != 1 && timeChange == 2) {
    numOneBright = 255;
    printOne();
    numTwoBright = 255;
    printNum(1, numOne);
  } else if (printNumOne == 1 && timeChange == 2) {
    numOneBright = 255;
    printOne();
    numTwoBright = 255;
    printNum(1, numOne);
  } else if (printNumOne != 1) {
    numOneBright = 255;
    printOne();
    numTwoBright = 205;
    printNum(1, numOne);
  } else {
    numOneBright = 225;
    printOne();
    numTwoBright = 205;
    printNum(1, numOne);
  }

// Now let's draw minutes!
  if (timeChange == 1) {
    numThreeBright = 255;
    printNum(2, numTwo);
    numFourBright = 255;
    printNum(3, numThree);
  } else {
    numThreeBright = 205;
    printNum(2, numTwo);
    numFourBright = 205;
    printNum(3, numThree);
  }
  
  FastLED.show(); // Show our pretty time
}

void printOne() {
  // This function is used to draw our leading '1' if it is needed (i.e. if it's 10, 11, or 12 o'clock)
  for (int i = 0; i < 5; i++){
    leds[i] = NUM_ONE_COL;
    leds[i].fadeLightBy(numOneBright);
  }
}


/////////////  !!WARNING!!  ///////////////

// Be VERY careful ahead, as this is the section that controls the 'drawing' of the numbers.
// Unless you are running your strips backwards, or have them wired a different way, or are using
// bigger/longer strips, and you know what you are getting into, DO NOT CHANGE ANYTHING! 

void printNum(int location, int num) {
  if (location == 1) {
    base = 5;
    if (num == 0) {  // NUMBER ZERO
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      leds[base + 5] = NUM_TWO_COL;
      leds[base + 7] = NUM_TWO_COL;
      leds[base + 5].fadeLightBy(numTwoBright);
      leds[base + 7].fadeLightBy(numTwoBright);
    } else if (num == 1) {  // NUMBER ONE
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 2) { // NUMBER TWO
      for (int i = base; i < base + 5; i++){
        if (i != base + 1) {
          leds[i] = NUM_TWO_COL;
          leds[i].fadeLightBy(numTwoBright);
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 11) {
          leds[i] = NUM_TWO_COL;
          leds[i].fadeLightBy(numTwoBright);
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 3) {  // NUMBER THREE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1 && i != base + 3) {
          leds[i] = NUM_TWO_COL;
          leds[i].fadeLightBy(numTwoBright);
        }
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 4) {  // NUMBER FOUR
      for (int i = base; i < base + 3; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      leds[base + 6] = NUM_TWO_COL;
      leds[base + 6].fadeLightBy(numTwoBright);
    } else if (num == 5) { // NUMBER FIVE
      for (int i = base; i < base + 5; i++){
        if (i != base + 3) {
          leds[i] = NUM_TWO_COL;
          leds[i].fadeLightBy(numTwoBright);
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 9) {
          leds[i] = NUM_TWO_COL;
          leds[i].fadeLightBy(numTwoBright);
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 6) {  // NUMBER SEVEN
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 5; i < base + 7; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 10; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 7) {  // NUMBER SEVEN
      leds[base] = NUM_TWO_COL;
      leds[base].fadeLightBy(numTwoBright);
      leds[base + 7] = NUM_TWO_COL;
      leds[base + 7].fadeLightBy(numTwoBright);
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 8) {  // NUMBER EIGHT
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else if (num == 9) {  // NUMBER NINE
      for (int i = base; i < base + 3; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 6; i < base + 8; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_TWO_COL;
        leds[i].fadeLightBy(numTwoBright);
      }
    } else {
      leds[base + 2] = NUM_TWO_COL;
      leds[base + 2].fadeLightBy(numTwoBright);
      leds[base + 6] = NUM_TWO_COL;
      leds[base + 6].fadeLightBy(numTwoBright);
      leds[base + 10] = NUM_TWO_COL;
      leds[base + 10].fadeLightBy(numTwoBright);
    }
  } else if (location == 2) {
    base = 20;
    if (num == 0) {  // NUMBER ZERO
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      leds[base + 5] = NUM_THREE_COL;
      leds[base + 5].fadeLightBy(numThreeBright);
      leds[base + 7] = NUM_THREE_COL;
      leds[base + 7].fadeLightBy(numThreeBright);
    } else if (num == 1) {  // NUMBER ONE
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 2) { // NUMBER TWO
      for (int i = base; i < base + 5; i++){
        if (i != base + 1) {
          leds[i] = NUM_THREE_COL;
          leds[i].fadeLightBy(numThreeBright);
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 11) {
          leds[i] = NUM_THREE_COL;
          leds[i].fadeLightBy(numThreeBright);
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 3) {  // NUMBER THREE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1 && i != base + 3) {
          leds[i] = NUM_THREE_COL;
          leds[i].fadeLightBy(numThreeBright);
        }
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 4) {  // NUMBER FOUR
      for (int i = base; i < base + 3; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      leds[base + 6] = NUM_THREE_COL;
      leds[base + 6].fadeLightBy(numThreeBright);
    } else if (num == 5) { // NUMBER FIVE
      for (int i = base; i < base + 5; i++){
        if (i != base + 3) {
          leds[i] = NUM_THREE_COL;
          leds[i].fadeLightBy(numThreeBright);
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 9) {
          leds[i] = NUM_THREE_COL;
          leds[i].fadeLightBy(numThreeBright);
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 6) {  // NUMBER SEVEN
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 5; i < base + 7; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 10; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 7) {  // NUMBER SEVEN
      leds[base] = NUM_THREE_COL;
      leds[base].fadeLightBy(numThreeBright);
      leds[base + 7] = NUM_THREE_COL;
      leds[base + 7].fadeLightBy(numThreeBright);
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 8) {  // NUMBER EIGHT
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else if (num == 9) {  // NUMBER NINE
      for (int i = base; i < base + 3; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 6; i < base + 8; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_THREE_COL;
        leds[i].fadeLightBy(numThreeBright);
      }
    } else {
      leds[base + 2] = NUM_THREE_COL;
      leds[base + 2].fadeLightBy(numThreeBright);
      leds[base + 6] = NUM_THREE_COL;
      leds[base + 6].fadeLightBy(numThreeBright);
      leds[base + 10] = NUM_THREE_COL;
      leds[base + 10].fadeLightBy(numThreeBright);
    }
  } else if (location == 3) {
    base = 33;
    if (num == 0) {  // NUMBER ZERO
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      leds[base + 5] = NUM_FOUR_COL;
      leds[base + 5].fadeLightBy(numFourBright);
      leds[base + 7] = NUM_FOUR_COL;
      leds[base + 7].fadeLightBy(numFourBright);
    } else if (num == 1) {  // NUMBER ONE
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 2) { // NUMBER TWO
      for (int i = base; i < base + 5; i++){
        if (i != base + 3) {
          leds[i] = NUM_FOUR_COL;
          leds[i].fadeLightBy(numFourBright);
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 9) {
          leds[i] = NUM_FOUR_COL;
          leds[i].fadeLightBy(numFourBright);
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 3) {  // NUMBER THREE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1 && i != base + 3) {
          leds[i] = NUM_FOUR_COL;
          leds[i].fadeLightBy(numFourBright);
        }
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 4) {  // NUMBER FOUR
      for (int i = base + 2; i < base + 5; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      leds[base + 6] = NUM_FOUR_COL;
      leds[base + 6].fadeLightBy(numFourBright);
    } else if (num == 5) { // NUMBER FIVE
      for (int i = base; i < base + 5; i++){
        if (i != base + 1) {
          leds[i] = NUM_FOUR_COL;
          leds[i].fadeLightBy(numFourBright);
        } 
      }
      for (int i = base + 8; i < base + 13; i++){
        if (i != base + 11) {
          leds[i] = NUM_FOUR_COL;
          leds[i].fadeLightBy(numFourBright);
        } 
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 6) {  // NUMBER SIX
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 6; i < base + 8; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 8; i < base + 11; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 7) {  // NUMBER SEVEN
      leds[base + 4] = NUM_FOUR_COL;
      leds[base + 4].fadeLightBy(numFourBright);
      leds[base + 5] = NUM_FOUR_COL;
      leds[base + 5].fadeLightBy(numFourBright);
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 8) {  // NUMBER EIGHT
      for (int i = base; i < base + 5; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 5; i < base + 8; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else if (num == 9) {  // NUMBER NINE
      for (int i = base + 2; i < base + 5; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 5; i < base + 7; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
      for (int i = base + 8; i < base + 13; i++){
        leds[i] = NUM_FOUR_COL;
        leds[i].fadeLightBy(numFourBright);
      }
    } else {
      leds[base + 2] = NUM_FOUR_COL;
      leds[base + 2].fadeLightBy(numFourBright);
      leds[base + 6] = NUM_FOUR_COL;
      leds[base + 6].fadeLightBy(numFourBright);
      leds[base + 10] = NUM_FOUR_COL;
      leds[base + 10].fadeLightBy(numFourBright);
    }
  }
}
