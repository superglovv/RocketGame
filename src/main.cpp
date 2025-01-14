#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>
#include <EEPROM.h>

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
unsigned long soundStartTime = 0;
unsigned long soundDuration = 500;
unsigned long displayMessageStartTime = 0;
int startupStep = 0;
bool gameOverSoundPlayed = false;
const unsigned long DISPLAY_TIME = 2000;
const unsigned long DIVIDING_UNIT = 1000;
const int maxScore = 9999;

// Menu related:
const String menuItems[] = {"Game Mode", "Leaderboard", "Settings", "Easter eggs"};
const int firstIndex = 0;
const int secondIndex = 1;
const int thirdIndex = 2;
const int fourthIndex = 3;
const int MENU_SIZE = 4;
const String gameModes[] = {"Classic", "Time Rush"};
const unsigned long EASTER_EGG_DISPLAY_TIME = 2000; 
int currentMenuIndex = 0;
int currentSubmenuIndex = 0;
int jbtncount = 0;
int leaderboardIndex = 0;
int gameModeChosen = 0;
const int timeRushMode = 1;
const int level = 100;
const int level_amount = 10;
const int max_intensity = 10;
const int max_frequency = 4000;
int brightness = 100;
int volume = 50;
int easterEggCount = 0;
bool inMenu = false;
bool inSubmenu = false;

