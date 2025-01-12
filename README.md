# "Rocket vs Asteroids" Game

## Introduction

This project represents a simple arcade-style game in which the player controls a rocket to avoid asteroids and shoot them with projectiles. The game is built using an LED matrix for display, a joystick for control, and a button for shooting and starting the game, while an LCD screen will display the menu.
The purpose of the project is to combine entertainment with practical learning in electronics and embedded programming. The inspiration for this project comes from classic arcade games that provide a simple yet captivating experience. This idea reminds me of the time when, as a child, I borrowed my mom's phone to relax and play 'Rocket vs Asteroids' for a few minutes.
Nowadays, there are many enthusiasts of retro games, as well as new consoles and arcade machines (Arcade Cabinets) that emulate the retro gaming experience. For me, building this project is a useful way to better understand robotics concepts.

---

## General Description

### Features:

- Game Flow:

The game begins when the player presses the fire button, initiating a countdown sequence ("3... 2... 1...GO"). Once the countdown ends, the rocketship, initially positioned at the bottom center of the LED matrix, becomes controllable. The player uses the joystick to navigate the rocketship up, and down to avoid incoming asteroids.

In addition to dodging, the player can shoot projectiles to destroy asteroids. However, there’s a catch: the ammunition is limited. The remaining ammo count is displayed on the LCD screen, adding an element of resource management. Players can earn extra ammunition by reaching specific "milestones" during the round ;), encouraging precise shooting and high performance.

As the game progresses, the difficulty increases: the number of asteroids falling from the top grows steadily, making navigation and shooting more challenging. The game ends if the rocketship collides with an asteroid.

After the game concludes, if the player’s score ranks among the top five highest scores (in the Time Rush mode), they are prompted to input their name. This nostalgic feature pays homage to classic arcade cabinets, where high scores and initials were immortalized. Both scores and names are saved in the Arduino Uno's internal memory, ensuring they persist even after the system is powered off.

- Menu:

When the build powers on, the LCD screen displays "Shoot to Start" and "Right for Menu" guiding the player to make a choice. 

Pressing the joystick opens a menu where options are displayed two at a time, and navigation is handled using the joystick. Players scroll through options by pushing the joystick up or down, enter a menu item by swiping right. Any changes to the settings or game mode selection are automatically saved upon returning to the previous menu.

The menu includes:
  - the Game Modes menu that allows players to select between Classic (standard mode with unlimited ammo and no time restriction) and Time Rush (game with a fixed timer and ammo, competitive)
  - a Leaderboard for the Time Rush mode (top five scores from the Time Rush mode)
  - the Settings menu, which includes options to adjust brightness (from 0 to 100%) and adjust volume levels (from 0 to 100%)
  - an Easter egg counter.

This intuitive system mimics retro arcade controls, enhancing the user experience with easy navigation, customizability, and a nostalgic feel.

For this project, I will leverage the following features from previous lab exercises:

- Interrupts and timers from Lab 2 will be utilized for precise timing of events such as asteroid movement, projectile firing, and the countdown sequence.
- Button handling from Lab 0 will manage user inputs, including the fire button and joystick button presses.
- AnalogWrite and AnalogRead from Lab 3 will be employed for joystick control.

---

## Hardware Design

<details>
<summary> <b> General Description </b> </summary>
  In my project, the Arduino Uno serves as the central controller, coordinating all the inputs and outputs. It's responsible for processing user inputs, updating the LED matrices to display the rocket and asteroids, handling the menu on the LCD, and controlling the buzzer to provide sound effects. The Arduino is the brain behind the game, managing all aspects of gameplay, from navigation to firing, and ensuring that the game logic runs smoothly. It's connected to the other components via the GPIO pins, which handle communication with these elements.

The LED matrices are key to displaying the game environment. These two 8x8 matrices, driven by two MAX7219 LED drivers, serve as the screen where the rocket and asteroids are shown. The MAX7219 simplifies controlling multiple LEDs, reducing the number of pins needed and allowing the Arduino to focus on game logic. These matrices communicate with the Arduino using SPI, with specific pins assigned for DIN (Data In - Pin 11 since it is part of the SPI/ Serial Peripheral Interface), CLK (Clock - Pin 13 since it is traditionally used for the SCK/ Serial Clock), and LOAD (CS/ Chip Select - Pin 10 since it is traditionally used as SS/ Slave Select) to transmit data and refresh the display.

