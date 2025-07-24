# 🦆 DuckTacToe

**Solo Project | Arduino, Embedded C++, Buttons, LEDs, Shift Registers**

A tangible Tic Tac Toe game built entirely with Arduino and embedded C++. This project combines classic game logic with physical hardware inputs and outputs, including buttons for player moves and multicolor LEDs to display game state. Shift registers efficiently control LED states, creating a responsive, interactive experience on a compact hardware setup.

---

## 🧠 What I Built

- Implemented full Tic Tac Toe game logic in Arduino C++, including turn management, win/draw detection, and game reset
- Managed real-time input from physical buttons to select spots on the board with debouncing logic for reliable presses
- Controlled red and blue LEDs (via shift registers) to visually indicate player moves: **Red for X**, **Blue for O**
- Programmed shift registers to handle multiple LEDs efficiently using bitwise operations and byte-level state management
- Integrated a buzzer with musical feedback for game events (e.g., win animations)
- Developed modular functions to maintain clean separation of concerns (game logic, hardware control, input handling)
- Designed wiring and hardware setup to optimize signal integrity and minimize complexity in controlling multiple outputs

---

## 🛠️ Hardware Overview

- **Inputs**: Three buttons used for cycling through available spots and selecting moves
- **Outputs**: 9 RGB LEDs arranged in a 3x3 grid, controlled via two 8-bit shift registers
- **Buzzer**: Plays simple melodies as audio feedback on game events
- **Microcontroller**: Arduino Uno managing game state, input/output, and LED control
- **Wiring**: Efficient use of shift registers reduces GPIO pin usage, while organized wiring supports maintainability and easy debugging

---

## ⚙️ Core Features

- Turn-based gameplay with clear tracking of active player (red or blue)
- Win and draw detection using row, column, and diagonal checks implemented with concise boolean logic
- Dynamic LED updates representing board state in real-time with smooth cycling of selections
- Reset functionality to restart the game with randomized first player
- Audio feedback through buzzer melodies enhancing user interaction
- Shift register byte manipulation for optimized LED control minimizing pin usage

---

## 🔧 Software Highlights

- Bitwise operations to update individual LEDs via shift registers efficiently
- Debounced button press handling to avoid erroneous multiple inputs
- Modular function design separating game logic, input management, LED control, and audio playback
- Use of arrays and loops for scalable, readable management of board state and LED indices
- Implementation of music playback via buzzer using timed tone sequences

---

This project demonstrates strong embedded software skills, including hardware interfacing, real-time input handling, and optimized resource management. It reflects proficiency in low-level programming, modular design, and creative problem solving — all valuable capabilities for software engineering roles involving embedded systems, IoT devices, or hardware-software integration.