// Leaderboard related:
const int playerNameSize = 10;
const char letters[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char playerName[playerNameSize] = {0};
int nameIndex = 0;
bool enteringName = false;
int currentLetter = 0;
String currentDisplayText = "";
String currentDisplayText2 = "";
struct LeaderboardEntry {
  char name[playerNameSize];
  int score;
};
const int nameSize = 8;
const int MAX_ENTRIES = 5;
LeaderboardEntry leaderboard[MAX_ENTRIES];
const int EEPROM_START_ADDR = 0;

// Game related:
const int MATRIX_SIZE = 8;
const int ROCKET_HEIGHT = 3;
const int ROCKET_WIDTH = 2;
const int ASTEROID_SIZE = 2;
const int MAX_PROJECTILES = 5;
const int MAX_ASTEROIDS = 5;
const unsigned long PROJECTILE_SPEED = 100;
const unsigned long ASTEROID_SPAWN_INTERVAL = 1500;
const unsigned long ASTEROID_SPEED1 = 600; 
const unsigned long ASTEROID_SPEED2 = 300; 
const unsigned long ASTEROID_SPEED3 = 150; 
const unsigned long MOVEMENT_DELAY = 150;
const unsigned long ASTEROID_SECOND_SPEED_TIME = 30000;
const unsigned long ASTEROID_FIRST_SPEED_TIME = 15000;
const unsigned long ASTEROID_SPAWN_INTERVAL_SLOW = 1500;
const unsigned long ASTEROID_SPAWN_INTERVAL_MEDIUM = 1000;
const unsigned long ASTEROID_SPAWN_INTERVAL_FAST = 750;
const unsigned long SCORE_UPDATE_INTERVAL = 1000;
const int INITIAL_AMMO = 10;
const int AMMO_REPLENISH_AMOUNT = 5;
const unsigned long AMMO_REPLENISH_INTERVAL = 10000;
const int TIME_SCORE = 10;
const int ASTEROID_SCORE = 30;

int currentAmmo = INITIAL_AMMO;
unsigned long lastAmmoReplenish = 0;
unsigned long lastScoreUpdate = 0;
const unsigned long roundTime = 60000;
unsigned long roundStartTime;
int gameScore = 0; 

LedControl lc = LedControl(LED_MATRIX_DIN, LED_MATRIX_CLK, LED_MATRIX_LOAD, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

enum GameState {
  MENU,
  STARTING,
  PLAYING,
  GAME_OVER
};

GameState currentGameState = MENU;

struct Point {
  int x;
  int y;
};

struct Rocket {
  Point pos;
  Point shape[4];
};

struct Projectile {
  Point pos;
  bool active;
};

struct Asteroid {
  Point pos;
  bool active;
};

Rocket rocket;
Projectile projectiles[MAX_PROJECTILES];
Asteroid asteroids[MAX_ASTEROIDS];
unsigned long lastProjectileMove = 0;
unsigned long lastAsteroidMove = 0;
unsigned long lastAsteroidSpawn = 0;

const byte numberPatterns[MATRIX_SIZE-3][MATRIX_SIZE] = {
    { // Number 3
        B00111100,
        B01111110,
        B00000110,
        B00011100,
        B00000110,
        B01000110,
        B00111100,
        B00011000
    },
    { // Number 2
        B00111100,
        B01111110,
        B01000110,
        B00001100,
        B00011000,
        B00110000,
        B01111110,
        B01111110
    },
    { // Number 1
        B00011000,
        B00111000,
        B01111000,
        B00011000,
        B00011000,
        B00011000,
        B01111110,
        B01111110
    },
    { // Number 0
        B00111100,
        B01111110,
        B01100110,
        B01100110,
        B01100110,
        B01100110,
        B01111110,
        B00111100
    },
    { // Letter G
        B00111110,
        B01111111,
        B01100000,
        B01100000,
        B01101110,
        B01100110,
        B01111110,
        B00111100
    }
};


void displayPattern(int matrix, const byte pattern[MATRIX_SIZE]) {
    for (int row = 0; row < MATRIX_SIZE; row++) {
        byte rowPattern = pattern[row];
        for (int col = 0; col < MATRIX_SIZE; col++) {
            lc.setLed(matrix, row, col, bitRead(rowPattern, 7 - col));
        }
    }
}

void clearMatrices() {
    lc.clearDisplay(0);
    lc.clearDisplay(1);
}

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

void displayMenu() {
  String line1 = ">" + menuItems[currentMenuIndex];
  String line2 = (currentMenuIndex + 1 < MENU_SIZE) ? menuItems[currentMenuIndex + 1] : "";
  displayOnLCD(line1, line2);
}

void displaySubmenu() {
  String line1 = menuItems[currentMenuIndex];
  String line2 = "";

  if (currentMenuIndex == firstIndex) { // "Game Mode"
      line2 = '*' + gameModes[gameModeChosen];
      displayOnLCD(line1, line2);
    
  } else if (currentMenuIndex == secondIndex) { // "Leaderboard"
    line2 = String(leaderboard[leaderboardIndex].name);
    line2 += ": ";
    line2 += String(leaderboard[leaderboardIndex].score);
    displayOnLCD(line1, line2);
    
  } else if (currentMenuIndex == thirdIndex) { // "Settings"
    if (currentSubmenuIndex == firstIndex) { // Brightness
      line2 = "Brightness: " + String(brightness);
    } else if (currentSubmenuIndex == 1) { // Volume
      line2 = "Volume: " + String(volume);
    }
    displayOnLCD(line1, line2);
    
  } else if (currentMenuIndex == 3) { // "Easter eggs"
    line2 = "Count: " + String(easterEggCount);
    displayOnLCD(line1, line2);
  }
}

void displaySettingsAdjust() {
  String settingName;
  int value;
  
  if (currentSubmenuIndex == 0) {
    settingName = "Brightness";
    value = brightness;
  } else {
    settingName = "Volume";
    value = volume;
  }
  
  displayOnLCD(settingName, String(value) + "%");
}

void readLeaderboard() {
  int addr = EEPROM_START_ADDR;
  for (int i = 0; i < MAX_ENTRIES; i++) {
    memset(leaderboard[i].name, 0, playerNameSize);
    
    for (int j = 0; j < playerNameSize; j++) {
      char c = EEPROM.read(addr + j);
      if (c == 0 || c == 0xFF) break;
      leaderboard[i].name[j] = c;
    }
    
    addr += playerNameSize;
    byte highByte = EEPROM.read(addr);
    byte lowByte = EEPROM.read(addr + 1);
    leaderboard[i].score = (highByte << MATRIX_SIZE) | lowByte;
    
    if (leaderboard[i].score < 0 || leaderboard[i].score > maxScore) {
      leaderboard[i].score = 0;
    }
    
    addr += 2;
  }
}

void storeLeaderboard(const char* name, int score) {
  readLeaderboard();
  
  int insertPos = MAX_ENTRIES;
  for (int i = 0; i < MAX_ENTRIES; i++) {
    if (score > leaderboard[i].score) {
      insertPos = i;
      break;
    }
  }
  
  if (insertPos < MAX_ENTRIES) {
    for (int i = MAX_ENTRIES - 1; i > insertPos; i--) {
      strncpy(leaderboard[i].name, leaderboard[i-1].name, playerNameSize);
      leaderboard[i].score = leaderboard[i-1].score;
    }
    
    strncpy(leaderboard[insertPos].name, name, 9);
    leaderboard[insertPos].name[9] = '\0';
    leaderboard[insertPos].score = score;
    
    int addr = EEPROM_START_ADDR;
    for (int i = 0; i < MAX_ENTRIES; i++) {
      for (int j = 0; j < playerNameSize; j++) {
        EEPROM.write(addr + j, leaderboard[i].name[j]);
      }
      
      addr += playerNameSize;
      EEPROM.write(addr, (leaderboard[i].score >> nameSize) & 0xFF);
      EEPROM.write(addr + 1, leaderboard[i].score & 0xFF);
      addr += 2;
    }
  }
  
  Serial.println("\nCurrent Leaderboard:");
  for (int i = 0; i < MAX_ENTRIES; i++) {
    Serial.print(i + 1);
    Serial.print(". ");
    Serial.print(leaderboard[i].name);
    Serial.print(": ");
    Serial.println(leaderboard[i].score);
  }
}

void clearEEPROM() {
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0xFF);
  }
}

