# Adaptive Vehicle Dashboard Simulator

A dual-node automotive dashboard simulation system using two NXP LPC2129 ARM7 microcontrollers communicating over CAN Bus 2.0B at 125 kbps.

---

## System Overview

* **Node A (Sensor Hub):** Samples engine temperature (LM35 via ADC), vehicle speed (potentiometer via ADC), obstacle range (HC-SR04 via Timer1), and driver switches (external interrupts EINT0–EINT2). Packs data into an 8-byte CAN frame (`ID: 0x123`) and transmits periodically.
* **Node B (Instrument Cluster):** Receives frames via CAN Rx interrupt (VIC ISR), decodes metrics, blinks indicator LEDs (P0.19, P0.20, P0.21), and logs live vehicle metrics to a terminal over UART0 (9600 baud).

---

## Hardware Pinout

| Peripheral | Node A (Tx) | Node B (Rx) |
| :--- | :--- | :--- |
| **CAN1 Bus** | P0.24 (TD1), P0.25 (RD1) | P0.24 (TD1), P0.25 (RD1) |
| **UART0 Debug** | — | P0.0 (TXD0), P0.1 (RXD0) |
| **LM35 Temp** | P0.27 (AD0.0) | — |
| **Speed Pot** | P0.28 (AD0.1) | — |
| **Ultrasonic** | P0.17 (Trig), P0.18 (Echo) | — |
| **Switches** | P0.14 (Left), P0.15 (Right), P0.16 (Headlight) | — |
| **Status LEDs** | — | P0.19 (Left), P0.20 (Right), P0.21 (Headlight) |

---

## CAN Frame Layout (ID: 0x123 | DLC: 8 | 125 kbps)

* **ByteA (32-bit):**
  * `[9:0]` — Temperature (ADC raw)
  * `[19:10]` — Speed (ADC raw)
  * `[29:20]` — Proximity Distance (cm)
* **ByteB (32-bit):**
  * `Bit 19` — Left Indicator Flag
  * `Bit 20` — Right Indicator Flag
  * `Bit 21` — Headlight Flag

---

## Build & Flash

1. Open `nodeA/` and `nodeB/` projects in Keil µVision.
2. Build targets (`F7`) to compile `.hex` binaries.
3. Flash to respective LPC2129 boards via Flash Magic.
4. Connect bus transceivers (CAN_H to CAN_H, CAN_L to CAN_L).
5. Open serial terminal on Node B COM port at **9600-8-N-1** for live telemetry.