# 🕹️ ESP32 Mini Arcade: Flappy Bird & Dino Game

Transform a standard ESP32 development board into a pocket-sized arcade machine! Built with a 0.96-inch monochrome OLED display and a single capacitive touch sensor, this project packs two fully playable, iconic retro games into a tiny form factor: **Flappy Bird** and the **Chrome T-Rex Dino Game**.

Instead of relying on clunky buttons or joysticks, the entire system is driven by a smart, time-based touch interface. A quick tap lets you flap your wings or jump over cacti, while holding the sensor navigates the main menu. 

## ✨ Features
* **Two Playable Games:** Switch seamlessly between Flappy Bird and Dino Game.
* **One-Touch Control System:** A single capacitive touch sensor handles all UI navigation and in-game mechanics using tap-and-hold logic.
* **Custom Pixel Art:** Features bespoke 16x16 pixel `PROGMEM` bitmap sprites for the Bird, Dinosaur, and obstacles, replacing boring geometric shapes.
* **Dynamic Physics & Hitboxes:** Includes a custom gravity simulation and precise hitbox collision detection for accurate and fair gameplay.
* **Low Hardware Footprint:** Requires only three components and runs entirely on 3.3V logic.

---

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 Development Board (e.g., NodeMCU-32S, ESP32-WROOM)
* **Display:** 0.96" I2C OLED Display (SSD1306)
* **Input:** Capacitive Touch Sensor Module (e.g., TTP223)
* **Miscellaneous:** Breadboard and Jumper Wires

---

## 🔌 Detailed Wiring Guide
Both the OLED and the Touch Sensor run perfectly on **3.3V**, so we will power everything directly from the ESP32's 3.3V pin using a breadboard power rail.

### 1. Power Rails
* **ESP32 3V3 (3.3V)** ➡️ Breadboard Positive **(+)** Rail
* **ESP32 GND** ➡️ Breadboard Negative **(-)** Rail

### 2. OLED Display (I2C)
⚠️ *Warning: Always check the pin order on your specific OLED before powering it on. Some manufacturers swap the GND and VCC pins!*

| OLED Pin | Connects to... | Purpose |
| :--- | :--- | :--- |
| **VCC / VDD** | Breadboard **(+)** Rail | Powers the screen (3.3V). |
| **GND** | Breadboard **(-)** Rail | Ground connection. |
| **SCL / SCK** | ESP32 **GPIO 22** | I2C Clock line. |
| **SDA** | ESP32 **GPIO 21** | I2C Data line. |

### 3. Touch Sensor (TTP223)
| Touch Sensor Pin | Connects to... | Purpose |
| :--- | :--- | :--- |
| **VCC** | Breadboard **(+)** Rail | Powers the sensor (3.3V). |
| **GND** | Breadboard **(-)** Rail | Ground connection. |
| **SIG / I/O** | ESP32 **GPIO 4** | Sends the touch signal. |

---

## 💻 Software Setup

### Dependencies
Before compiling and uploading the code in the Arduino IDE, ensure you have installed the following libraries via the **Library Manager** (`Sketch` -> `Include Library` -> `Manage Libraries...`):
1. **Adafruit GFX Library** by Adafruit
2. **Adafruit SSD1306** by Adafruit

*(Note: The `Wire.h` library for I2C communication is included by default with the ESP32 Arduino Core).*

### Installation
1. Clone this repository or download the `.zip`.
2. Open the `.ino` sketch in the Arduino IDE.
3. Select your specific ESP32 board and COM port from the Tools menu.
4. Set the Upload Speed (Baud Rate) to `115200`.
5. Click **Upload**.

---

## 🎮 How to Play (Controls)
The entire system operates using a single touch button, utilizing time-based logic to distinguish between a "Tap" and a "Hold."

### Main Menu
* **Tap (Quick Touch):** Toggles the cursor between *Flappy Bird* and *Dino Game*.
* **Hold (1 Second):** Confirms your selection and starts the chosen game.

### In-Game
* **Tap:** Makes the Bird flap or the Dinosaur jump. 
* **Game Over:** When you crash, release your finger from the sensor. The game will automatically display your score for 2 seconds and return to the main menu.

---

## 🧠 Code Architecture Highlights
* **PROGMEM Graphics:** The custom pixel art is stored in Flash memory (`PROGMEM`) as hexadecimal arrays, keeping dynamic RAM usage incredibly low.
* **State Machine Logic:** The `loop()` utilizes a simple `gameMode` variable to seamlessly swap between rendering the menu, calculating Flappy Bird physics, and running the Dino game loop.
* **Gravity Simulation:** Velocity is updated constantly by a gravity variable. Tapping forces velocity negative (upward), which gravity then slowly degrades to pull the sprite back down.

## 📜 License
This project is open-source and available under the [MIT License](LICENSE). Feel free to fork, modify, and build your own mini arcade games!
