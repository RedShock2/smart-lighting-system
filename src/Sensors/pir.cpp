// ============================================================
// sensors/pir.cpp — Módulo sensor PIR HC-SR501
// Versión: 0.2.0 (Sprint 2)
// Responsable: Luis Alejandro (Software Lead)
// ============================================================

#include "pir.h"
#include "../config.h"
#include <Arduino.h>

static unsigned long lastMotionTime = 0;

void pirInit() {
  pinMode(PIR_PIN, INPUT);
}

// Retorna true si se detectó movimiento en este ciclo
bool pirDetectMotion() {
  return digitalRead(PIR_PIN) == HIGH;
}

// Actualiza el timestamp del último movimiento detectado
void pirUpdate() {
  if (pirDetectMotion()) {
    lastMotionTime = millis();
  }
}

// Retorna true si hubo movimiento en los últimos DELAY_OFF ms
bool pirHasRecentMotion() {
  return (millis() - lastMotionTime) < DELAY_OFF;
}