---
layout: post
title: "Small SPI Update"
date: 2026-04-20
permalink: /posts/small-spi-update/
---

Short update this week — didn't have as much time as I would have liked, but still made some meaningful progress.

---

## Multi-Slave SPI Master

The main achievement this week was updating the **SPI Master code** to support **multiple SPI slaves** on the same bus. Previously, the Master Gateway could only talk to a single slave at a time. Now it manages multiple devices, each with their own CS pin, handshake pin, semaphore, and send/receive buffers.

The approach uses the ESP-IDF SPI master driver with `spi_bus_add_device()` for each slave, and a per-slave handshake ISR to signal when a slave is ready for the next transaction. This keeps things clean and lets us address each slave independently.

---

## A Small Roadblock: 3-Slave Limit

Unfortunately, I ran into a limitation — the current code only supports **up to 3 slaves**. Beyond that, things stop working. I haven't fully diagnosed the root cause yet, but it's likely related to how the ESP-IDF SPI master driver manages its internal resources or how the GPIO ISR handles the handshake lines at scale.

The good news? It's a software problem, not a hardware one. So it should be solvable — worst case, with a bit of help from AI to dig through the ESP-IDF internals.

---

## Testing With Last Week's Code

I tested last week's integrated communication chain with the new multi-slave SPI Master code, and everything looks good. The performance improvements from the previous week still hold up, and the system remains stable. Happy to move forward with this approach.

---

## Next Steps

- Investigate the 3-slave limit and find a way to support more devices
- Begin wiring and testing with additional physical PCBs
- Continue integrating the full data pipeline from PC to all boards