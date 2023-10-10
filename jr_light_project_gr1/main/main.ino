#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define RINGSIZE         7                                                                              // DEFINITION: The amount of LEDs on the ring
#define RINGPIN          6                                                                              // DEFINITION: The pin to which the tx/rx pin for the Neo Pixel is attached
#define BUTTON           4                                                                              // DEFINITION: The pin where the button input is configured

const int colour_white[3]       = {255, 255, 255};                                                      // CONSTANT: COLOUR - WHITE

const int colour_red[3]         = {255, 0, 0};                                                          // CONSTANT: COLOUR - RED
const int colour_orange[3]      = {255, 40, 0};                                                         // CONSTANT: COLOUR - ORANGE
const int colour_yellow[3]      = {255, 120, 0};                                                        // CONSTANT: COLOUR - YELLOW
const int colour_green[3]       = {0, 255, 0};                                                          // CONSTANT: COLOUR - GREEN
const int colour_blue[3]        = {0, 0, 255};                                                          // CONSTANT: COLOUR - BLUE
const int colour_indigo[3]      = {63, 0 ,225};                                                         // CONSTANT: COLOUR - INDIGO
const int colour_violet[3]      = {80,10,120};                                                         // CONSTANT: COLOUR - VIOLET

const int colour_red_dim[3]     = {25, 0, 0};                                                           // CONSTANT: COLOUR - RED [DIM]
const int colour_green_dim[3]   = {0, 25, 0};                                                           // CONSTANT: COLOUR - GREEN [DIM]
const int colour_blue_dim[3]    = {0, 0, 25};                                                           // COMSTANT: COLOUR - BLUE [DIM]
const int colour_turquoise[3]   = {0, 150, 200};                                                        // CONSTANT: COLOUR - TURQUOISE
const int colour_off[3]         = {0,0,0};                                                              // CONSTANT: COLOUR - OFF

bool  buttonState = LOW;                                                                                // VARIABLE: Initialise a button variable
bool  buttonLastState = 0;                                                                              // VARIABLE: Create a variable to manage debounce
Adafruit_NeoPixel ring = Adafruit_NeoPixel(RINGSIZE,   RINGPIN,   NEO_GRB + NEO_KHZ800);                // VARIABLE: A neo pixel ring to be managed
float fade_fast = 0.6;                                                                                  // VARIABLE: A variable to define how fast the fade works
float fade_slow = 0.99;                                                                                 // VARIABLE: A variable to define how fast the fade works
float fade_speed = fade_fast;                                                                           // VARIABLE: A variable to define how fast the fade works
int   delay_value = 20;                                                                                 // VARIABLE: A delay value 
int   ringled[24][3] = {0};                                                                             // VARIABLE: A variable to hold the colour state of each LED on a ring
int   rotate_right = 0;                                                                                 // VARIABLE: A variable to track a right rotation
int   rotate_left = 0;                                                                                  // VARIABLE: A variable to track a left rotation
int   rotate_speed = 1;                                                                                 // VARIABLE: A variable to configure the rotation speed.
int   rotate_count = 0;                                                                                 // VARIABLE: A variable used in conjunction with rotate_speed to allow for timing.
int   R = 0;                                                                                            // VARIABLE: Used for positional arguments to make the 2D array more readable (RED)
int   G = 1;                                                                                            // VARIABLE: Used for positional arguments to make the 2D array more readable (GREEN)
int   B = 2;                                                                                            // VARIABLE: Used for positional arguments to make the 2D array more readable (BLUE)
int   CURRENT = R;
int   status = 0;                                                                                       // VARIABLE: A variable to track the status 

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> FUNCTIONS: GENERAL

void fade_all(){                                                                                        // FUNCTION START: A function that will fade all of the LED's colour settings by a fixed amount
    for(int count = 0; count < RINGSIZE; count++ ){                                                     //  0. [1] Iterate through the LEDs on the ring
      ringled[count][R] = (int)floor(ringled[count][R]*fade_speed);                                     //  1. Fade the current red colour setting on this LED by one step
      ringled[count][G] = (int)floor(ringled[count][G]*fade_speed);                                     //  1. Fade the current green colour setting on this LED by one step
      ringled[count][B] = (int)floor(ringled[count][B]*fade_speed);                                     //  1. Fade the current blue colour setting on this LED by one step
    }                                                                                                   //  0. [1] END
}                                                                                                       // FUNCTION END!

void update_leds(){                                                                                     // FUNCTION START: Send the colour settings to the LED
    for(int count = 0; count < RINGSIZE; count++ ){                                                     //  0. [1] Iterate through the LEDs on the ring
      ring.setPixelColor(count, ringled[count][R],ringled[count][G],ringled[count][B]);                 //  1. Send the colour information to the ring buffer
    }                                                                                                   //  0. [1] END
   ring.show();                                                                                         //  0. Update the pixel state based on new settings
}                                                                                                       // FUNCTION END!

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> FUNCTIONS: ANIMATIONS

