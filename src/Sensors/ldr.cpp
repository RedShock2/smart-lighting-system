// ============================================================
// sensors/ldr.cpp — Módulo LDR LM393 (salida digital)
// Versión: 0.2.0 (Sprint 2)
// Responsable: Luis Alejandro (Software Lead)
// ============================================================

#include "ldr.h"
#include "../config.h"
#include <Arduino.h>

void ldrInit() {
  pinMode(LDR_PIN, INPUT);
}

// Retorna true si el ambiente está oscuro
// Nota: LDR_DARK_VALUE se define en config.h
// Si el LED no responde bien, cambiar HIGH por LOW en config.h
bool ldrIsDark() {
  return digitalRead(LDR_PIN) == LDR_DARK_VALUE;
}

// Retorna el valor raw del pin para debug
int ldrRaw() {
  return digitalRead(LDR_PIN);
}