void initializeDefaultLeaderboard() {
  storeLeaderboard(" ", 0);
  storeLeaderboard(" ", 0);
  storeLeaderboard(" ", 0);
  storeLeaderboard(" ", 0);
  storeLeaderboard(" ", 0);
}

void handleNameInput() {
  int yValue = analogRead(JOYSTICK_VRY);
  int xValue = analogRead(JOYSTICK_VRX);
  static unsigned long lastInput = 0;
  
  if (millis() - lastInput > DELAY) { 
    if (yValue < LEFT_DOWN) {
      currentLetter = (currentLetter + 1) % 26;
      displayOnLCD("Enter name:", String(playerName) + letters[currentLetter]);
      lastInput = millis();
    }
    else if (yValue > RIGHT_UP) {
      currentLetter = (currentLetter + 25) % 26;
      displayOnLCD("Enter name:", String(playerName) + letters[currentLetter]);
      lastInput = millis();
    }
    else if (xValue > RIGHT_UP && nameIndex < nameSize) {  // Right - confirm letter
      playerName[nameIndex++] = letters[currentLetter];
      playerName[nameIndex] = '\0';
      currentLetter = 0;
      displayOnLCD("Enter name:", String(playerName) + letters[currentLetter]);
      lastInput = millis();
    }
    else if (xValue < LEFT_DOWN && nameIndex > 0) {  // Left - backspace
      playerName[--nameIndex] = '\0';
      displayOnLCD("Enter name:", String(playerName) + letters[currentLetter]);
      lastInput = millis();
    }
  }
  
  if (shootingButtonPressed) {  // Shooting button to confirm name
    if (nameIndex > 0) {
      storeLeaderboard(playerName, gameScore);
      enteringName = false;
      memset(playerName, 0, sizeof(playerName));
      nameIndex = 0;
      currentGameState = MENU;
      displayOnLCD("Shoot to start", "Right to Menu");
    }
    shootingButtonPressed = false;
  }
}

