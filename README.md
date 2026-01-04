# Arduino Pong Console

This repository contains an embedded systems project implementing a two-player Pong game on an Arduino-compatible microcontroller. The project emphasizes real-time input handling, on-device graphics rendering, and hardware–software integration in a standalone system.

---

## Overview

This project recreates the classic Pong game using a microcontroller, joystick inputs, and a display. All game logic—including paddle control, collision detection, scoring, and rendering—is handled directly on the embedded hardware, without reliance on an external computer.

The project was designed to explore:
- Real-time embedded programming
- Analog input processing
- Low-level graphics rendering
- System-level integration of hardware and software

---

## Features

- Two-player joystick-controlled gameplay  
- Real-time graphics rendering on a TFT/OLED display  
- Collision detection and scoring logic  
- Standalone embedded operation  
- Compact and self-contained hardware design  

---

## Repository Structure

Arduino_Pong_Console

│

├── Arduino_Pong_Console.ino # Main Arduino sketch

│

├── .gitignore

└── README.md


Generated build artifacts are intentionally excluded from version control.

---

## Hardware

- Arduino-compatible microcontroller (e.g., Arduino Nano)
- Display (TFT or OLED)
- 2× analog thumbstick joysticks
- Passive components (resistors, wiring, breadboard or PCB)
- External power (USB or battery)

---

## Software

- Arduino IDE
- C++
- Arduino core libraries
- Display driver libraries (as required by hardware)

---

## Usage

1. Open `Arduino_Pong_Console.ino` in the Arduino IDE  
2. Select the correct board and serial port  
4. Power the system and play using the joysticks  

No external computer is required during operation.

---

## Notes

This project focuses on embedded design principles rather than graphical complexity. Emphasis was placed on responsiveness, reliability, and clean separation between input handling, game logic, and rendering.
