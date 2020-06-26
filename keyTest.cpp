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

#define MATRIX_MAX 4
#define X_PINS 6, 13, 19, 26
#define Y_PINS 12, 16, 20, 21
#define MAX_BUTTON_DELAY 250000000 	// should trigger ~4 times a second at this delay. In ns
#define MIN_BUTTON_DELAY 25000000	// should trigger ~40 times a second at this delay. In ns
#define PIN_DELAY 0

int main(int argc, char** argv) {
   unsigned short int n;
   unsigned short int loopMax;
   unsigned char i;
   const int xPin[] = {X_PINS};
   const int yPin[] = {Y_PINS};
   bool pressed;
   unsigned char yCount;
   short int buttons;
   struct timespec waitTime;
   struct timespec pinTime;
   char* message;

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
   buttons = 0;
   wiringPiSetupGpio();
   for (i = 0; i < MATRIX_MAX; i++) {
      pinMode(xPin[i], OUTPUT);
      pinMode(yPin[i], INPUT);
      digitalWrite(xPin[i], LOW);
      pullUpDnControl(yPin[i], PUD_UP);
      // Wait for 150 cycles here?
   }

   while (1) {
      pressed = 1;
      for (i = 0; i <  MATRIX_MAX; i++) {	// check all yPins, to see if any where pulled low
         pressed &= digitalRead(yPin[i]);
      }
      if (pressed == 0) {
         for (i = 0; i < MATRIX_MAX; i++) {	// set all xpins to high, AKA won't pull ypins low.
            digitalWrite(xPin[i], HIGH);
         }
         buttons = 0;
         for (i = 0; i < MATRIX_MAX; i++) {
            digitalWrite(xPin[i], LOW);		// set the xpin we are checking to low
            // Waiting for the bare minimum amount of time seems to be enough
            nanosleep(&pinTime, NULL);	// delay, so can read right values from pins
            for (yCount = 0; yCount < MATRIX_MAX; yCount++) {
               if (digitalRead(yPin[yCount]) == 0) {
                  if (buttons) {	// only one key press should be detected at a time. If more than one detected, ignore all.
                     buttons = 0;
                     goto LOOPEND; // exit both loops if multiple keys were pressed
                  }
                  buttons |= 1 << (i*4 + yCount);
               }
            }
            digitalWrite(xPin[i], HIGH);	// reset the xpin we finished checking to high
         }
         
         LOOPEND:
         for (i = 0; i < MATRIX_MAX; i++) {	// reset all xPins to low
            digitalWrite(xPin[i], LOW);
         }
         if (buttons) {   // skip the rest of this loop if ignoring the button presses.
            message = new char[43];   // 22 for 1st message part, 19 for binary num(16 "0/1"s, and 3 "."s), 1 for \n, 1 for null terminator
            sprintf(message, "(%3d) Button Pressed: ", n + 1);
            for (i = 0; i < MATRIX_MAX*MATRIX_MAX; i++) {
               sprintf(message+22+i+(i/4), "%01d", (buttons >> i) & 1);
               if ((i+1)%4 == 0) sprintf(message+22+i+(i/4)+1, ".");
            }
            sprintf(message+42, "\n");
            printf("%s", message);
            n++;
            if (n >= loopMax) break;
         }
      }
      nanosleep(&waitTime, NULL);
   }
   
   return 0;
}