void playSound(int volume) {
  int frequency = map(volume, 0, level, 0, max_frequency);
  
  tone(BUZZER, frequency);
}

void stopSound() {
  noTone(BUZZER);
}

void adjustBrightness(int brightness) {
  int intensity = map(brightness, 0, level, 0, max_intensity);
  
  lc.setIntensity(0, intensity);
  lc.setIntensity(1, intensity);
}

void checkJoystick() {
  if (millis() - lastJoystickCheck < DELAY) return;

  int xValue = analogRead(JOYSTICK_VRX);
  int yValue = analogRead(JOYSTICK_VRY);
  static bool inSettingsAdjust = false;

  if (!inMenu) {
    if (xValue > RIGHT_UP && enteringName == false) {
      inMenu = true;
      currentMenuIndex = 0;
      displayMenu();
    }
  } else if (!inSubmenu) {
    if (xValue > RIGHT_UP) {
      inSubmenu = true;
      currentSubmenuIndex = 0;
      displaySubmenu();
    } else if (xValue < LEFT_DOWN) {
      inMenu = false;
      displayOnLCD("Shoot to start", "Right to Menu");
    } else if (yValue > RIGHT_UP && currentMenuIndex < MENU_SIZE - 1) {
      currentMenuIndex++;
      displayMenu();
    } else if (yValue < LEFT_DOWN && currentMenuIndex > firstIndex) {
      currentMenuIndex--;
      displayMenu();
    }
  } else if (inSubmenu && currentMenuIndex == thirdIndex && !inSettingsAdjust) {
    if (xValue < LEFT_DOWN) {
      inSubmenu = false;
      displayMenu();
    } else if (xValue > RIGHT_UP) {
      inSettingsAdjust = true;
      displaySettingsAdjust();
    } else if (yValue > RIGHT_UP && currentSubmenuIndex < secondIndex) {
      currentSubmenuIndex++;
      displaySubmenu();
    } else if (yValue < LEFT_DOWN && currentSubmenuIndex > firstIndex) {
      currentSubmenuIndex--;
      displaySubmenu();
    }
  } else if (inSettingsAdjust) { // In settings adjustment menu
    if (xValue < LEFT_DOWN) {
      inSettingsAdjust = false;
      adjustBrightness(brightness);
      displaySubmenu();
    } else if (yValue > RIGHT_UP) { // Decrease value
      if (currentSubmenuIndex == firstIndex && brightness > 0) {
        brightness = max(0, brightness - level_amount);
        displaySettingsAdjust();
      } else if (currentSubmenuIndex == secondIndex && volume > 0) {
        volume = max(0, volume - level_amount);
        displaySettingsAdjust();
      }
    } else if (yValue < LEFT_DOWN) { // Increase value
      if (currentSubmenuIndex == firstIndex && brightness < level) {
        brightness = min(level, brightness + level_amount);
        displaySettingsAdjust();
      } else if (currentSubmenuIndex == secondIndex && volume < level) {
        volume = min(level, volume + level_amount);
        displaySettingsAdjust();
      }
    }
  } else {
    if (xValue < LEFT_DOWN) {
      inSubmenu = false;
      displayMenu();
    }

    if (currentMenuIndex == firstIndex) {
      if (yValue > RIGHT_UP && gameModeChosen < (sizeof(gameModes) / sizeof(gameModes[0])) - 1) {
        gameModeChosen++;
        displaySubmenu();
      } else if (yValue < LEFT_DOWN && gameModeChosen > 0) {
        gameModeChosen--;
        displaySubmenu();
      }
    }

    if (currentMenuIndex == secondIndex) {
      if (yValue > RIGHT_UP && leaderboardIndex < (sizeof(leaderboard) / sizeof(leaderboard[0])) - 1) {
        leaderboardIndex++;
        displaySubmenu();
      } else if (yValue < LEFT_DOWN && leaderboardIndex > 0) {
        leaderboardIndex--;
        displaySubmenu();
      }
    }
  }

  lastJoystickCheck = millis();
}

