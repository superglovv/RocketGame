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

For this project, I am using PlatformIO as the development environment. 

The following libraries will be used throughout the development:
- <LiquidCrystal_I2C.h>
- "LedControl.h"
- <Wire.h>
(As the project progresses, I will continue to update the libraries list.)

---

## Results

---

## Conclusions
