/********************************************************
* Name         : Roge' Kuntz
* File Name    : keyTest.cpp
* Date created : 1/13/2020
* Date modified: 6/26/2020
* Purpose : Reads key presses from a 4x4 button matrix.
*           Does this using a basic (crappy) method.
********************************************************/

#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
/*#include <unistd.h>*/
#include <time.h>

#define TRUE 1
#define FALSE 0
#define MATRIX_MAX 4
#define X_PINS 6, 13, 19, 26
#define Y_PINS 12, 16, 20, 21
#define MAX_BUTTON_DELAY 250000000 	// should trigger ~4 times a second at this delay. In ns
#define MIN_BUTTON_DELAY  50000000	// should trigger ~20 times a second at this delay. In ns
#define BUTTON_DELAY_STEP 10000000

int main(int argc, char** argv) {
   unsigned short int n;
   unsigned short int loopMax;
   unsigned char i;
   const int xPin[] = {X_PINS};
   const int yPin[] = {Y_PINS};
   bool pressed;
   unsigned char yCount;
   unsigned char button;
   unsigned char lastButton;
   struct timespec waitTime;
   struct timespec pinTime;

   n = 0;
   waitTime.tv_sec = 0;
   waitTime.tv_nsec = MAX_BUTTON_DELAY;
   pinTime.tv_sec = 0;
   pinTime.tv_nsec = 0;
   if (argc < 2) {
      loopMax = 10;
   } else {
      loopMax = atoi(argv[1]);
      if (loopMax <= 1000) loopMax = 999;
   }
   button = 0;
   lastButton = 0;
   wiringPiSetupGpio();
   for (i = 0; i < MATRIX_MAX; i++) {
      pinMode(xPin[i], OUTPUT);
      pinMode(yPin[i], INPUT);
      digitalWrite(xPin[i], LOW);
      pullUpDnControl(yPin[i], PUD_UP);
   }
   // Wait for 150 cycles here?
   #pragma message("Warning: add code to force 150-cycle wait after changing pull-up state")

   while (1) {
      pressed = FALSE;
      for (i = 0; i <  MATRIX_MAX; i++) {	// check all yPins, to see if any where pulled low
         if (!digitalRead(yPin[i])) {
            pressed = TRUE;
            break;
         }
      }
      if (pressed) {
         for (i = 0; i < MATRIX_MAX; i++) {	// set all xpins to high, AKA won't pull ypins low.
            digitalWrite(xPin[i], HIGH);
         }
         pressed = FALSE;
         //buttons = 0;
         for (i = 0; i < MATRIX_MAX; i++) {
            digitalWrite(xPin[i], LOW);		// set the xpin we are checking to low
            // Waiting for the bare minimum amount of time seems to be enough
            nanosleep(&pinTime, NULL);	// delay, so can read right values from pins
            for (yCount = 0; yCount < MATRIX_MAX; yCount++) {
               if (digitalRead(yPin[yCount]) == 0) {
                  if (pressed) {	// only one key press should be detected at a time. If more than one detected, ignore all.
                     pressed = FALSE;
                     goto LOOPEND; // exit both loops if multiple keys were pressed
                  }
                  pressed = TRUE;
                  button = i*4 + yCount;
               }
            }
            digitalWrite(xPin[i], HIGH);	// reset the xpin we finished checking to high
         }
         
         LOOPEND:
         for (i = 0; i < MATRIX_MAX; i++) {	// reset all xPins to low
            digitalWrite(xPin[i], LOW);
         }
         if (pressed) {   // skip the rest of this loop if ignoring the button presses.
            printf("(%3d) Button Pressed: %2d\n", n + 1, button);
            n++;
            if (n >= loopMax) break;
            if (lastButton == button) {
               // count down, decrease delay
               if (waitTime.tv_nsec > MIN_BUTTON_DELAY) {
                  waitTime.tv_nsec -= BUTTON_DELAY_STEP;
               }
               goto DONTRESETDELAY;
            } /*else {
               // reset delay
               waitTime.tv_nsec = MAX_BUTTON_DELAY;
            }*/
            lastButton = button;
         } /*else {	// if two buttons being pressed, reset delay
            waitTime.tv_nsec = MAX_BUTTON_DELAY;
         }*/
      } /*else {	// if no buttons being pressed, reset delay
         waitTime.tv_nsec = MAX_BUTTON_DELAY;
      }*/
      waitTime.tv_nsec = MAX_BUTTON_DELAY;
      	// rest delay if:
	//	two buttons pressed,
	//	no buttons pressed,
	//	or last button pressed not match this button
      DONTRESETDELAY:
	// skip reseting delay if: last button pressed is same as this button press
      nanosleep(&waitTime, NULL);
   }
   
   return 0;
}
