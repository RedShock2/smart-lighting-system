#pragma once
 
// ============================================================
// config.h — Smart Lighting System
// Versión: 0.1.0 (Sprint 1 — estructura base)
// Responsable: Luis Alejandro (Software Lead)
// ============================================================
 
// --- Pines GPIO ---
#define PIR_PIN   14    // Sensor de movimiento HC-SR501 (OUT → GPIO14)
#define LED_PIN   26    // LED actuador (ánodo → 220Ω → GPIO26)
#define LDR_PIN   34    // Módulo LDR salida digital DO (GPIO34, input-only)
 
// --- Tiempos ---
#define DELAY_OFF       10000   // ms sin movimiento antes de apagar el LED