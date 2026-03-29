---
layout: post
title: "Small SPI Progress"
date: 2026-03-29
permalink: /posts/small-spi-progress/
---

## What Happened This Week

### The Root Cause: 64-Byte SPI Limit

After last week's SPI integration hell, I finally tracked down the core problem — and it wasn't my code, my wiring, or my sanity. It was the **SPI Slave library** I was using on the slave PCB. Turns out, it has a hard limit of **64 bytes per transfer**.

That explains why my 242-byte structs were arriving as garbage. The library was silently truncating everything beyond 64 bytes without any warning or error. Once I identified this, a lot of last week's mysterious behavior suddenly made sense.

---

### A Working SPI Test

With the root cause identified, I built a **simple SPI transmission example** from scratch to validate the fix. The good news: it works. Data goes from master to slave and arrives correctly on the other side.

However, integrating this back into the actual firmware hasn't gone smoothly yet. I tested around with folding the working example into the real codebase, but didn't get very far. There's clearly more work needed to bridge the gap between "simple test works" and "full integration works." More details on that struggle in the next update.

---

### Side Project: PCB Holder

Outside of firmware work, I used some free time this week to start building a **holder for the PCBs**. Right now all the boards are just loosely sitting on my desk, sliding around and getting in each other's way — not exactly ideal when you're probing signals and debugging SPI.

The holder isn't finished yet, but it should make the workspace a lot more manageable once it's done.

---

### Next Steps

- Continue working on the full SPI integration with the real firmware
- Finish the PCB holder
- Hopefully have a stable master-to-slave data pipeline by next week