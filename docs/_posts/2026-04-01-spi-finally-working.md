---
layout: post
title: "SPI Finally Working"
date: 2026-04-01
permalink: /posts/spi-finally-working/
---

## Mid-Week Update — And No, This Is Not an April Fools Joke

This is the first mid-week update on this blog, and yes, I'm aware of the date. But I promise — this is real. **SPI is finally working with a bigger data size.**

---

## The Breakthrough: ESP-IDF Official SPI Example

On my way to my daytime job, I stumbled across the **official ESP-IDF example for SPI communication** between ESP32 boards. And let me tell you — it was an eye-opener.

Compared to what I had cobbled together, the official example brings **a lot more code** and, more importantly, a proper **handshake mechanism**. This handshake is what makes reliable, larger-than-64-byte SPI transfers actually work. It coordinates the master and slave so both sides know exactly when data is ready to be sent and received — no more silent truncation, no more garbage data.

The speed I'm seeing so far is looking great, and I'm excited about where this is heading.

---

## The Trade-Off: 6-Pin → 8-Pin Molex Connectors

Of course, good news always comes with a catch. The handshake requires additional signal lines, which means the current **6-pin Molex connectors are no longer enough**. We're moving to **8-pin Molex connectors**.

You know what that means — more soldering. All the existing connectors on the boards need to be redone. Not exactly how I wanted to spend my time, but if it means stable SPI communication, it's worth it.

---

## Next Steps

- **Friday:** Solder the new 8-pin Molex connectors onto the boards
- **Monday:** Have a working SPI connection up and running with the new setup
- Continue integrating the ESP-IDF SPI example into the actual firmware