void checkEasterEggs() {
  unsigned long currentTime = millis();

  if (!easterEggsFound[0] && jbtncount >= 3) {
    easterEggsFound[0] = true;
    easterEggCount++;
    isDisplayingEasterEgg = true;
    easterEggDisplayStartTime = currentTime;
    displayOnLCD("hello there!");
  }
  if (!easterEggsFound[1] && currentGameState == GAME_OVER && 
      gameScore > leaderboard[MAX_ENTRIES-1].score && 
      !enteringName) {
    easterEggsFound[1] = true;
    easterEggCount++;
    isDisplayingEasterEgg = true;
    easterEggDisplayStartTime = currentTime;
    displayOnLCD("Winner Winner,", " Chicken Dinner!");
    displayMessageStartTime = currentTime;
  }
  
  if (isDisplayingEasterEgg && 
      (currentTime - easterEggDisplayStartTime >= EASTER_EGG_DISPLAY_TIME)) {
    isDisplayingEasterEgg = false;
  }
}

void shootingButtonISR() {
  static unsigned long lastDebounceTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastDebounceTime > DELAY && !inMenu) {
    shootingButtonPressed = true;
    joystickButtonPressed = false;
  }
  lastDebounceTime = currentTime;
}

void joystickButtonISR() {
  static unsigned long lastDebounceTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastDebounceTime > DELAY && !inMenu) {
    jbtncount++;
    joystickButtonPressed = true;
    shootingButtonPressed = false;
  }
  lastDebounceTime = currentTime;
}

void updateRocketShape() {
  rocket.shape[0] = {rocket.pos.x, rocket.pos.y + 2};     // Nose
  rocket.shape[1] = {rocket.pos.x - 1, rocket.pos.y + 1}; // Left wing
  rocket.shape[2] = {rocket.pos.x, rocket.pos.y + 1};     // Center
  rocket.shape[3] = {rocket.pos.x + 1, rocket.pos.y + 1}; // Right wing
}

void initializeGame() {
  gameScore = 0;
  currentGameState = PLAYING;
  
  if (gameModeChosen == timeRushMode) { // Time Rush mode
        currentAmmo = INITIAL_AMMO;
        lastAmmoReplenish = millis();
    }

  rocket.pos = {MATRIX_SIZE/2, 0};
  updateRocketShape();
  
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    projectiles[i].active = false;
  }
  
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    asteroids[i].active = false;
  }
  
  clearMatrices();
}

void updateProjectiles() {
  if (currentGameState != PLAYING) return;

  if (millis() - lastProjectileMove > PROJECTILE_SPEED) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
      if (projectiles[i].active) {
        projectiles[i].pos.y++;
        if (projectiles[i].pos.y >= MATRIX_SIZE * 2) {
          projectiles[i].active = false;
        }
      }
    }
    lastProjectileMove = millis();
  }
}

void updateAmmo() {
    unsigned long currentTime = millis();
    if (currentTime - lastAmmoReplenish >= AMMO_REPLENISH_INTERVAL) {
        currentAmmo += AMMO_REPLENISH_AMOUNT;
        lastAmmoReplenish = currentTime;
    }
}

