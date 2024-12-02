# "Rocket vs Asteroids" Game

## Introduction

This project represents a simple arcade-style game in which the player controls a rocket to avoid asteroids and shoot them with projectiles. The game is built using an LED matrix for display, a joystick for control, and a button for shooting and starting the game, while an LCD screen will display the menu.
The purpose of the project is to combine entertainment with practical learning in electronics and embedded programming. The inspiration for this project comes from classic arcade games that provide a simple yet captivating experience. This idea reminds me of the time when, as a child, I borrowed my mom's phone to relax and play 'Rocket vs Asteroids' for a few minutes.
Nowadays, there are many enthusiasts of retro games, as well as new consoles and arcade machines (Arcade Cabinets) that emulate the retro gaming experience. For me, building this project is a useful way to better understand robotics concepts.

---

## General Description

### Features:

- Game Flow:

The game begins when the player presses the fire button, initiating a countdown sequence ("3... 2... 1..."). Once the countdown ends, the rocketship, initially positioned at the bottom center of the LED matrix, becomes controllable. The player uses the joystick to navigate the rocketship left, and right to avoid incoming asteroids, which descend continuously from the top of the LED matrix.

In addition to dodging, the player can shoot projectiles to destroy asteroids. However, there’s a catch: the ammunition is limited. The remaining ammo count is displayed on the LCD screen, adding an element of resource management. Players can earn extra ammunition by reaching specific "milestone" scores, encouraging precise shooting and high performance.

As the game progresses, the difficulty increases: the number of asteroids falling from the top grows steadily, making navigation and shooting more challenging. The game ends if the rocketship collides with an asteroid.

After the game concludes, if the player’s score ranks among the top five highest scores (in the Time Rush mode), they are prompted to input their name. This nostalgic feature pays homage to classic arcade cabinets, where high scores and initials were immortalized. Both scores and names are saved in the Arduino Uno's internal memory, ensuring they persist even after the system is powered off.

- Menu:

When the build powers on, the LCD screen displays the game’s title, "Rocket vs Asteroids," along with "Shoot to start!" and "Press the joystick to open menu." 

Pressing the joystick opens a menu where options are displayed one at a time, and navigation is handled using the joystick. Players scroll through options by pushing the joystick up or down, enter a menu item by swiping right, go back by swiping left, and confirm selections by pressing the joystick. 

The menu includes a Leaderboard for the Time Rush mode (top three scores from the Time Rush mode). 

The Modes menu allows players to select between Classic (standard mode) and Time Rush (game with a fixed timer, competitive) with modes activated by pressing the fire button. 

The How to Play menu provides instructions on game mechanics, such as movement, shooting, ammo management, and mode descriptions, with navigation handled through up and down joystick movements. 

The Settings menu includes options to adjust brightness (from 1 to 5) and toggle or adjust volume levels, with changes confirmed by pressing the joystick. 

This intuitive system mimics retro arcade controls, enhancing the user experience with easy navigation, customizability, and a nostalgic feel.

For this project, I will leverage various features from previous lab exercises:

- Interrupts and timers from Lab 2 will be utilized for precise timing of events such as asteroid movement, projectile firing, and the countdown sequence.
- Button handling from Lab 0 will manage user inputs, including the fire button and joystick button presses.
- AnalogWrite and AnalogRead from Lab 3 will be employed for joystick control and possibly adjusting features like brightness or buzzer output.

### Provisional Block Diagram
![image](https://github.com/user-attachments/assets/ebd86ec7-a342-4a27-9ed0-1024c88e8182)

---

## Hardware Design

### Component List

- 1x Arduino Uno
- 2x LED Matrix
- 1x LCD display 16x2
- Buzzer
- 1x Joystick
- 1x Button for firing and starting the game
- Connecting wires
- Breadboard
- Necessary resistors

### Circuit Diagram

![image](https://github.com/user-attachments/assets/68f403c4-a576-442b-83e9-4fc5e2e4d5bc)
![image](https://github.com/user-attachments/assets/bf3b495b-2c6e-4ddf-9d71-831a7f22c294)

---

## Software Design

For this project, I am using PlatformIO as the development environment. 

The following libraries will be used throughout the development:
- <LiquidCrystal.h>
(As the project progresses, I will continue to update the libraries list.)

---

## Results

---

## Conclusions
