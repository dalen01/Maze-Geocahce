#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
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
bool allowedMove(int currentPos, int direction){
    int matrixSize = 8;
    int newPos = currentPos + direction;
    if (direction == 1 && currentPos % matrixSize == 7){
        return false;
    }
    else if(direction == -1 && currentPos % matrixSize == 0){
        return false;
    }
    else if(direction == 8 && currentPos >= matrixSize*(matrixSize-1)){
        return false;
    }
    else if(direction == -8 && currentPos < matrixSize){
        return false;
    }
    return true;
}
void moveLight(Adafruit_NeoPixel &matrix,int &currentPos, int direction, int red = 255, int green = 0, int blue = 0, int step = 1){
    // up = 8, down = -8, left = 1, right = -1
    int matrixSize = matrix.numPixels(); //assuming the matrix is square
    matrixSize = sqrt(matrixSize);
    int newPos = currentPos + direction*step;
    if (direction == 1 && currentPos % matrixSize == 7){
        newPos = currentPos;
        Serial.println("At left edge");
    }
    else if(direction == -1 && currentPos % matrixSize == 0){
        newPos = currentPos;
        Serial.println("At right edge");
    }
    else if(direction == 8 && currentPos >= matrixSize*(matrixSize-1)){
        newPos = currentPos;
        Serial.println("At top edge");
    }
    else if(direction == -8 && currentPos < matrixSize){
        newPos = currentPos;
        Serial.println("At bottom edge");
    }
    matrix.setPixelColor(currentPos, 0, 0, 0);
    matrix.setPixelColor(newPos, red, green, blue);
    matrix.show();
    currentPos = newPos;
}
