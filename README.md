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

After the game concludes, if the player’s score ranks among the top five highest scores, they are prompted to input their name. This nostalgic feature pays homage to classic arcade cabinets, where high scores and initials were immortalized. Both scores and names are saved in the Arduino Uno's internal memory, ensuring they persist even after the system is powered off.

- Menu:

When the build powers on, the LCD screen displays the game’s title, "Rocket vs Asteroids," along with "Shoot to start!" and "Press the joystick to open menu." Pressing the joystick opens a menu where options are displayed one at a time, and navigation is handled using the joystick. Players scroll through options by pushing the joystick up or down, enter a menu item by swiping right, go back by swiping left, and confirm selections by pressing the joystick. The menu includes a Leaderboard with two subcategories: Classic (top five scores from the Classic mode) and Time Rush (top five scores from the Time Rush mode). The Modes menu allows players to select between Classic (standard mode with limited ammo), Time Rush (game with a fixed timer), and Unlimited (casual play with unlimited ammo), with modes activated by pressing the fire button. The How to Play menu provides instructions on game mechanics, such as movement, shooting, ammo management, and mode descriptions, with navigation handled through up and down joystick movements. The Settings menu includes options to adjust brightness (from 1 to 5) and toggle or adjust volume levels, with changes confirmed by pressing the joystick. This intuitive system mimics retro arcade controls, enhancing the user experience with easy navigation, customizability, and a nostalgic feel.

### Provisional Block Diagram

- LED matrix for displaying the rocket, asteroids, and projectiles.
- LCD screen for displaying the menu and score-related information.
- Joystick for moving the rocket and navigating through the menu.
- Button for firing projectiles and starting the game.
- Arduino Uno as the main control unit.
---

## Hardware Design

### Component List

- 1x Arduino Uno
- 1x LED Matrix
- 1x LCD display 16x2
- Buzzer
- 1x Joystick
- 1x Button for firing and starting the game
- Connecting wires
- Breadboard
- Necessary resistors

### Circuit Diagram

---

## Software Design

---

## Results

---

## Conclusions
