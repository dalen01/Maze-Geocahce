#include "C:\Users\danda\Documents\Geocaching\Utlegg\Maze Geocahce\Functions.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
int finalPosition;
int currentMaze;
const int numberOfButtons = 4;
const int buttonPins[numberOfButtons] = {2, 3, 4, 5};
bool lastButtonStates[numberOfButtons] = {0, 0, 0, 0};
bool buttonStates[numberOfButtons] = {0, 0, 0, 0};
unsigned long long lastDebounceTime[numberOfButtons] = {0, 0, 0, 0};
bool buttonPressed[numberOfButtons] = {0, 0, 0, 0};
int currentPos;
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

currentMaze = random(0,9);
currentPos = getPosition();
while(currentPos == circlePos[currentMaze][0] || currentPos == circlePos[currentMaze][1]){
    currentPos = getPosition();
}
finalPosition = getPosition(currentPos);
Serial.begin(9600);
Serial.println("Starting:");
Serial.print("Current Maze: ");
Serial.println(currentMaze);
Serial.print("Final Position: ");
Serial.println(finalPosition);
matrix.setPixelColor(currentPos, 50, 50, 50);
matrix.setPixelColor(finalPosition, 50, 0, 0);
//matrix.setPixelColor(randomNumber, 0, 255, 0);
for (int i=0; i<2;i++){
    matrix.setPixelColor(circlePos[currentMaze][i], 0, 50, 0);
}
fillEdges(matrix);
matrix.show();
}

void loop(){

for (int i=0; i<numberOfButtons;i++){
    buttonPressed[i] = checkButtonPressed(buttonPins[i], buttonStates[i], lastButtonStates[i], lastDebounceTime[i]);
    if (buttonPressed[i]){
        if (i == 0){
            moveLight(matrix, currentPos, 1,currentMaze);
        }
        else if (i == 1){
            moveLight(matrix, currentPos, 8,currentMaze);
        }
        else if (i == 2){
            moveLight(matrix, currentPos, -1,currentMaze);
        }
        else if (i == 3){
            moveLight(matrix, currentPos, -8,currentMaze);
        }
        }
        matrix.show();
        
    }
    if (currentPos==finalPosition){
        Serial.println("You found the final position");
        for (int i=0; i<2;i++){
            matrix.setPixelColor(circlePos[currentMaze][i], 0, 0, 0);
        }
        currentMaze = random(0,9);
        for (int i=0; i<2;i++){
            matrix.setPixelColor(circlePos[currentMaze][i], 0, 50, 0);
        }
        matrix.setPixelColor(currentPos, 0, 0, 0);
        currentPos = circlePos[currentMaze][0];
        while(currentPos == circlePos[currentMaze][0] || currentPos == circlePos[currentMaze][1])
        {
            currentPos = getPosition();
        }
        finalPosition=getPosition(currentPos);
        matrix.setPixelColor(currentPos, 50, 50, 50);
        matrix.setPixelColor(finalPosition, 50, 0, 0);  
    }
}