#include <Adafruit_NeoPixel.h>
#include "C:\Users\danda\Documents\Geocaching\Utlegg\Maze Geocahce\MazeWalls.h"

constexpr int MATRIX_PIN = 7;
constexpr int MATRIX_SIZE = 6;
constexpr int NUM_PIXELS = MATRIX_SIZE * MATRIX_SIZE;
constexpr int NUMBER_OF_BUTTONS = 4;
const int relayPin = 2;
const int buttonPins[NUMBER_OF_BUTTONS] = {5, 4, 6, 3}; //RIGHT DOWN LEFT UP
bool lastButtonStates[NUMBER_OF_BUTTONS] = {false, false, false, false};
bool buttonStates[NUMBER_OF_BUTTONS] = {false, false, false, false};
unsigned long lastDebounceTime[NUMBER_OF_BUTTONS] = {0, 0, 0, 0};
int finalPosition;
int currentMaze;
int currentPos;
int numberToGo = 4;
Adafruit_NeoPixel matrix(NUM_PIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

bool checkButtonPressed(int buttonPin, bool &buttonState, bool &lastButtonState, unsigned long &lastDebounceTime){
  constexpr unsigned long DEBOUNCE_DELAY = 50;
  bool reading = digitalRead(buttonPin) == LOW;
  if (reading != lastButtonState) lastDebounceTime = millis();
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY && reading != buttonState){
    buttonState = reading;
    if (buttonState) return true;
  }
  lastButtonState = reading;
  return false;
}

bool atEdge(int currentPosition, int direction){
  int row = currentPosition / MATRIX_SIZE;
  int column = currentPosition % MATRIX_SIZE;
  if (direction == 1) return column == MATRIX_SIZE - 1;
  if (direction == -1) return column == 0;
  if (direction == MATRIX_SIZE) return row == MATRIX_SIZE - 1;
  if (direction == -MATRIX_SIZE) return row == 0;
  return true;
}

bool hitWall(int currentPosition, int direction, int maze){
  int row = currentPosition / MATRIX_SIZE;
  int column = currentPosition % MATRIX_SIZE;
  if (direction == 1) return wallsVertical[maze][row][column];
  if (direction == -1) return wallsVertical[maze][row][column - 1];
  if (direction == MATRIX_SIZE) return wallsHorizontal[maze][row][column];
  if (direction == -MATRIX_SIZE) return wallsHorizontal[maze][row - 1][column];
  return true;
}

void drawGame(){
  matrix.clear();
  for (int i = 0; i < 2; i++) matrix.setPixelColor(circlePos[currentMaze][i], 0, 50, 0);
  matrix.setPixelColor(finalPosition, 50, 0, 0);
  matrix.setPixelColor(currentPos, 50, 50, 50);
  matrix.show();
}

void blinkRedCross(){
  for (int blinkCount = 0; blinkCount < 3; blinkCount++){
    matrix.clear();
    for (int row = 0; row < MATRIX_SIZE; row++){
      matrix.setPixelColor(row * MATRIX_SIZE + row, 255, 0, 0);
      matrix.setPixelColor(row * MATRIX_SIZE + MATRIX_SIZE - row - 1, 255, 0, 0);
    }
    matrix.show();
    delay(200);
    matrix.clear();
    matrix.show();
    delay(120);
  }
}

void showGreenCheck(int number){
  const int numberOfPixels[] = {12, 8, 11, 11, 12};
  const int zero[] = {2, 3, 7, 10, 13, 16, 19, 22, 25, 28, 32, 33};
  const int one[] = {3, 8, 9, 13, 15, 21, 27, 33};
  const int two[] = {2, 3, 7, 10, 16, 21, 26, 31, 32, 33, 34};
  const int three[] = {2, 3, 7, 10, 15, 16, 22, 25, 28, 32, 33};
  const int four[] = {3, 8, 9, 13, 15, 18, 19, 20, 21, 22, 27, 33};
  matrix.clear();
  for (int i = 0; i < numberOfPixels[number]; i++) {
    int pixel;
    switch (number) {
      case 0:
        pixel = zero[i];
        break;
      case 1:
        pixel = one[i];
        break;
      case 2:
        pixel = two[i];
        break;
      case 3:
        pixel = three[i];
        break;
      case 4:
        pixel = four[i];
        break;
    }
    matrix.setPixelColor(pixel, 0, 255, 0);
  }
  matrix.show();
  delay(1500);
}
void showRedCheck(int number){
  const int numberOfPixels[] = {12, 8, 11, 11,12};
  const int zero[] = {2, 3, 7, 10, 13, 16, 19, 22, 25, 28, 32, 33};
  const int one[] = {3, 8, 9, 13, 15, 21, 27, 33};
  const int two[] = {2, 3, 7, 10, 16, 21, 26, 31, 32, 33, 34};
  const int three[] = {2, 3, 7, 10, 15, 16, 22, 25, 28, 32, 33};
  const int four[] = {3, 8, 9, 13, 15, 18, 19, 20, 21, 22, 27, 33};
  matrix.clear();
  for (int i = 0; i < numberOfPixels[number]; i++) {
    int pixel;
    switch (number) {
      case 0:
        pixel = zero[i];
        break;
      case 1:
        pixel = one[i];
        break;
      case 2:
        pixel = two[i];
        break;
      case 3:
        pixel = three[i];
        break;
      case 4:
        pixel = four[i];
        break;
    }
    matrix.setPixelColor(pixel, 255, 0, 0);
  }
  matrix.show();
  delay(1500);
}
void displayVictorySequence(){
  const int wheelPixels[12] = {2, 3, 10, 17, 23, 28, 33, 32, 25, 18, 12, 7};
  const int standPixels[4] = {26, 27, 31, 34};
  const int cabinColors[4][3] = {
    {255, 0, 0},
    {0, 255, 80},
    {0, 80, 255},
    {255, 100, 0}
  };
  for (int frame = 0; frame < 48; frame++){
    matrix.clear();
    // Wheel outline
    for (int i = 0; i < 12; i++) matrix.setPixelColor(wheelPixels[i], 0, 0, 25);
    // Wheel centre
    matrix.setPixelColor(14, 80, 80, 80);
    matrix.setPixelColor(15, 80, 80, 80);
    matrix.setPixelColor(20, 80, 80, 80);
    matrix.setPixelColor(21, 80, 80, 80);
    // Wheel stand
    for (int i = 0; i < 4; i++) matrix.setPixelColor(standPixels[i], 80, 40, 0);
    // Four rotating cabins
    for (int cabin = 0; cabin < 4; cabin++){
      int wheelIndex = (frame + cabin * 3) % 12;
      int pixel = wheelPixels[wheelIndex];
      matrix.setPixelColor(pixel, cabinColors[cabin][0], cabinColors[cabin][1], cabinColors[cabin][2]);
    }
    matrix.show();
    delay(80);
  }
  matrix.clear();
  matrix.show();
}

