#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Touch Sensor Pin
#define TOUCH_PIN 4  

// --- CUSTOM BITMAP GRAPHICS ---

// 16x16 Flappy Bird Sprite
const unsigned char PROGMEM bird_bmp[] = {
  0x01, 0xf0, 0x03, 0xf8, 0x07, 0xfc, 0x0f, 0xce, 0x1f, 0x87, 0x3f, 0x87, 0x7f, 0x87, 0xff, 0xff,
  0xff, 0xff, 0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x00, 0x00
};

// 16x16 T-Rex Sprite
const unsigned char PROGMEM dino_bmp[] = {
  0x00, 0x7e, 0x00, 0xff, 0x00, 0xe7, 0x00, 0xff, 0x00, 0xff, 0x00, 0xfc, 0x00, 0xff, 0x81, 0xff,
  0xc1, 0xff, 0xe3, 0xff, 0x7f, 0xfe, 0x3f, 0xfc, 0x1f, 0xf8, 0x07, 0x60, 0x07, 0x60, 0x05, 0x20
};

// 8x16 Cactus Sprite
const unsigned char PROGMEM cactus_bmp[] = {
  0x18, 0x18, 0x18, 0x18, 0x58, 0x58, 0x58, 0x58, 0x5b, 0x5b, 0x5b, 0x5b, 0x1a, 0x1a, 0x18, 0x18
};

// Game Variables
int gameMode = 0; // 0: Menu, 1: Flappy Bird, 2: Dino Game
int menuSelection = 1; // 1: Flappy Bird, 2: Dino Game
bool gameOver = false;
int score = 0;

// Touch Navigation Variables
bool lastTouchState = LOW;
unsigned long touchStartTime = 0;
bool isTouching = false;

// Flappy Bird Variables
int birdY = SCREEN_HEIGHT / 2;
int velocity = 0;
int gravity = 1;
unsigned long lastJumpTime = 0;
int pipeX = SCREEN_WIDTH;
int pipeGap = 35; // Increased gap to fit the new bird sprite
int pipeHeight = 25;

// Dino Game Variables
int groundLevel = SCREEN_HEIGHT - 16; // Adjusted for 16px tall Dino sprite
int dinoY = groundLevel;
int dinoVelocity = 0;
int dinoGravity = 1;
int cactusX = SCREEN_WIDTH;

// Display Initial Menu
void displayMenu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 10);
    display.println("Select Game:");
    display.setCursor(20, 30);
    display.println("1. Flappy Bird");
    display.setCursor(20, 50);
    display.println("2. Dino Game");
    display.display();
    delay(1000);
}

void setup() {
    pinMode(TOUCH_PIN, INPUT);
    Serial.begin(115200); 

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    displayMenu();
}

void loop() {
    if (gameMode == 0) {
        selectGame();
    } else if (gameMode == 1) {
        playFlappyBird();
    } else if (gameMode == 2) {
        playDinoGame();
    }
}

// Game Selection Logic (Tap to toggle, Hold to select)
void selectGame() {
    bool currentTouch = digitalRead(TOUCH_PIN);

    // Detect initial touch
    if (currentTouch == HIGH && lastTouchState == LOW) {
        touchStartTime = millis();
        isTouching = true;
    }

    // Detect holding
    if (currentTouch == HIGH && isTouching) {
        if (millis() - touchStartTime > 1000) {
            gameMode = menuSelection;
            resetGame();
            isTouching = false; 
            delay(500); 
            return;
        }
    }

    // Detect release (Tap)
    if (currentTouch == LOW && lastTouchState == HIGH) {
        if (millis() - touchStartTime < 1000) {
            menuSelection = (menuSelection == 1) ? 2 : 1; 
        }
        isTouching = false;
    }
    
    lastTouchState = currentTouch;
    
    // Draw Menu
    display.clearDisplay();
    display.setTextSize(1); // Standardizing size for 128x64 fit
    display.setTextColor(WHITE); // Fixed from RED to WHITE for monochrome display
    display.setCursor(5, 10);
    display.println("Tap:Switch|Hold:Play");

    if (menuSelection == 1) {
        display.setCursor(10, 30);
        display.print("> ");
    }
    display.setCursor(25, 30);
    display.println("Flappy Bird");

    if (menuSelection == 2) {
        display.setCursor(10, 50);
        display.print("> ");
    }
    display.setCursor(25, 50);
    display.println("Dino Game");

    display.display();
}

