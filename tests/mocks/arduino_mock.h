#pragma once
#include <cstdint>
#include <cstring>

// ── Arduino constants ──────────────────────────────────────────────────────
#define HIGH   1
#define LOW    0
#define INPUT  0
#define OUTPUT 1

#define constrain(amt, lo, hi) ((amt) < (lo) ? (lo) : ((amt) > (hi) ? (hi) : (amt)))

// ── Mock control API ───────────────────────────────────────────────────────
// Call mock_reset() in each test's SetUp() to start with a clean slate.

void mock_reset();
uint32_t      mock_ledc_get(uint8_t pin);
void          mock_digital_set(uint8_t pin, int value);
void          mock_millis_set(unsigned long ms);
void          mock_millis_advance(unsigned long ms);

// ── Arduino / ESP32 hardware API (implemented in arduino_mock.cpp) ─────────
void          ledcWrite(uint8_t pin, uint32_t value);
void          ledcAttach(uint8_t pin, uint32_t freq, uint8_t bits);
int           digitalRead(uint8_t pin);
void          pinMode(uint8_t pin, uint8_t mode);
unsigned long millis();
