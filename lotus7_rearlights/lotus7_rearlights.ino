#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define BIGRINGSIZE        24                                                                           // DEFINITION: The amount of LEDs on the ring
#define BIGRINGPIN          6                                                                           // DEFINITION: The pin to which the tx/rx pin for the Neo Pixel is attached

const int colour_white[3]       = {255, 255, 255};                                                      // CONSTANT: COLOUR - WHITE
const int colour_orange[3]      = {255, 50, 0};                                                         // CONSTANT: COLOUR - ORANGE
const int colour_red_bright[3]  = {255, 0, 0};                                                          // CONSTANT: COLOUR - WHITE
const int colour_red_dim[3]     = {50, 0, 0};                                                           // CONSTANT: COLOUR - ORANGE
const int colour_green[3]       = {0, 255, 0};                                                          // CONSTANT: COLOUR - GREEN
const int colour_blue[3]        = {0, 0, 255};                                                          // CONSTANT: COLOUR - BLUE
const int colour_startup[3]     = {255, 200, 0};                                                        // CONSTANT: COLOUR - COLOUR MIX FOR STARTUP

bool  buttonState = LOW;                                                                                // DEBUG: Initialise a button variable
Adafruit_NeoPixel bigring = Adafruit_NeoPixel(BIGRINGSIZE,   BIGRINGPIN,   NEO_GRB + NEO_KHZ800);       // VARIABLE: A neo pixel ring to be managed
float fade_speed = 0.6;                                                                                 // VARIABLE: A variable to define how fast the fade works
int   delay_value = 20;                                                                                 // VARIABLE: A delay value 
int   bigringled[24][3] = {0};                                                                          // VARIABLE: A variable to hold the colour state of each LED on a ring
int   rotate_right = 0;                                                                                 // VARIABLE: A variable to track a right rotation
int   rotate_left = 0;                                                                                  // VARIABLE: A variable to track a left rotation
int   rotate_speed = 1;                                                                                 // VARIABLE: A variable to configure the rotation speed.
int   rotate_count = 0;                                                                                 // VARIABLE: A variable used in conjunction with rotate_speed to allow for timing.
int   R = 0;                                                                                            // VARIABLE: Used for positional arguments to make the 2D array more readable (RED)
int   G = 1;                                                                                            // VARIABLE: Used for positional arguments to make the 2D array more readable (GREEN)
int   B = 2;                                                                                            // VARIABLE: Used for positional arguments to make the 2D array more readable (BLUE)

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> FUNCTIONS: GENERAL

void fade_all(){                                                                                        // FUNCTION START: A function that will fade all of the LED's colour settings by a fixed amount
    for(int count = 0; count < BIGRINGSIZE; count++ ){                                                  //  0. [1] Iterate through the LEDs on the ring
      bigringled[count][R] = (int)floor(bigringled[count][R]*fade_speed);                               //  1. Fade the current red colour setting on this LED by one step
      bigringled[count][G] = (int)floor(bigringled[count][G]*fade_speed);                               //  1. Fade the current green colour setting on this LED by one step
      bigringled[count][B] = (int)floor(bigringled[count][B]*fade_speed);                               //  1. Fade the current blue colour setting on this LED by one step
    }                                                                                                   //  0. [1] END
}                                                                                                       // FUNCTION END!

void update_leds(){                                                                                     // FUNCTION START: Send the colour settings to the LED
    for(int count = 0; count < BIGRINGSIZE; count++ ){                                                  //  0. [1] Iterate through the LEDs on the ring
      bigring.setPixelColor(count, bigringled[count][R],bigringled[count][G],bigringled[count][B]);     //  1. Send the colour information to the ring buffer
    }                                                                                                   //  0. [1] END
   bigring.show();                                                                                      //  0. Update the pixel state based on new settings
}                                                                                                       // FUNCTION END!

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> FUNCTIONS: ANIMATIONS

