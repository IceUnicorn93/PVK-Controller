---
layout: post
title: "SPI Working and Integrated, thanks to a 100 ohm Resistor"
date: 2026-04-06
permalink: /posts/spi-working-and-integrated/
---

Happy Easter everyone! 🐣 Today I bring amazing news — **we finally got SPI working between the Master Gateway and the first PCB!**

---

## The Last Hurdle: Breadboard vs. Real Hardware

On Wednesday I shared that the official ESP-IDF SPI example was working. But a frustrating problem continued to haunt us. On a breadboard, everything was fine — SPI communication worked flawlessly. But as soon as we connected the ESPs to the real hardware, the slave was unable to receive data. Strangely, the master received correct data from the slave without any issues. Only the slave side was broken.

---

## The Fix: A 100Ω Resistor on MOSI

A huge thanks goes to the user **hello_world** on the Discord server for not giving up on the idea of SPI. He suggested soldering a **100Ω resistor on the SCLK line**. Unfortunately, that didn't solve the issue.

But curiosity got the better of me — I went ahead and soldered the **100Ω resistor on the MOSI line** instead. And just like that, **it worked**. Crazy, right?

My best guess is that the signal on the MOSI line was ringing or overshooting due to the PCB traces and connectors, and the resistor acts as a simple dampener to clean it up. Whatever the exact reason — it works, and I'm not touching it.

---

## Current State

It took me a bit of time to get most of the kinks out of the system. The SPI communication and the firmware structure are actually combined — the Master Gateway is sending **real payload data** to the PCB. From the PCB side, I'm currently sending back **dummy data for debugging purposes**, just to confirm the return path works correctly. So far, so good.

<img src="../../images/2026-04-06/SPI Master and Slave Wired Up and Communicating.jpg" alt="SPI Master and Slave Wired Up and Communicating">

---

## Bonus: Custom 14-Segment Font

I also used the motivation from working SPI to finally create a **custom 14-segment font** for the displays. I followed [this YouTube tutorial](https://www.youtube.com/watch?v=WIAcy5FXuAA) for generating fonts for U8G2 displays. I had trouble creating a pixel-perfect font by hand, so instead I took a different approach:

1. A script that **generates all 255 ASCII characters as images**
2. A simple automation script to **import all images into Fony** to create the `.bdf` font file

This turned out to be way faster and more reliable than trying to draw each character manually.

<img src="../../images/2026-04-06/Custom Font for U8G2 Displays.jpg" alt="Custom Font for U8G2 Displays">

---

## Next Steps

- Implement real return data from Board 1 back to the Master Gateway
- Begin sending real display data to the physical displays
- Continue refining the custom font and test it on hardware