void spawnAsteroid() {
  if (currentGameState != PLAYING) return;
  
  static unsigned long lastSpawnTime = 0;
  unsigned long currentSpawnInterval;
  unsigned long elapsedSeconds = (millis() - roundStartTime) / DIVIDING_UNIT;
  
  if (elapsedSeconds >= ASTEROID_SECOND_SPEED_TIME / DIVIDING_UNIT) {
    currentSpawnInterval = ASTEROID_SPAWN_INTERVAL_FAST;
  } else if (elapsedSeconds >= ASTEROID_FIRST_SPEED_TIME / DIVIDING_UNIT) {
    currentSpawnInterval = ASTEROID_SPAWN_INTERVAL_MEDIUM;
  } else {
    currentSpawnInterval = ASTEROID_SPAWN_INTERVAL_SLOW;
  }
  
  if (millis() - lastSpawnTime > currentSpawnInterval) { 
    int asteroidsToSpawn = (elapsedSeconds >= ASTEROID_SECOND_SPEED_TIME / DIVIDING_UNIT) ? 2 : 1;
    
    for (int spawn = 0; spawn < asteroidsToSpawn; spawn++) {
      for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
          asteroids[i].active = true;
          asteroids[i].pos = {static_cast<int>(random(0, MATRIX_SIZE - ASTEROID_SIZE)), 15};
          break;
        }
      }
    }
    lastSpawnTime = millis();
  }
}

void updateAsteroids(unsigned long asteroidSpeed) {
  if (currentGameState != PLAYING) return;
  
    if (millis() - lastAsteroidMove > asteroidSpeed) {
      for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
          asteroids[i].pos.y--;
          if (asteroids[i].pos.y < 0) {
            asteroids[i].active = false;
          }
        }
      }
      lastAsteroidMove = millis();
    }
  
}

void updateScore() {
  unsigned long currentTime = millis();
  if (currentTime - lastScoreUpdate >= SCORE_UPDATE_INTERVAL) {
    gameScore += TIME_SCORE;
    lastScoreUpdate = currentTime;
  }
}

void updateDisplay() {
  clearMatrices();
  
  if (currentGameState == PLAYING) {
    for (int i = 0; i < MATRIX_SIZE/2; i++) {
      Point p = rocket.shape[i];
      if (p.y >= 0 && p.y < MATRIX_SIZE) {
        lc.setLed(0, 7 - p.y, p.x, true);
      }
    }
    
    for (int i = 0; i < MAX_PROJECTILES; i++) {
      if (projectiles[i].active) {
        int matrix = projectiles[i].pos.y / MATRIX_SIZE;
        int y = projectiles[i].pos.y % MATRIX_SIZE;
        if (matrix >= 0 && matrix < 2) {
          lc.setLed(matrix, 7 - y, projectiles[i].pos.x, true);
        }
      }
    }
    
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
      if (asteroids[i].active) {
        for (int ax = 0; ax < ASTEROID_SIZE; ax++) {
          for (int ay = 0; ay < ASTEROID_SIZE; ay++) {
            int matrix = (asteroids[i].pos.y + ay) / MATRIX_SIZE;
            int y = (asteroids[i].pos.y + ay) % MATRIX_SIZE;
            int x = asteroids[i].pos.x + ax;
            if (matrix >= 0 && matrix < 2) {
              lc.setLed(matrix, 7 - y, x, true);
            }
          }
        }
      }
    }
  }
}

void handleGameControls() {
    if (currentGameState != PLAYING) return;

    static unsigned long lastMoveTime = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastMoveTime >= MOVEMENT_DELAY) {
        int yValue = analogRead(JOYSTICK_VRY);
        
        if (yValue < LEFT_DOWN && rocket.pos.x > 1) {
            rocket.pos.x--;
            updateRocketShape();
            lastMoveTime = currentTime;
        }
        else if (yValue > RIGHT_UP && rocket.pos.x < MATRIX_SIZE - 2) {
            rocket.pos.x++;
            updateRocketShape();
            lastMoveTime = currentTime;
        }
    }

    if (shootingButtonPressed) {
        if (gameModeChosen == timeRushMode && currentAmmo <= 0) {
            shootingButtonPressed = false;
            return;
        }
        
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (!projectiles[i].active) {
                projectiles[i].active = true;
                projectiles[i].pos = {rocket.pos.x, rocket.pos.y + 3};
                
                if (gameModeChosen == timeRushMode) {
                    currentAmmo--;
                }
                
                shootingButtonPressed = false;
                break;
            }
        }
    }
}