// Reset Game Variables
void resetGame() {
    score = 0;
    gameOver = false;
    birdY = SCREEN_HEIGHT / 2;
    dinoY = groundLevel;
    velocity = 0;
    dinoVelocity = 0;
    pipeX = SCREEN_WIDTH;
    cactusX = SCREEN_WIDTH;
}

// Flappy Bird Logic
void playFlappyBird() {
    if (gameOver) {
        displayGameOver();
        return;
    }

    // Jump when touched
    if (digitalRead(TOUCH_PIN) == HIGH && millis() - lastJumpTime > 200) {
        velocity = -6;
        lastJumpTime = millis();
    }

    velocity += gravity;
    birdY += velocity;

    // Boundary checks
    if (birdY >= SCREEN_HEIGHT - 16) birdY = SCREEN_HEIGHT - 16;
    if (birdY <= 0) birdY = 0;

    pipeX -= 3; // Slightly faster for fun
    if (pipeX < -10) {
        pipeX = SCREEN_WIDTH;
        pipeHeight = random(10, 30);
        score++;
    }

    // Collision Detection updated for 16x16 Bird Sprite
    if ((pipeX < 36 && pipeX > 15) && (birdY < pipeHeight - 2 || birdY + 14 > pipeHeight + pipeGap)) {
        gameOver = true;
    }

    display.clearDisplay();
    
    // Draw the new Bird sprite!
    display.drawBitmap(20, birdY, bird_bmp, 16, 16, WHITE);
    
    // Draw pipes
    display.fillRect(pipeX, 0, 10, pipeHeight, WHITE);
    display.fillRect(pipeX, pipeHeight + pipeGap, 10, SCREEN_HEIGHT - pipeHeight - pipeGap, WHITE);
    
    display.setCursor(100, 0);
    display.print("S: ");
    display.print(score);
    display.display();
    delay(50);
}

// Dino Game Logic
void playDinoGame() {
    if (gameOver) {
        displayGameOver();
        return;
    }

    // Jump when touched
    if (digitalRead(TOUCH_PIN) == HIGH && dinoY == groundLevel) {
        dinoVelocity = -10;
    }

    dinoVelocity += dinoGravity;
    dinoY += dinoVelocity;
    
    if (dinoY >= groundLevel) {
        dinoY = groundLevel;
        dinoVelocity = 0;
    }

    cactusX -= 3;
    if (cactusX < -10) {
        cactusX = SCREEN_WIDTH;
        score++;
    }

    // Collision Detection updated for 16x16 Dino and 8x16 Cactus
    if (cactusX < 22 && cactusX > 4 && dinoY > groundLevel - 12) {
        gameOver = true;
    }

    display.clearDisplay();
    
    // Draw the new Dino and Cactus sprites!
    display.drawBitmap(10, dinoY, dino_bmp, 16, 16, WHITE);
    display.drawBitmap(cactusX, groundLevel, cactus_bmp, 8, 16, WHITE);
    
    display.setCursor(100, 0);
    display.print("S: ");
    display.print(score);
    display.display();
    delay(50);
}

// Display Game Over Message
void displayGameOver() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(35, 20);
    display.println("GAME OVER!");
    display.setCursor(35, 40);
    display.println("Score: " + String(score));
    display.display();
    delay(2000);
    
    // Wait for touch release before going back to menu
    while(digitalRead(TOUCH_PIN) == HIGH) {
        delay(10); 
    }
    gameMode = 0;
}
