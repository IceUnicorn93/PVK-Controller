---
layout: post
title: "Target Display"
date: 2026-03-16
permalink: /posts/target-dis/
---

## 🎯 This Week: Target Display Board

This week was all about **Board 2 — the Target Display**. The goal: drive 20 individual segment displays from a single ESP32, reliably and fast. After roughly **7 hours of hands-on work** — soldering, cutting headers, preparing the PCB — the board is fully operational.

---

## 🔧 Hardware Setup

The board centers around one challenge: an ESP32 only has two hardware I2C buses, but we need to address 20 displays. The solution is **3× I2C multiplexers**, which let us fan out the single I2C bus to all 20 displays without any address conflicts.

**Component overview:**
- 20× Segment displays
- 1× ESP32 microcontroller
- 3× I2C multiplexers

The bulk of the build time went into the physical assembly — cutting pin headers to size, soldering all connections cleanly, and making sure every display was seated correctly.

<img src="../../images/2026-03-16/IMG_20260314_093007.webp" alt="Early board assembly" width="502">
<img src="../../images/2026-03-16/IMG_20260314_144837.webp" alt="Board mid-assembly" width="502">
<img src="../../images/2026-03-16/IMG_20260314_151640.webp" alt="Board nearly complete" width="502">

---

## ⚡ Performance

Once assembled, the board was brought up and measured:

| Metric | Result |
|---|---|
| Startup time | **428 ms** |
| Displays addressable | **20 / 20** |
| Protocol | Shared `protocol.h` |

All 20 displays can be written to individually, and the board correctly speaks the shared packet protocol defined in `protocol.h` — the same protocol used across all boards in the system.

<img src="../../images/2026-03-16/IMG_20260315_211915.webp" alt="Finished and running board" width="502">

---

## ✅ Current Status

Board 2 is **ready for integration**. The firmware handles display addressing through the multiplexers transparently, startup completes in well under half a second, and the board is ready to receive packets from the Master Gateway over SPI.