void indicate_right(){                                                                                  // FUNCTION START: A function to trigger an indicate right animation
  if (rotate_right > 23) rotate_right = 0;                                                              //  0. If the rotate counter is out of range, reset the counter
  memcpy(bigringled[rotate_right],colour_orange, sizeof(bigringled[1]));                                //  0. Set the colour of the LED (ORANGE)
  rotate_right++;                                                                                       //  0. Increment the colour rotator
}                                                                                                       // FUNCTION END!

void indicate_left(){                                                                                   // FUNCTION START: A function to trigger an indicate left animation
  memcpy(bigringled[rotate_left],colour_orange, sizeof(bigringled[1]));                                 //  0. Set the colour of the LED (ORANGE)
  if (rotate_left == 0) rotate_left = 24;                                                               //  0. If the rotate counter is out of range, reset the counter
  rotate_left--;                                                                                        //  0. Decrement the colour rotator
}                                                                                                       // FUNCTION END!

void indicate_hazards(){                                                                                // FUNCTION START: A function to trigger the indicate hazards animation
  indicate_right();                                                                                     // 0. Call indicate right sequence
  indicate_left();                                                                                      // 0. Call indicate left sequence
}                                                                                                       // FUNCTION END!

void indicate_reverse(){                                                                                // FUNCTION START: A function to trigger an indicate reverse
  for(int count = 0; count < BIGRINGSIZE; count++ ){                                                    //  0. [1] Iterate through the LEDs on the ring
    memcpy(bigringled[count],colour_white, sizeof(bigringled[1]));                                      //  1. Set the colour of the LED (WHITE)
  }                                                                                                     //  0. [1] END
}                                                                                                       // FUNCTION END!

void indicate_night(){                                                                                  // FUNCTION START: A function to trigger an indicate night time running
  for(int count = 0; count < BIGRINGSIZE; count++ ){                                                    //  0. [1] Iterate through the LEDs on the ring
    memcpy(bigringled[count],colour_red_dim, sizeof(bigringled[1]));                                    //  1. Set the colour of the LED (RED DIM)
  }                                                                                                     //  0. [1] END
}                                                                                                       // FUNCTION END!

void indicate_braking(){                                                                                // FUNCTION START: A function to trigger an indicate braking
  for(int count = 0; count < BIGRINGSIZE; count++ ){                                                    //  0. [1] Iterate through the LEDs on the ring
    memcpy(bigringled[count],colour_red_bright, sizeof(bigringled[1]));                                 //  1. Set the colour of the LED (RED BRIGHT)
  }                                                                                                     //  0. [1] END
}                                                                                                       // FUNCTION END!

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> FUNCTIONS: MAIN

void setup() {                                                                                          // FUNCTION: Initialise the program
  pinMode(4, INPUT);                                                                                    // DEBUG: Set up pin4 as input
  bigring.begin();                                                                                      //  0. This initializes the NeoPixel library.
  for(int count = 0; count < BIGRINGSIZE; count++ ){                                                    //  0. [1] Iterate through the LEDs on the ring
    memcpy(bigringled[count],colour_startup, sizeof(bigringled[1]));                                    //  1. Set the colour of the LED (STARTUP COLOURS)
  } 
}                                                                                                       // FUNCTION END!

void loop() {                                                                                           // FUNCTION: The main loop of the program
  fade_all();                                                                                           //  0. Run a fade on all LEDs
  buttonState = digitalRead(4);                                                                         // DEBUG
  if (buttonState == HIGH) {                                                                            // DEBUG
    indicate_left();                                                                                    // DEBUG
  }                                                                                                     // DEBUG
  update_leds();                                                                                        //  0. Push the latest colour information to the LEDs
  delay(delay_value);                                                                                   //  0. Delay before next update
}                                                                                                       // FUNCTION END!

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> NOTES

// Set Specific Pin         - bigpixels.setPixelColor(0, bigpixels.Color(0,50,0));
// Clear all Pins           - bigpixels.clear();
// Set all Pins             - bigpixels.fill(12582656, 0 ,24);
// Set all Pin Brightness   - bigpixels.setBrightness(10);
// Code Notes               - https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
// COLOURS                  - https://www.shodor.org/stella2java/rgbint.html
