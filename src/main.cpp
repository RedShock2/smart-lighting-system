#include <Arduino.h>
#include "config.h"

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  delay(1000);

  Serial.println("=== Smart Lighting System v0.1.0 ===");
  Serial.println("Sistema iniciado. Esperando sensores...");
}

void loop() {
  // Sprint 1: solo lectura básica de sensores para verificar conexiones
  int pir = digitalRead(PIR_PIN);
  int ldr = digitalRead(LDR_PIN);

  Serial.print("PIR: "); Serial.print(pir);
  Serial.print(" | LDR: "); Serial.println(ldr);

  // Lógica completa se implementa en Sprint 2
  delay(500);
}