For displaying the score and accessing the game menu, there is the 16x2 LCD display. This LCD is controlled using the I2C protocol, which allows it to communicate with the Arduino using just two data pins—SDA (SDA - Pin A4/ Default SDA Pin) and SCL (SCL - Pin A5/ Default SCL Pin) —saving other valuable ports. The LCD shows information such as the score during gameplay and provides an interface for navigating the game’s menu.

The joystick is an essential input device in the game, used for navigating the menu and controlling the rocket’s movements. The joystick has two analog axes, which the Arduino reads via A0 and A1 for horizontal and vertical movement, respectively. A0 & A1 are among the six dedicated analog input pins (A0–A5) on the Arduino Uno, designed to read varying voltage levels, which is exactly what the joystick outputs for its X & Y axes. It also has a built-in button that is mapped to Pin 2 (since it corresponds to Interrupt 0) and serves the purpose of saving configurations in the settings section of the menu. This allows players to easily move the rocket on the screen and interact with the menu and in-game mechanics.

In addition to the joystick, a separate button is used for shooting projectiles towards the asteroids and starting the game. This button, connected to Pin 3 (corresponding to Interrupt 1), is essential for adding interactivity to the game, as players can press it to shoot and destroy the asteroids and the possibilty of controling the movement with one hand (via the joystick) and the shooting with the other hand.

To enhance the sensory experience, the buzzer provides audio feedback, particularly when the rocket collides with an asteroid. The buzzer is connected to Pin 9, which has PWM capability — allowing to generate varying signal patterns & create tones of different frequencies. Its sound can be triggered during collisions, making the gameplay feel more immersive and engaging.

The breadboard acts as the foundational platform for connecting all the components. It makes it easy to manage the various connections between the Arduino and peripherals without the need for soldering.
</details>

<details>
<summary> <b> Pins </b> </summary>
  
| Component             | Pin Number    | Function Description                                              |
|-----------------------|---------------|-------------------------------------------------------------------|
| **LED Matrix DIN**     | Pin 11        | Data input for LED matrix communication (SPI)                     |
| **LED Matrix CLK**     | Pin 13        | Clock signal for LED matrix communication (SPI)                   |
| **LED Matrix LOAD**    | Pin 10        | Chip select for LED matrix (SPI)                                  |
| **LCD SDA**            | Pin A4        | I2C data line for LCD communication                               |
| **LCD SCL**            | Pin A5        | I2C clock line for LCD communication                              |
| **Joystick VRx (Horizontal)** | Pin A0    | Analog input for joystick's horizontal axis movement              |
| **Joystick VRy (Vertical)**   | Pin A1    | Analog input for joystick's vertical axis movement                |
| **Joystick Button**    | Pin 2         | Button for joystick press (interrupt enabled)                     |
| **Shooting Button**    | Pin 3         | Button to trigger shooting action (interrupt enabled)             |
| **Buzzer**             | Pin 9         | Buzzer for sound feedback (PWM capable)                           |
</details>

<details>
<summary> <b> Bill of Materials </b> </summary>
  