bool checkCollision(Point p1, Point p2) {
  return (p1.x == p2.x && p1.y == p2.y);
}

void checkCollisions() {
  if (currentGameState != PLAYING) return;
  
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectiles[i].active) {
      for (int j = 0; j < MAX_ASTEROIDS; j++) {
        if (asteroids[j].active) {
          for (int ax = 0; ax < ASTEROID_SIZE; ax++) {
            for (int ay = 0; ay < ASTEROID_SIZE; ay++) {
              Point asteroidPoint = {asteroids[j].pos.x + ax, asteroids[j].pos.y + ay};
              if (checkCollision(projectiles[i].pos, asteroidPoint)) {
                projectiles[i].active = false;
                asteroids[j].active = false;
                gameScore += ASTEROID_SCORE;
              }
            }
          }
        }
      }
    }
  }
  
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
    if (asteroids[i].active) {
      for (int j = 0; j < 4; j++) {
        for (int ax = 0; ax < ASTEROID_SIZE; ax++) {
          for (int ay = 0; ay < ASTEROID_SIZE; ay++) {
            Point asteroidPoint = {asteroids[i].pos.x + ax, asteroids[i].pos.y + ay};
            if (checkCollision(rocket.shape[j], asteroidPoint)) {
              currentGameState = GAME_OVER;
              return;
            }
          }
        }
      }
    }
  }
}

