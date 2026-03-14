---
layout: post
title: "Communication Update"
date: 2026-03-08
permalink: /posts/communication-update/
---

## 📡 This Week: Communication Architecture

This week we focused on one of the most fundamental aspects of the project — how the different parts of the system talk to each other. With multiple boards, a master controller, and a PC game all needing to exchange data in real time, getting the communication right is critical.

---

## 🔌 Communication Architecture

The system is built around two communication layers:

### Game ↔ Master Gateway — Serial
The PC game (PVKK) communicates with the **Master Gateway** over a **Serial connection**. This keeps the interface simple and reliable, and allows the game to send display/state data to the console and receive button/sensor input back.

### Master Gateway ↔ All Boards — SPI
The Master Gateway acts as the SPI master and talks to all controller boards as SPI slaves. SPI was chosen for its speed and simplicity, making it well suited for the short-distance, time-sensitive communication between boards inside the console enclosure.

```
[ PC Game ]
     |
  (Serial)
     |
[ Master Gateway ]  ←── SPI ──→  [ Board 1 ]
                    ←── SPI ──→  [ Board 2 ]
                    ←── SPI ──→  [ Board 3 ]
                    ←── SPI ──→  [ ... ]
```
<img src="../../images/2026-03-08/ConnectionDiagram.png" alt="Connection Diagram" width="502" height="115">
<img src="../../images/2026-03-08/WorkingSPICommunication.png" alt="Working SPI Communication" width="482" height="151">

---

## 🧪 Hello World Prototype

To validate the SPI setup, a small prototype was built using **two ESP32 microcontrollers** — one acting as the SPI master and one as the SPI slave. The prototype successfully demonstrated a basic data exchange between the two devices, confirming that the chosen approach works and will scale to the full set of boards.
<img src="../../images/2026-03-08/IMG_20260307_130307.jpg" alt="Prototype" width="502" height="282">
    
---

## 📄 Shared Protocol File

To ensure all boards speak the same language, a shared **`protocol.h`** header file was created. This file defines the exact binary packet structures used in every SPI transaction, so there is no ambiguity about the data format regardless of which board is involved.

Every packet sent from the Master Gateway to a board follows this structure:

```cpp
typedef struct __attribute__((packed)) {
    uint8_t boardId;            // Target board (1, 2, 3, ...)
    uint8_t checksum;           // XOR checksum for validation
    uint8_t payload[250];       // The actual data
} SerialPacket;
```

Each board also sends back an **answer packet** (up to 30 bytes) containing its current input state — button presses, dial positions, and so on.

---

## 🗂️ Board Protocols — Boards 1–3 Defined

Payload and answer structures are now fully defined for the first three boards:

### Board 1 — Upper Center
The most data-heavy board. The payload covers all LEDs, display values, and brightness. The answer reports button states for the ammo, priming, targeting, and chamber controls.

| Direction | Size | Contents |
|-----------|------|----------|
| Payload   | 143 bytes | 137 LED states, brightness, 5 display values |
| Answer    | 10 bytes  | Ammo selection, load/prime/target/chamber buttons |

### Board 2 — Upper Right (Target List)
Receives data for up to **4 simultaneous targets**, each described by identification, altitude, estimated time of arrival, approach angle, and speed.

| Direction | Size | Contents |
|-----------|------|----------|
| Payload   | 60 bytes | 4× target data (ID, altitude, ETA, angle, speed) |
| Answer    | — | No inputs on this board |

### Board 3 — Aiming Calculator
The most input-heavy board. It hosts the aiming calculator keypad, angle controls, arming switches, and the fire button.

| Direction | Size | Contents |
|-----------|------|----------|
| Payload   | ~53 bytes | Target data, countdown, ideal angle, energy, sequence assistant |
| Answer    | 23 bytes  | Calculator buttons, angle deltas, lock, arm switches, fire |

Boards 4–6 are stubbed out in the protocol file with spare bytes, ready to be filled in as their hardware designs are finalised.

---

## ⏭️ What's Next?

With the communication backbone in place, the next steps are:
- Expanding the SPI prototype to include real board hardware
- Integrating the Master Gateway logic with the PVKK Simulator
- Finalising the hardware designs for boards 2 and 3

---

*Posted by the PVK Console Team on March 8, 2026*