| **Name**                        | **Source**                      | **Datasheet Link**                                                                                     |
|----------------------------------|---------------------------------|--------------------------------------------------------------------------------------------------------|
| Arduino Uno                      | University Of Bucharest      | [Arduino Uno Datasheet](https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf)                                 |
| I2C LCD Display 16x2             | [Optimu Digital](https://www.google.com/aclk?sa=L&ai=DChcSEwiquoygl6WKAxUIbEECHWaMIksYABAAGgJ3cw&co=1&ase=2&gclid=EAIaIQobChMIqrqMoJeligMVCGxBAh1mjCJLEAAYASAAEgJzXPD_BwE&sig=AOD64_3gKZwYI8h5odrFMlnXUxdu7K6dFw&q&nis=4&adurl&ved=2ahUKEwjlwoagl6WKAxUqR_EDHZSDEB4Q0Qx6BAgKEAE)      | [I2C LCD Datasheet](https://www.handsontec.com/dataspecs/module/I2C_1602_LCD.pdf)                                               |
| 8x8 LED Matrix with MAX7219 Driver (2x) | University Of Bucharest      | [MAX7219 LED Matrix Datasheet](https://www.handsontec.com/dataspecs/display/MAX7219-8x8.pdf)                                     |
| Buzzer                           | University Of Bucharest          | [Buzzer Datasheet](https://www.farnell.com/datasheets/2171929.pdf)                                      |
| Joystick                         | University Of Bucharest     | [Joystick Module Datasheet](https://www.hwkitchen.cz/user/related_files/joystick-modul-s-tlacitkem-datasheet-pdf.pdf)                                      |
| Button (for firing and starting) | University Of Bucharest      | [Button Datasheet](https://www.hdk.co.jp/pdf/eng/e291702.pdf)  |
| Connecting Wires                 | University Of Bucharest          | -                                                 |
| Breadboard                       | University Of Bucharest      | [Breadboard Datasheet](https://www.farnell.com/datasheets/1734497.pdf)                                             |
| Resistors (220 Ohm)             | University Of Bucharest      | -              |

</details>

<details>
<summary> <b> Block Diagram </b> </summary>
  
![image](https://github.com/user-attachments/assets/1ed82f72-cd13-4971-bc54-d2f1a142c8cd)
</details>

<details>
<summary> <b> Circuit Diagram </b> </summary>
  
![image](https://github.com/user-attachments/assets/876db71a-0964-4d03-a17e-ed9f1f99625a)
![image](https://github.com/user-attachments/assets/461ce925-60d8-4eaa-8e0a-31e8c1caa680)
</details>



<details>
<summary> <b> Photos and Testing </b> </summary>
  
![WhatsApp Image 2024-12-15 at 18 49 29_23edfa23](https://github.com/user-attachments/assets/a18f8312-529f-4e5a-8749-96b19ad77357)
![WhatsApp Image 2024-12-15 at 18 49 28_86ef5022](https://github.com/user-attachments/assets/fc4fb3f8-9696-427d-a1ec-c171eb699aca)
![WhatsApp Image 2024-12-15 at 18 49 28_bd1f7200](https://github.com/user-attachments/assets/322c319e-cd46-4590-9304-440a196fa5a2)
![image](https://github.com/user-attachments/assets/2d58b81f-f56a-48f3-ba69-dd8bed7362ed)
![WhatsApp Image 2024-12-15 at 18 49 28_24aeb993](https://github.com/user-attachments/assets/f9052ebf-edf1-49ea-8320-72689de5b382)

The functionality of the key components—LCD display, LED matrices, joystick, and buttons has been tested using the following program:
```cpp
#include "LedControl.h" 
#include <Wire.h>       
#include <LiquidCrystal_I2C.h> 

const int VRx = A0; 
const int VRy = A1; 
const int joystickButton = 2;   

const int newButton = 3; 

const byte dinPin = 11;
const byte clockPin = 13;
const byte loadPin = 10;
const byte numMatrices = 2;

const int buzzerPin = 9;

LedControl lc = LedControl(dinPin, clockPin, loadPin, numMatrices);
byte matrixBrightness = 2;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void testJoystick();
void animateMatrices();
void checkNewButton();
void displayTest();

void setup() {
  Serial.begin(9600);

  pinMode(joystickButton, INPUT_PULLUP);
  pinMode(newButton, INPUT);
  
  pinMode(buzzerPin, OUTPUT);

  tone(buzzerPin, 2000);
  delay(1000);
  tone(buzzerPin, 1000);
  delay(1000);
  noTone(buzzerPin); 

  for (int i = 0; i < numMatrices; i++) {
    lc.shutdown(i, false);           
    lc.setIntensity(i, matrixBrightness); 
    lc.clearDisplay(i);              
  }

  lcd.init();   
  lcd.backlight();   
  lcd.setCursor(0, 0); 
  displayTest(); 
}

void loop() {
  testJoystick();    
  checkNewButton();   
  // animateMatrices();  
}

// Function to test joystick functionality
void testJoystick() {
  int xValue = analogRead(VRx); // Read horizontal position
  int yValue = analogRead(VRy); // Read vertical position
  bool joystickPressed = (digitalRead(joystickButton) == LOW); // Check joystick button state

  // Print joystick status
  if (joystickPressed) {
    Serial.println(F("Joystick Button Pressed"));
  } else if (xValue < 200) {
    Serial.println("Joystick Left");
  } else if (xValue > 800) {
    Serial.println("Joystick Right");
  } else if (yValue < 200) {
    Serial.println("Joystick Up");
  } else if (yValue > 800) {
    Serial.println("Joystick Down");
  } else {
    Serial.println("Joystick Center");
  }

  delay(300);
}

// Function to check the new button
void checkNewButton() {
  bool newButtonPressed = (digitalRead(newButton) == HIGH); // Button is pressed when HIGH

  if (newButtonPressed) {
    Serial.println(F("New Button Pressed"));
    delay(300);
  }
}

// Function to animate LEDs on the matrices
void animateMatrices() {
  for (int matrix = numMatrices - 1; matrix >= 0; matrix--) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        lc.setLed(matrix, row, col, true); // Turn on the LED
        delay(25);
      }
    }
  }

  for (int matrix = numMatrices - 1; matrix >= 0; matrix--) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        lc.setLed(matrix, row, col, false); // Turn off the LED
        delay(25);
      }
    }
  }
}

// Function to display Game Ready on the LCD
void displayTest() {
  lcd.clear();               // Clear the LCD screen
  lcd.setCursor(0, 0);       // Set cursor to the first line
  lcd.print("Game Ready ;)!"); // Display "Game Ready ;)!" on the second line
  lcd.setCursor(0, 1);       // Set cursor to the second line
  lcd.print("I2C LCD Test"); // Display "I2C LCD Test" on the second line
}

```
The program utilizes the LedControl library for controlling the LED matrices, the LiquidCrystal_I2C library for the LCD display, and the analog and digital pins for reading the joystick and buttons. The LCD display and LED matrices were verified by displaying "Game Ready ;)! I2C LCD Test" and animating LEDs, respectively. Screenshots of the LCD and LED matrices in operation are included to show that both components are working as expected.

![WhatsApp Image 2024-12-15 at 18 49 28_0ea6306f](https://github.com/user-attachments/assets/810737e5-999e-400f-b1fd-d0798e3cd354)
![WhatsApp Image 2024-12-15 at 18 49 28_6a81f3ac](https://github.com/user-attachments/assets/2a1ec5c5-3ecb-4ed8-b6eb-6dcad213e355)

For testing the joystick and buttons, the joystick's horizontal and vertical values are read via analog pins A0 and A1, while the joystick button and the shooting button are read through digital pins (2 & 3). Serial monitor outputs show the joystick's movement (left, right, up, down, center) and button presses, which were tested by observing the corresponding outputs in the serial monitor:

https://github.com/user-attachments/assets/25265325-a636-4ac7-883d-4d63312b6a5a

</details>

---

## Software Design

<details>
<summary> <b> General Description </b> </summary>

The current version of the game software is fully functional and delivers a comprehensive and engaging gaming experience. It features a menu system, accessible through the LCD display using joystick movements, which provides four options: Game Mode, Leaderboard, Settings, and Easter Eggs. Players can tailor their gameplay by adjusting volume and brightness settings or selecting their preferred game mode. The Easter Eggs option adds an element of discovery and fun, with the menu displaying the count of discovered Easter eggs, encouraging exploration.

The game itself adapts dynamically to the selected mode. In Classic mode, players enjoy endless playtime without ammo or time restrictions, skillfully navigating a rocket to avoid or shoot asteroids. Meanwhile, Time Rush offers a competitive challenge with limited ammo, a fixed timer, and a high-score leaderboard stored in the Arduino Uno's EEPROM memory. Gameplay intensity is amplified by a dynamic asteroid spawning system, where the frequency and speed of asteroids increase over time, ensuring progressively challenging play.

To enhance reliability and precision, the program incorporates collision detection, enabling accurate interactions between the rocket and asteroids. A score tracking and leaderboard system tracks player performance, motivating improvement and replayability. Additionally, the inclusion of button debouncing ensures reliable input, minimizing false triggers and enhancing the overall user experience.

While the menu is conveniently displayed on the LCD screen, the gameplay unfolds on two LED matrices, offering a retro aesthetic and an expanded playfield. Together, these elements aim to create a polished and immersive gaming experience that balances fun and challenge.

</details>

<details>
<summary> <b> Library Selection </b> </summary>
  
For this project, I am using PlatformIO as the development environment. 

The following libraries have been used throughout the development:
- <Arduino.h>
  - Provides compatibility with the Arduino framework
- <LiquidCrystal_I2C.h>
  - Enables efficient I2C communication with the LCD display
  - Reduces pin usage compared to direct LCD connection
  - Provides comprehensive display control functions
  - Well-maintained and widely tested library
- "LedControl.h"
  - Specialized for controlling LED matrix displays
  - Efficient management of multiple cascaded LED matrices
  - Provides low-level control for custom animations
  - Optimized for MAX7219 LED driver
- <EEPROM.h>
  - Built-in Arduino library for persistent storage
  - Essential for maintaining leaderboard data
  - Reliable long-term data storage solution
  - Low memory overhead
</details>

<details>
<summary> <b> Innovation Elements </b> </summary>
  
The project introduces several elements that could be considered innovative in order to enhance the gaming experience. The use of two LED matrices creates a larger game area and enables seamless transitions of game elements between displays, providing an expanded, visually engaging playfield. The dynamic difficulty system ensures a progressively challenging experience by increasing difficulty based on gameplay time, featuring adaptive asteroid spawning rates and speed variations, with multiple game modes catering to different skill levels. The easter eggs add an element of surprise, rewarding players through hidden features unlocked via button combinations, achievements, or skill-based actions, encouraging exploration.
</details>

<details>
<summary> <b> Laboratory Functionality Implementation </b> </summary>

The project's functionality incorporates key features developed with knowledge from previous laboratory exercises to ensure precise and responsive gameplay. Interrupts and timers from Lab 2 are utilized to manage critical timing events, such as asteroid movement, projectile firing, and the countdown sequence, enabling smooth and synchronized game dynamics. 
```cpp
attachInterrupt(digitalPinToInterrupt(SHOOTING_BUTTON), shootingButtonISR, FALLING);
attachInterrupt(digitalPinToInterrupt(JOYSTICK_BUTTON), joystickButtonISR, FALLING);
```
These interrupt handlers are used for the shooting and joystick buttons, triggering on the FALLING edge for responsive input detection.

Button handling techniques from Lab 0 are employed to manage user inputs reliably, including the fire button and joystick button presses, ensuring seamless interaction during gameplay.
```cpp
void shootingButtonISR() {
  static unsigned long lastDebounceTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastDebounceTime > DELAY && !inMenu) {
    shootingButtonPressed = true;
    joystickButtonPressed = false;
  }
  lastDebounceTime = currentTime;
}
```
The code implements software debouncing using time comparison to prevent multiple triggers from a single button press.

Analog input handling from Lab 3 is used for joystick control, providing accurate and intuitive navigation within the game and menu system.
```cpp
int xValue = analogRead(JOYSTICK_VRX);
int yValue = analogRead(JOYSTICK_VRY);
```
AnalogRead is used for joystick position detection, reading values from 0-1023 to determine movement direction.

Additionally, in the project I've used PWM through the tone() function for sound generation, with volume control mapped to frequency values.
```cpp
void playSound(int volume) {
  int frequency = map(volume, 0, 100, 0, 4000);
  tone(BUZZER, frequency);
  delay(500);
  noTone(BUZZER);
}
```

These foundational elements, adapted from prior labs, contribute significantly to the game's overall precision and functionality.
</details>

<details>
<summary> <b> Project Structure and Functionality </b> </summary>
The structure of the project is designed to be modular, with a clear organization of variables, macros, and functions to streamline development and facilitate future changes. The variables and macros are grouped by functionality, with comments in the code to ensure easy modification or addition of new variables. This structured approach not only simplifies updates but also provides a stable starting state for the game. 

```cpp
#define LED_MATRIX_DIN 11
#define LED_MATRIX_CLK 13
#define LED_MATRIX_LOAD 10
#define LCD_SDA A4
#define LCD_SCL A5
#define JOYSTICK_VRX A0
#define JOYSTICK_VRY A1
#define JOYSTICK_BUTTON 2
#define SHOOTING_BUTTON 3
#define BUZZER 9

// General:
volatile bool shootingButtonPressed = false;
volatile bool joystickButtonPressed = false;
const long RIGHT_UP = 800;
const long LEFT_DOWN = 200;
const long DELAY = 200;
const int TOTAL_EASTER_EGGS = 3;
bool easterEggsFound[TOTAL_EASTER_EGGS] = {false};
unsigned long easterEggDisplayStartTime = 0;
bool isDisplayingEasterEgg = false;
unsigned long lastJoystickCheck = 0;

// etc...
```

At the heart of the project lies the game state management, which governs the game's flow using an enumerated state structure:
```cpp
enum GameState {
  MENU,
  STARTING,
  PLAYING,
  GAME_OVER
};
```
This structure enables smooth transitions between various states and ensures that specific behaviors are executed during each phase of the game. The core state machine is managed in the playGame() function, which is called once per loop in the main program. In the MENU state, the checkJoystick() function is used for menu navigation, allowing the user to scroll through options and access submenus for settings adjustments. The menu options and changes are displayed using the displayMenu() and displaySubmenu() functions. The settings, such as brightness and volume, can be adjusted by moving the joystick up or down within the respective submenu. These settings are then applied using the playSound(int volume) and adjustBrightness(int brightness) functions.

The displayOnLCD() function handles the display of text on the LCD screen, managing the separation of text across two lines:

```cpp
void displayOnLCD(const String &line1, const String &line2 = "") {
  if (line1 != currentDisplayText || line2 != currentDisplayText2) {
    lcd.clear();
    lcd.home();
    lcd.print(line1);
    if (line2.length() > 0) {
      lcd.setCursor(0, 1);
      lcd.print(line2);
    }
    currentDisplayText = line1;
    currentDisplayText2 = line2;
  }
}
```

When the user exits the menu and enters the game, pressing the shooting button triggers the start of the game in the selected mode. This is achieved by the following code snippet:
```cpp
if (shootingButtonPressed && !enteringName) { 
  roundStartTime = millis();
  currentGameState = STARTING;
} else if (!isDisplayingEasterEgg) {
  checkEasterEggs();
}
checkJoystick();

if (enteringName) {
  handleNameInput();
}

```
This triggers a countdown sequence displayed on the LED matrices using the displayPattern() function, which is followed by the transition to the STARTING state. The countdown is displayed across both matrices, with different numbers being shown using the following pattern:
```cpp
clearMatrices();
displayPattern(1, numberPatterns[0]); // 3 on second matrix
playSound(volume);

clearMatrices();
displayPattern(0, numberPatterns[1]); // 2 on first matrix
playSound(volume);

clearMatrices();
displayPattern(1, numberPatterns[2]); // 1 on second matrix
playSound(volume);

clearMatrices();
displayPattern(1, numberPatterns[4]); // G on second matrix
displayPattern(0, numberPatterns[3]); // 0 on first matrix

```

Once the countdown completes, the game state changes to STARTING, and the initializeGame() function sets up the game, initializing the score to 0 and resetting game elements such as the rocket, asteroids, and projectiles.

During the PLAYING phase, various functions are called to handle gameplay:

- handleGameControls() processes player input, including rocket movement and shooting mechanics.
- spawnAsteroid() spawns new asteroids at regular intervals.
- updateProjectiles() moves the projectiles fired by the rocket.
- Asteroid speeds are updated based on elapsed time:
  ```cpp
  if (elapsedSeconds >= 30){
    updateAsteroids(ASTEROID_SPEED3);
  } else if (elapsedSeconds >= 15){
    updateAsteroids(ASTEROID_SPEED2);
  } else {
    updateAsteroids(ASTEROID_SPEED1);
  }
  ```
- checkCollisions() handles collision detection between projectiles and asteroids as well as between the rocket and asteroids, triggering a game over if the rocket collides with an asteroid.
- updateDisplay() and updateScore() update the game visuals and score on the LED matrices.
- The game also checks for Easter eggs with checkEasterEggs() and updates ammo in Time Rush mode using updateAmmo().

The leaderboard system stores high scores using EEPROM memory. Functions like readLeaderboard(), storeLeaderboard(), and handleNameInput() are responsible for reading and saving leaderboard data, as well as handling the entry of player names for high scores.
```cpp
readLeaderboard(); // Loads leaderboard from EEPROM
storeLeaderboard(); // Saves new scores to EEPROM
handleNameInput(); // Manages name entry for high scores
```

To reset or initialize the leaderboard data, clearEEPROM() and initializeDefaultLeaderboard() can be used. These functions are called only once to initialize the leaderboard and then commented out after their first use.

Each component of the project was tested individually during development to ensure it functions as expected. The LCD display and LED matrix were tested to verify that they display the correct information at each stage of the game, from the menu to the game itself. Functions such as displayMenu(), displayPattern(), and displayOnLCD() were tested to ensure that text and game patterns appeared correctly on the display. Additionally, the EEPROM memory was tested by checking if scores were correctly stored and retrieved using functions like readLeaderboard() and storeLeaderboard(). Playtesting was also conducted to evaluate the overall functionality of the game, ensuring smooth transitions between game states, accurate input handling, and the correct updating of scores, ammo, and game objects throughout the game duration. This iterative testing process helped identify and resolve any issues early on.

<b>Key Interactions: </b>
1. Game Loop Flow:
  loop() → playGame() → State Machine → Input Handling → Game Updates → Display Updates
2. Input Chain:
  ISR Functions → handleGameControls() → Object Updates → Collision Checks → Display Updates
3. Menu Navigation:
  checkJoystick() → displayMenu()/displaySubmenu()/displaySettings() → LCD Updates → Settings Adjustments
4. Score Management:
  updateScore() → Game Over → handleNameInput() → storeLeaderboard() → readLeaderboard()

This structure creates a complete game system where each function has a specific role, but works together with the others to create the full gameplay experience. The code uses a state machine pattern to manage different game states, with clear separation between input handling, game logic, and display updates.

</details>

<details>
<summary> <b> Calibration Details </b> </summary>

Joystick Calibration
- Defined threshold values (LEFT_DOWN = 200, RIGHT_UP = 800)
- Dead zone implementation for stability
- Movement delay implementation for controlled response

Button Debouncing
- Implemented hardware interrupts with software debouncing
- 200ms debounce delay for reliable input
- State tracking for multiple button presses

LED Matrix Brightness
- Adjustable intensity levels (0-15)
- User-configurable through settings menu
</details>

<details>
<summary> <b> Optimization </b> </summary>

- The displayOnLCD function is designed to prevent unnecessary LCD updates, reducing flicker and improving the response time of the display. It only clears and updates the screen when the content being displayed changes, thus avoiding the constant refreshing of the display when no new data is provided.
  ```cpp
  void displayOnLCD(const String &line1, const String &line2 = "") {
    if (line1 != currentDisplayText || line2 != currentDisplayText2) {
      lcd.clear();
      lcd.home();
      lcd.print(line1);
      if (line2.length() > 0) {
        lcd.setCursor(0, 1);
        lcd.print(line2);
      }
      currentDisplayText = line1;
      currentDisplayText2 = line2;
    }
  }
  ```

- To control the LED matrices effectively, hardware SPI is used for fast communication with the display. The refresh rates are optimized for smooth animations, and selective updates are made only for the active game elements. This minimizes the overhead of constantly refreshing the entire matrix and helps focus processing power on updating only the relevant game components. 

- Memory management plays a considerable role in ensuring the stability of the game, especially when dealing with limited resources on the Arduino Uno. Compact data structures, such as the Point struct, are used to store the coordinates of game objects like the rocket and asteroids. This minimizes memory usage while maintaining the necessary data structure for proper gameplay functionality. Additionally, efficient use of array pools is employed for projectiles and asteroids, ensuring that memory is allocated statically for predictable performance and that objects are reused efficiently. This avoids memory fragmentation and ensures that the game can run smoothly over extended periods without running into memory-related issues.

- To ensure smooth gameplay, the game loop is state-based, allowing the game to transition smoothly between different phases, such as menu, game start, playing, and game over. Instead of relying on delay functions, which can hinder the responsiveness of the game, time-based updates are used to control the progression of game events. This enables more precise timing and responsive behavior throughout gameplay.

- Efficient collision detection algorithms are also employed to handle interactions between game objects (e.g., the rocket and asteroids) without causing delays in the game loop.

The combination of these optimizations leads to smooth gameplay, responsive controls, and efficient memory usage
</details>

<details>
<summary> <b> Demo Video </b> </summary>
<div align="center">
  <a href="https://www.youtube.com/watch?v=l4JrSOgvjtM&ab_channel=AlexV">
     <img 
      src="https://img.youtube.com/vi/l4JrSOgvjtM/maxresdefault.jpg" 
      alt="Rocket vs Asteroids" 
      style="width:100%;">
  </a>
</div>

Videoclipul poate fi văzut prin accesarea link-ului:
[Video pe YouTube](https://www.youtube.com/watch?v=l4JrSOgvjtM&ab_channel=AlexV)
</details>

---

## Results

---

## Conclusions
