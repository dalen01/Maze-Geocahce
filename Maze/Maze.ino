#include "C:\Users\danda\Documents\Geocaching\Utlegg\Maze Geocahce\Functions.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
int randomNumber;
const int numberOfButtons = 4;
const int buttonPins[numberOfButtons] = {2, 3, 4, 5};
bool lastButtonStates[numberOfButtons] = {0, 0, 0, 0};
bool buttonStates[numberOfButtons] = {0, 0, 0, 0};
unsigned long long lastDebounceTime[numberOfButtons] = {0, 0, 0, 0};
bool buttonPressed[numberOfButtons] = {0, 0, 0, 0};
int currentPos = 0;
#define MATRIXPIN 6 
#define NUMPIXELS 64

Adafruit_NeoPixel matrix(NUMPIXELS, MATRIXPIN, NEO_GRB + NEO_KHZ800);

void setup(){
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
clock_prescale_set(clock_div_1);
#endif

matrix.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
matrix.setBrightness(255);
matrix.show(); // Initialize all pixels to 'off'

for (int i=0;i<numberOfButtons;i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
}
randomSeed(analogRead(0));
randomNumber = random(0, NUMPIXELS);

Serial.begin(9600);
Serial.println("Starting");
matrix.setPixelColor(currentPos, 255, 0, 0);
matrix.setPixelColor(randomNumber, 0, 255, 0);
matrix.show();

}

void loop(){
for (int i=0; i<numberOfButtons;i++){
    buttonPressed[i] = checkButtonPressed(buttonPins[i], buttonStates[i], lastButtonStates[i], lastDebounceTime[i]);
    if (buttonPressed[i]){
        if (i == 0){
            moveLight(matrix, currentPos, 1);
        }
        else if (i == 1){
            moveLight(matrix, currentPos, 8);
        }
        else if (i == 2){
            moveLight(matrix, currentPos, -1);
        }
        else if (i == 3){
            moveLight(matrix, currentPos, -8);
        }
        if (currentPos == randomNumber){
            Serial.println("You win!");
            matrix.setPixelColor(randomNumber, 255, 0, 0);
            matrix.show();
            delay(1000);
            randomNumber = random(0, NUMPIXELS);
            matrix.setPixelColor(randomNumber, 0, 255, 0);
            matrix.show();
        }
    }
}

}