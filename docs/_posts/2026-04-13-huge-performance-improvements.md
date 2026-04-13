---
layout: post
title: "Huge Performance Improvements with SPI Communication"
date: 2026-04-13
permalink: /posts/huge-performance-improvements-with-spi-communication/
---

A week ago, hitting 60 Hz refresh rates for 6 PCBs felt like a pipe dream. With a single package taking around **15 ms** to transmit from the PC to the Master Gateway, the math simply didn't work out. This week, that changed dramatically.

---

## The Bottleneck: C# Struct Marshalling

After some digging, I finally found the culprit — **marshalling**. In C#, converting a struct into a byte array using `Marshal` is convenient, but it turns out to be surprisingly expensive. This marshalling was happening inside the sending loop, meaning every single package was being serialized on the fly. At 15 ms per package, there was no way to hit our target refresh rate.

---

## The Fix: Smaller Packages and Separate Package Creation

Two changes made the difference. First, we **reduced the package size from 240 bytes down to 60 bytes**. Smaller packages mean less overhead per transfer and significantly faster processing on both sides.

Second, I **moved the package creation out of the sending loop**. Instead of marshalling each struct right before sending, I now prepare all byte arrays in a simple loop beforehand. For the final version, this will most likely become a dedicated task that creates the packages in the background.

Once I made that change, the results spoke for themselves — **1.2 ms per package**. That's over 10x faster than before.

<img src="../../images/2026-04-13/image.png" alt="Transmission time of 1.2ms per package">

---

## A New Problem: Sending Too Fast

Of course, with great speed comes great instability. Sending data as fast as possible without any delay causes the Master Gateway to **stop responding after 7 to 8 LEDs**. Sometimes it's 7, sometimes 8 — it's not entirely consistent, but the result is always the same: the ESP becomes unresponsive and the **USB cable has to be unplugged and plugged back in** to recover.

My current workaround is a simple **1 ms delay** in the sending loop. That's enough to keep the Master Gateway happy, and honestly — it's still plenty fast if you ask me.

---

## Next Steps

- Investigate the root cause of the Master Gateway locking up under full-speed transmission
- Begin testing with multiple PCBs to validate the 60 Hz target
- Move package creation into a proper background task for the final version