int getPosition(int notSameAs = -1){
  int position = random(NUM_PIXELS);
  while (position == notSameAs) position = random(NUM_PIXELS);
  return position;
}

int getMazePosition(int notSameAs = -1){
  int position = getPosition(notSameAs);
  while (position == circlePos[currentMaze][0] || position == circlePos[currentMaze][1]) position = getPosition(notSameAs);
  return position;
}
void moveLight(int direction){
  if (atEdge(currentPos, direction)){
    Serial.println("You hit the edge");
    blinkRedCross();
    numberToGo = 4;
    showRedCheck(numberToGo);
    currentMaze = random(MAZE_COUNT);
    currentPos = getMazePosition();
    finalPosition = getMazePosition(currentPos);
    drawGame();
    return;
  }
  else if (hitWall(currentPos, direction, currentMaze)){
    Serial.println("You hit a wall");
    blinkRedCross();
    numberToGo = 4;
    showRedCheck(numberToGo);
    currentMaze = random(MAZE_COUNT);
    currentPos = getMazePosition();
    finalPosition = getMazePosition(currentPos);
    drawGame();
    return;
  }
  matrix.setPixelColor(currentPos, 0, 0, 0);
  if (currentPos == circlePos[currentMaze][0] || currentPos == circlePos[currentMaze][1]) matrix.setPixelColor(currentPos, 0, 50, 0);
  currentPos += direction;
  matrix.setPixelColor(currentPos, 50, 50, 50);
}



void setup(){
  matrix.begin();
  matrix.setBrightness(255);
  matrix.clear();
  matrix.show();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++) pinMode(buttonPins[i], INPUT_PULLUP);
  randomSeed(analogRead(0));
  currentMaze = random(MAZE_COUNT);
  currentPos = getMazePosition();
  finalPosition = getMazePosition(currentPos);
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.println("Comiled: "  __DATE__ ", " __TIME__);
  Serial.println("Starting:");
  //displayVictorySequence();
  /*
  for (int i = 0; i < NUM_PIXELS; i++){
    if (i) matrix.setPixelColor(i-1, 0, 0, 0);
    matrix.setPixelColor(i, 50, 50, 50);
    matrix.show();
    delay(50);
  }
  matrix.clear();
  */
  Serial.print("Current Maze: ");
  Serial.println(currentMaze);
  Serial.print("Final Position: ");
  Serial.println(finalPosition);
  drawGame();
}

void loop(){
  for (int i = 0; i < NUMBER_OF_BUTTONS; i++){
    if (checkButtonPressed(buttonPins[i], buttonStates[i], lastButtonStates[i], lastDebounceTime[i])){
      if (i == 0){
        moveLight(1);
        Serial.println("Moving Right");
      }
      else if (i == 1){
        moveLight(MATRIX_SIZE);
        Serial.println("Moving Down");
      }
      else if (i == 2){
        moveLight(-1);
        Serial.println("Moving Left");
      }
      else{
        moveLight(-MATRIX_SIZE);
        Serial.println("Moving Up");
      }
      matrix.show();
    }
  }
  if (currentPos == finalPosition){
    Serial.println("You found the final position");
    numberToGo--;
    if (numberToGo <= 0){
      displayVictorySequence();
      digitalWrite(relayPin, HIGH);
      delay(500);
      digitalWrite(relayPin, LOW);
      numberToGo = 4;
      currentMaze = random(MAZE_COUNT);
      currentPos = getMazePosition();
      finalPosition = getMazePosition(currentPos);
      drawGame();
    }
    else{
      Serial.print("You have ");
      Serial.print(numberToGo);
      Serial.println(" more to go");
      showGreenCheck(numberToGo);
      currentMaze = random(MAZE_COUNT);
      currentPos = getMazePosition();
      finalPosition = getMazePosition(currentPos);
      drawGame();
    }
    
  }
}
