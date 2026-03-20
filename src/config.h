#pragma once

// ============================================================
// config.h — Smart Lighting System
// Version: 0.2.0 (Sprint 2 — prototipo funcional)
// Responsable: Luis Alejandro (Software Lead)
// ============================================================

// --- Pines GPIO ---
#define PIR_PIN   14    // Sensor de movimiento HC-SR501 (OUT -> GPIO14)
#define LED_PIN   26    // LED actuador (anodo -> 220ohm -> GPIO26)
#define LDR_PIN   34    // Modulo LDR salida digital DO (GPIO34, input-only)

// --- Tiempos ---
#define DELAY_OFF       10000   // ms sin movimiento antes de apagar el LED
#define PRINT_INTERVAL  500     // ms entre impresiones del Serial Monitor

// --- Logica LDR ---
#define LDR_DARK_VALUE  HIGH