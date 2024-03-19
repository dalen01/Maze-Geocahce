#include "Functions.h"

bool checkButtonPressed(const int buttonPin, int buttonState, int lastButtonState, unsigned long long lastDebounceTime){
    const int debounceDelay = 50; // Adjust debounce delay as needed
    buttonState = digitalRead(buttonPin); 
    if (buttonState != lastButtonState){
        if (millis() - lastDebounceTime > debounceDelay){
            lastDebounceTime = millis();
                if (buttonState == LOW){
                    return true;
                }
            }
        }
    return false;
}
    