void playGame() {
  unsigned long elapsedSeconds = (millis() - roundStartTime) / DIVIDING_UNIT;

  switch (currentGameState) {
    case MENU:
      if (shootingButtonPressed && !enteringName) { 
          roundStartTime = millis();
          currentGameState = STARTING;
          startupStep = 0;
      } else if (!isDisplayingEasterEgg) {
        checkEasterEggs();
      }
      checkJoystick();

      if (enteringName) {
        handleNameInput();
      }
      break;
      
    case STARTING: {
      unsigned long currentTime = millis();
      
      if (startupStep == 0) {
        soundStartTime = currentTime;
        clearMatrices();
        displayPattern(1, numberPatterns[0]); // 3
        playSound(volume);
        startupStep = 1;
      }
      
      switch(startupStep) {
        case 1:
          if (currentTime - soundStartTime >= soundDuration) {
            stopSound();
            clearMatrices();
            displayPattern(0, numberPatterns[1]); // 2
            playSound(volume);
            soundStartTime = currentTime;
            startupStep = 2;
          }
          break;
          
        case 2:
          if (currentTime - soundStartTime >= soundDuration) {
            stopSound();
            clearMatrices();
            displayPattern(1, numberPatterns[2]); // 1
            playSound(volume);
            soundStartTime = currentTime;
            startupStep = 3;
          }
          break;
          
        case 3:
          if (currentTime - soundStartTime >= soundDuration) {
            stopSound();
            clearMatrices();
            displayPattern(1, numberPatterns[4]); // G
            displayPattern(0, numberPatterns[3]); // 0
            playSound(volume);
            soundStartTime = currentTime;
            startupStep = 4;
          }
          break;
          
        case 4:
          if (currentTime - soundStartTime >= soundDuration) {
            stopSound();
            clearMatrices();
            initializeGame();
            lastScoreUpdate = currentTime;
            roundStartTime = currentTime;
            startupStep = 0; 
          }
          break;
      }
      
      displayOnLCD("Get Ready!", "");
      break;
    }
      
    case PLAYING:
      if (gameModeChosen == timeRushMode) {
        if (millis() - roundStartTime >= roundTime){
          currentGameState = GAME_OVER;
        } else {
          handleGameControls();
          spawnAsteroid();
          updateProjectiles();
          if (elapsedSeconds >= ASTEROID_SECOND_SPEED_TIME / DIVIDING_UNIT){
            updateAsteroids(ASTEROID_SPEED3);
          } else if (elapsedSeconds >= ASTEROID_FIRST_SPEED_TIME / DIVIDING_UNIT){
            updateAsteroids(ASTEROID_SPEED2);
          } else {
            updateAsteroids(ASTEROID_SPEED1);
          }
          checkCollisions();
          updateDisplay();
          updateScore();
          checkEasterEggs();
        }
        updateAmmo();
        displayOnLCD("Score: " + String(gameScore), "Ammo: " + String(currentAmmo));
      } else {
        handleGameControls();
        spawnAsteroid();
        updateProjectiles();
        if (elapsedSeconds >= ASTEROID_SECOND_SPEED_TIME / DIVIDING_UNIT){
            updateAsteroids(ASTEROID_SPEED3);
          } else if (elapsedSeconds >= ASTEROID_FIRST_SPEED_TIME / DIVIDING_UNIT){
            updateAsteroids(ASTEROID_SPEED2);
          } else {
            updateAsteroids(ASTEROID_SPEED1);
          }
        checkCollisions();
        updateDisplay();
        updateScore();
        displayOnLCD("Score: " + String(gameScore), "Time: " + String(elapsedSeconds) + "s");
      }
      break;
      
    case GAME_OVER:
      unsigned long currentTime = millis();
    
      if (displayMessageStartTime == 0) {
          displayMessageStartTime = currentTime;
          gameOverSoundPlayed = false;
          playSound(volume);
          displayOnLCD("Game Over!", "Score: " + String(gameScore));
      } 
      else if (!gameOverSoundPlayed && currentTime - displayMessageStartTime >= soundDuration) {
          stopSound();
          gameOverSoundPlayed = true;
      }
      else if (currentTime - displayMessageStartTime >= DISPLAY_TIME) {
          if (gameModeChosen == timeRushMode) {
              checkEasterEggs();
              if (gameScore > leaderboard[MAX_ENTRIES-1].score) {
                  currentGameState = MENU;
                  enteringName = true;
                  currentLetter = 0;
                  displayOnLCD("Enter name:", String(playerName) + letters[currentLetter]);
              } else {
                  currentGameState = MENU;
                  displayOnLCD("Score too low", "Try again!");
                  displayMessageStartTime = millis();
              }
          } else {
              currentGameState = MENU;
              displayOnLCD("Shoot to start", "Right to Menu");
          }
          displayMessageStartTime = 0;
          gameOverSoundPlayed = false;
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);
  pinMode(SHOOTING_BUTTON, INPUT_PULLUP);
  pinMode(LED_MATRIX_DIN, OUTPUT);
  pinMode(LED_MATRIX_CLK, OUTPUT);
  pinMode(LED_MATRIX_LOAD, OUTPUT);
  pinMode(JOYSTICK_VRX, INPUT);
  pinMode(JOYSTICK_VRY, INPUT);
  pinMode(BUZZER, OUTPUT);

  shootingButtonPressed = false;
  joystickButtonPressed = false;
  
  attachInterrupt(digitalPinToInterrupt(SHOOTING_BUTTON), shootingButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_BUTTON), joystickButtonISR, FALLING);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Default EEPROM values:
  // clearEEPROM();
  // initializeDefaultLeaderboard();
  // Serial.println("Default leaderboard initialized!");
  
  readLeaderboard();

  for (int i = 0; i < 2; i++) {
    lc.shutdown(i, false);           
    lc.setIntensity(i, max_intensity); 
    lc.clearDisplay(i);              
  }

  for (int matrix = 1; matrix >= 0; matrix--) {
    for (int row = 0; row < MATRIX_SIZE; row++) {
      for (int col = 0; col < MATRIX_SIZE; col++) {
        lc.setLed(matrix, row, col, true);
      }
    }
  }

  displayOnLCD("Shoot to start", "Right to Menu");
  
}

void loop() {
  playGame();
}