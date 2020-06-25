/********************************************************
* Name         : Roge' Kuntz
* File Name    : keyTest.cpp
* Date created : 1/13/2020
* Date modified: 1/13/2020
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
#define BUTTON_DELAY 250000000 /* Value in nano-seconds */

int main(int argc, char** argv) {
   int n;
   int loopMax;
   int i;
   const int xPin[] = {X_PINS};
   const int yPin[] = {Y_PINS};
   int pressed;
   int xCount;
   int yCount;
   int button[MATRIX_MAX*MATRIX_MAX];
   struct timespec waitTime;
   
   n = 0;
   waitTime.tv_sec = 0;
   waitTime.tv_nsec = BUTTON_DELAY;
   if (argc < 2) {
      loopMax = 10;
   } else {
      loopMax = atoi(argv[1]);
   }
   for (i = 0; i < MATRIX_MAX*MATRIX_MAX; i++) {
      button[i] = 0;
   }
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
      for (i = 0; i <  MATRIX_MAX; i++) {
         pressed &= digitalRead(yPin[i]);
      }
      if (pressed == 0) {
         xCount = 0;
         for (i = 1; i < MATRIX_MAX; i++) {
            digitalWrite(xPin[i], HIGH);
         }
         for (i = 1; i < MATRIX_MAX + 1; i++) {
            for (yCount = 0; yCount < MATRIX_MAX; yCount++) {
               if (digitalRead(yPin[yCount]) == 0) button[xCount*4 + yCount] = 1;
            }
            if (i == MATRIX_MAX) break;
            xCount++;
            digitalWrite(xPin[i-1], HIGH);
            digitalWrite(xPin[i], LOW);
         }
         for (i = 0; i < MATRIX_MAX; i++) {
            digitalWrite(xPin[i], LOW);
         }
         printf("(%d) Buttons Pressed: ", n + 1);
         for (i = 0; i < MATRIX_MAX*MATRIX_MAX; i++) {
            if (button[i]) printf(" %d", i + 1);
            button[i] = 0;
         }
         printf("\n");
         n++;
         if (n >= loopMax) break;
      }
      //usleep((useconds_t) BUTTON_DELAY);
      nanosleep(&waitTime, NULL);
   }
   
   return 0;
}
