#pragma once

// ============================================================
// config.h — Smart Lighting System
// Versión: 1.0.0 (Sprint 3 — release final)
// Responsable: Luis Alejandro (Software Lead)
// ============================================================

// --- Pines GPIO ---
#define PIR_PIN   14    // Sensor de movimiento HC-SR501 (OUT → GPIO14)
#define LED_PIN   26    // LED actuador (ánodo → 220Ω → GPIO26)
#define LDR_PIN   34    // Módulo LDR salida digital DO (GPIO34, input-only)

// --- Tiempos ---
#define DELAY_OFF       10000   // ms sin movimiento antes de apagar el LED
#define PRINT_INTERVAL  500     // ms entre impresiones del Serial Monitor
#define STARTUP_DELAY   2000    // ms de espera al iniciar (estabilización)

// --- Lógica LDR ---
// Nuestro módulo LM393: HIGH = oscuro, LOW = hay luz
// Si el LED no responde al oscurecer, cambiar HIGH por LOW aquí
#define LDR_DARK_VALUE  HIGH

// --- Versión del firmware ---
#define FW_VERSION   "1.0.0"
#define PROJECT_NAME "Smart Lighting System"
