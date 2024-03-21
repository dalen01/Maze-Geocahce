#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "MazeWalls.h"
int matrixSize = 8;
bool checkButtonPressed(const int buttonPin, bool &buttonState, bool &lastButtonState, unsigned long long &lastDebounceTime){
    const int debounceDelay = 50;

    bool reading = !digitalRead(buttonPin);
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay){
        if (reading != buttonState){
            buttonState = reading;
            if (buttonState == HIGH){
                return true;
            }
        }
    }
    lastButtonState = reading;
    return false;
}
bool atEdge(int currentPos, int direction){
    if (direction == 1 && currentPos % matrixSize == 6){
        return true;
    }
    else if(direction == -1 && currentPos % matrixSize == 1){
        return true;
    }
    else if(direction == 8 && currentPos >= matrixSize*(matrixSize-2)){
        return true;
    }
    else if(direction == -8 && currentPos < matrixSize*2){
        return true;
    }
    else{
        return false;
    }
}

bool hitWall(int currentPos, int direction,int currentMaze){
    if (direction == 1){
        return wallsVetical[currentMaze][matrixSize-currentPos/matrixSize-1][matrixSize-currentPos%matrixSize-2];
    }
    else if (direction == -1){
        return wallsVetical[currentMaze][matrixSize-currentPos/matrixSize-1][matrixSize-currentPos%matrixSize-1];
    }
    else if (direction == 8){
        return wallsHorizontal[currentMaze][matrixSize-currentPos/matrixSize-2][matrixSize-currentPos%matrixSize-1];
    }
    else if (direction == -8){
        return wallsHorizontal[currentMaze][matrixSize-currentPos/matrixSize-1][matrixSize-currentPos%matrixSize-1];
    }
}
void moveLight(Adafruit_NeoPixel &matrix,int &currentPos, int direction,int currentMaze = 0, int red = 50, int green = 50, int blue = 50, int step = 1){
    // up = 8, down = -8, left = 1, right = -1
    /*Serial.print("Current Position: ");
    Serial.print(currentPos);
    Serial.print("  Circle Position: 1: ");
    Serial.print(circlePos[currentMaze][0]);
    Serial.print("  2: ");
    Serial.print(circlePos[currentMaze][1]);
    Serial.print("  Direction: ");
    Serial.print(direction);
    Serial.print("  currentMaze: ");
    Serial.println(currentMaze);*/
    int newPos = currentPos + direction*step;
    if (atEdge(currentPos, direction)){
        Serial.println("You hit the edge");
        newPos = currentPos;
    }
    else if(hitWall(currentPos, direction, currentMaze)){
        Serial.println("You hit a wall");
        newPos = currentPos;
    }
    matrix.setPixelColor(currentPos, 0, 0, 0);
    if (currentPos == circlePos[currentMaze][0]){
        matrix.setPixelColor(currentPos, 0, 50, 0);
    }
    else if (currentPos == circlePos[currentMaze][1]){
        matrix.setPixelColor(currentPos, 0, 50, 0);
    }
    
    matrix.setPixelColor(newPos, red, green, blue);
    currentPos = newPos;
}
void fillEdges(Adafruit_NeoPixel &matrix, int red = 0, int green = 1, int blue = 1){
    for (int i=0; i<matrixSize; i++){
        matrix.setPixelColor(i, red, green, blue);
        matrix.setPixelColor(i+matrixSize*(matrixSize-1), red, green, blue);
        matrix.setPixelColor(i*matrixSize, red, green, blue);
        matrix.setPixelColor(i*matrixSize+matrixSize-1, red, green, blue);
    }
}

int getPosition(int notSameAs = 0){
    int position = random(0,matrixSize*matrixSize);
    while (position == notSameAs || position< matrixSize || position >= matrixSize*(matrixSize-1) || position % matrixSize == 0 || position % matrixSize == matrixSize-1){
        position = random(0,matrixSize*matrixSize);
    }
    return position;
}
