// ============================================================
// main.cpp — Smart Lighting System
// Version: 0.2.0 (Sprint 2 — prototipo funcional)
// Responsable: Luis Alejandro (Software Lead)
//
// Logica del sistema:
//  Oscuro + Movimiento = LED ON
//  Cualquier otra combinacion = LED OFF
// ============================================================

#include <Arduino.h>
#include "config.h"
#include "sensors/pir.h"
#include "sensors/ldr.h"

static bool          ledState      = false;
static unsigned long lastPrintTime = 0;

void updateLED() {
  bool estaOscuro    = ldrIsDark();
  bool hayMovimiento = pirHasRecentMotion();
  bool debeEncenderse = estaOscuro && hayMovimiento;

  if (debeEncenderse != ledState) {
    ledState = debeEncenderse;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    Serial.println(ledState ? ">> LED ON" : ">> LED OFF");
  }
}

void printStatus() {
  unsigned long now = millis();
  if (now - lastPrintTime < PRINT_INTERVAL) return;
  lastPrintTime = now;

  Serial.print("PIR: ");
  Serial.print(pirHasRecentMotion() ? "MOVIMIENTO  " : "SIN MOV     ");
  Serial.print("| Luz: ");
  Serial.print(ldrIsDark() ? "OSCURO  " : "HAY LUZ ");
  Serial.print("| LED: ");
  Serial.println(ledState ? "ON" : "OFF");
}

void setup() {
  pirInit();
  ldrInit();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== Smart Lighting System v0.2.0 ===");
  Serial.println("Sistema listo. Monitoreando...");
}

void loop() {
  pirUpdate();
  updateLED();
  printStatus();
  delay(100);
}