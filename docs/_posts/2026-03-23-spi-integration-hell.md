---
layout: post
title: "SPI Integration Hell"
date: 2026-03-23
permalink: /posts/spi-integration-hell/
---

## What Happened This Week

### Master-Gateway in Action

This week I finally brought the Master-Gateway PCB to life. A small PCB I quickly designed now sits between the PCBs and is meant to serve as the central coordinator. The initial connection to the **Center-Upper board** (Board-1) is already working – at least on the hardware side.

But then came the reality check: SPI integration.

### The SPI Chaos

Right now I'm fighting three stubborn problems:

**1. Completely wrong data arriving in the C# Simulator**
The bytes coming into the simulator have absolutely no relationship to what I'm sending. It looks like I'm transmitting garbage instead of data. First-level debugging is underway: timing analysis, signal verification, the usual suspects.

**2. Random reboots of the Slave board**
The slave side keeps rebooting randomly – sometimes after minutes, sometimes after a serial transmission from the master. This probably indicates either a hardware fault or a stack overflow in the firmware code. Or both.

**3. Only 1 byte instead of 242 is being transmitted**
This is perhaps the most frustrating problem: when an SPI transaction goes through at all, only 1 byte arrives – not the expected 242 bytes. This means my buffer handling or SPI configuration is fundamentally broken.

### The Cost of Impatience

To make the week even more interesting, I've lost my first **ESP32 controller**. The culprit? A wrongly oriented ESP32 on the headers of the Master-Gateway PCB. I placed it in the wrong orientation, and when I first powered up the board, the result was inevitable: 💥 The ESP32's voltage regulator became smoke.

That's an expensive reminder: always double-check the polarity before connecting anything.

### Next Steps

I'm continuously working on the SPI integration. The plan:
- Analyze signal logic
- Check buffer sizes and memory management
- First send an array of 242 bytes (all 0) to the C# simulator to test serial transmission
- Incrementally expand the firmware to transmit the data structure correctly

More on that in the next update – hopefully with better news.