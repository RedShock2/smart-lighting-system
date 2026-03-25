// ============================================================
// main.cpp -- Smart Lighting System
// Version: 1.0.0 (Sprint 3 -- release final)
// Responsable: Luis Alejandro (Software Lead)
//
// Logica del sistema:
//  Hay luz  + Movimiento = LED OFF
//  Oscuro   + Sin mov    = LED OFF
//  Oscuro   + Movimiento = LED ON
//
// El LED se apaga automaticamente tras DELAY_OFF ms sin movimiento.
// ============================================================

#include <Arduino.h>
#include "config.h"
#include "sensors/pir.h"
#include "sensors/ldr.h"

// -- Variables de estado --
static bool          ledState      = false;
static unsigned long lastPrintTime = 0;

// -- Control del LED --
void updateLED() {
  bool estaOscuro    = ldrIsDark();
  bool hayMovimiento = pirHasRecentMotion();

  // Condicion principal: oscuro Y movimiento reciente
  bool debeEncenderse = estaOscuro && hayMovimiento;

  if (debeEncenderse != ledState) {
    ledState = debeEncenderse;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);

    if (ledState) {
      Serial.println(">> LED ON  -- oscuro + movimiento detectado");
    } else {
      Serial.println(">> LED OFF -- condicion no cumplida");
    }
  }
}

// -- Debug Serial --
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

// -- Banner de inicio --
void printBanner() {
  Serial.println();
  Serial.println("=====================================");
  Serial.print  ("  ");
  Serial.print  (PROJECT_NAME);
  Serial.println();
  Serial.print  ("  Firmware v");
  Serial.println(FW_VERSION);
  Serial.println("=====================================");
  Serial.println("  PIR -> GPIO 14");
  Serial.println("  LDR -> GPIO 34");
  Serial.println("  LED -> GPIO 26");
  Serial.println("=====================================");
  Serial.println("  AVISO: Espera 60s para PIR estable");
  Serial.println("=====================================");
  Serial.println();
}

// -- Setup --
void setup() {
  pirInit();
  ldrInit();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  delay(STARTUP_DELAY);

  printBanner();
  Serial.println("Sistema listo. Monitoreando...\n");
}

// -- Loop principal --
void loop() {
  pirUpdate();    // Actualiza timestamp de movimiento
  updateLED();    // Aplica logica y actualiza el LED si cambio
  printStatus();  // Imprime estado cada PRINT_INTERVAL ms

  delay(100);
}