void rotate_led_right(const int thecolour[3]){                                                          // FUNCTION START: A function to trigger an indicate right animation
  if (fade_speed == fade_slow) fade_speed = fade_fast;
  if (rotate_right > 23) rotate_right = 0;                                                              //  0. If the rotate counter is out of range, reset the counter
  memcpy(ringled[rotate_right],thecolour, sizeof(ringled[1]));                                          //  0. Set the colour of the LED (ORANGE)
  rotate_right++;                                                                                       //  0. Increment the colour rotator
}                                                                                                       // FUNCTION END!

void rotate_led_left(const int thecolour[3]){                                                           // FUNCTION START: A function to trigger an indicate left animation
  if (fade_speed == fade_slow) fade_speed = fade_fast;
  memcpy(ringled[rotate_left],thecolour, sizeof(ringled[1]));                                           //  0. Set the colour of the LED (ORANGE)
  if (rotate_left == 0) rotate_left = 24;                                                               //  0. If the rotate counter is out of range, reset the counter
  rotate_left--;                                                                                        //  0. Decrement the colour rotator
}                                                                                                       // FUNCTION END!                                                                                                   // FUNCTION END!

void solid_light(const int thecolour[3]){
  for(int count = 0; count < RINGSIZE; count++ ){
  memcpy(ringled[count],thecolour, sizeof(ringled[1]));
  }
}

void pulse_light(const int thecolour[3]){
  if (fade_speed == fade_fast) fade_speed = fade_slow;
  if (ringled[0][R] >= ringled[0][G] && ringled[0][R] >= ringled[0][B]) CURRENT = R;
  else if (ringled[0][G] >= ringled[0][R] && ringled[0][G] >= ringled[0][B]) CURRENT = G;
  else CURRENT = B;
  if (ringled[0][CURRENT] < 1){
    solid_light(thecolour);
  }
}

void demo_rgb(){
  solid_light(colour_off);
  memcpy(ringled[0],colour_red_dim, sizeof(ringled[1]));
  memcpy(ringled[4],colour_green_dim, sizeof(ringled[1]));
  memcpy(ringled[1],colour_blue_dim, sizeof(ringled[1]));
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------> FUNCTIONS: MAIN                                                                                                      // FUNCTION END!

void status_update() {
  switch(status) {
   case 0  :
      demo_rgb();  
      break;
   case 1  :
      solid_light(colour_red);
      break;
   case 2  :
      solid_light(colour_orange);
      break;
   case 3  :
      solid_light(colour_yellow);
      break;
   case 4  :
      solid_light(colour_green);
      break;
   case 5  :
      solid_light(colour_blue);
      break;
   case 6  :
      solid_light(colour_indigo);
      break;
   case 7  :
      solid_light(colour_violet);
      break;
   case 8  :
      pulse_light(colour_white);
      break;
   default :
      solid_light(colour_red);
      status = 1;
    }
}

void setup() {                                                                                          // FUNCTION: Initialise the program
  pinMode(BUTTON, INPUT);                                                                               // DEBUG: Set up pin4 as input
  ring.begin();                                                                                         //  0. This initializes the NeoPixel library.

} 

void loop() {                                                                                           // FUNCTION: The main loop of the program
  fade_all();                                                                                           //  0. Run a fade on all LEDs
  buttonState = digitalRead(BUTTON);                                                                    //  0. Import current button status
  if (buttonState == HIGH) {                                                                            //  0. [1] Is button currently pressed?
      if (buttonLastState == LOW) {                                                                     //  1. YES, [2] Was it not pressed previously?
          buttonLastState = HIGH;                                                                       //  2. YES, Set flag to indicate it has been pressed
          memcpy(ringled[0],colour_off, sizeof(ringled[0]));
          status++;                                                                                     //  2. YES, Increment the status indicator.
      }                                                                                                 //  2. [END]
  }                                                                                                     //  1. [END]
  else {                                                                                                //  1. NO, The button is not pressed
    if (buttonLastState == HIGH) {                                                                      //  1. NO, [2] Is the button flagged as pressed?
      buttonLastState = LOW;                                                                            //  2. YES, Set the flag as unpressed
    }                                                                                                   //  2. [END]
  }                                                                                                     //  1. [END]
  status_update();                                                                                      //  0. Run the status update switch statement
  update_leds();                                                                                        //  0. Push the latest colour information to the LEDs
  delay(delay_value);                                                                                   //  0. Delay before next update
}                                                                                                       // FUNCTION END!