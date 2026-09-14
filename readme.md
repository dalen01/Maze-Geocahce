# Maze Geocache

An electronic maze puzzle inspired by the maze module in *Keep Talking and Nobody Explodes*. A player uses an arcade joystick to move a white light through an invisible maze on a 6×6 RGB LED matrix. Reaching four randomly selected goals in succession triggers a solenoid and opens the geocache.

This repository contains the Arduino firmware, KiCad PCB design and production files, enclosure models, and laser-cutting drawings required to build the project.

## How it works

- The two green LEDs identify which of the nine maze layouts is active.
- The white LED is the player's current position.
- The red LED is the goal.
- The joystick moves the player up, down, left, or right.
- Hitting a wall or the edge of the matrix displays a red cross and resets progress.
- Reaching a goal displays the number of rounds remaining and loads a new random maze.
- Completing four mazes plays a victory animation and energizes the solenoid for 500 ms.

The walls are intentionally not shown on the LED matrix. Players must use an external maze reference or solve the layout by deduction.

## Hardware

The supplied PCB is designed around the following main components:

- Arduino Nano or a pin-compatible board
- 36 × WS2812B addressable RGB LEDs arranged as a 6×6 matrix
- Arcade joystick with four directional switches
- Solenoid lock
- TIP120 Darlington transistor and SS34 flyback diode for solenoid control
- 3 × AA battery holder
- Custom PCB and enclosure

See [`PCB Design/Maze Geocache/production/bom.csv`](PCB%20Design/Maze%20Geocache/production/bom.csv) for the PCB bill of materials and component footprints.

## Pin assignment

| Function | Arduino Nano pin |
| --- | ---: |
| Solenoid control | D2 |
| Joystick up | D3 |
| Joystick down | D4 |
| Joystick right | D5 |
| Joystick left | D6 |
| WS2812B data | D7 |

The directional inputs use the Arduino's internal pull-up resistors and are active low.

## Firmware setup

### Requirements

- Arduino IDE or Arduino CLI
- Arduino AVR Boards package
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library

### Prepare the sketch

The current sketch contains an absolute Windows path for `MazeWalls.h`. Before compiling on another computer:

1. Copy `Code/MazeWalls.h` into `Code/Maze/`.
2. Change the second line of `Code/Maze/Maze.ino` to:

```cpp
#include "MazeWalls.h"
```

### Upload with Arduino IDE

1. Open `Code/Maze/Maze.ino`.
2. Install **Adafruit NeoPixel** from the Library Manager.
3. Select **Arduino Nano** and the correct processor variant and serial port.
4. Compile and upload the sketch.
5. Optionally open the Serial Monitor at **9600 baud** to view movement and game-state messages.

## Assembly notes

1. Manufacture and populate the PCB from the files in `PCB Design/Maze Geocache/`.
2. Connect the four joystick switches to the matching UP, DOWN, LEFT, and RIGHT JST connectors.
3. Connect the solenoid to the two-pin screw terminal.
4. Install the Arduino Nano with the correct orientation.
5. Fit the PCB, joystick, lock, and battery holder into the enclosure.
6. Test all controls and the lock release before closing the enclosure.

The solenoid is an inductive, relatively high-current load. Do not drive it directly from an Arduino pin. Confirm the solenoid voltage, polarity, current requirement, transistor pinout, flyback-diode orientation, and power-source capability before applying power.

## Repository layout

| Path | Contents |
| --- | --- |
| `Code/` | Arduino sketch and maze wall definitions |
| `PCB Design/` | KiCad schematics, PCB layout, 3D board model, BOM, placement data, and fabrication archive |
| `3D Files/` | Fusion 360 archive and STEP model of the complete enclosure |
| `DXF Files/` | Laser-cutting drawings for the box and acrylic window |

## Customization

- Change `numberToGo` in `Maze.ino` to alter the number of successful mazes required to unlock the cache.
- Adjust the RGB values in `drawGame()` to change marker colors or brightness.
- Edit `circlePos`, `wallsVertical`, and `wallsHorizontal` in `MazeWalls.h` to change or add maze layouts. Update `MAZE_COUNT` when adding or removing layouts.
- Change the 500 ms delay in the victory sequence if the lock needs a different activation time, while staying within the solenoid's safe duty cycle.

## Fabrication warning

Review the PCB, mechanical files, clearances, power design, and production outputs before ordering parts or manufacturing. The repository does not include a verified electrical rating for the solenoid or batteries, and the design files should not be treated as production-ready without an independent check.

## Acknowledgements

The puzzle concept is inspired by the maze module from *Keep Talking and Nobody Explodes*. This is an independent fan project and is not affiliated with or endorsed by Steel Crate Games.
