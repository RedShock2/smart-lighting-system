#pragma once
#include <cstdint>
#include <cmath>

// ── Pin definitions (mirrors src/smart_lighting_1.0) ──────────────────────
#define PIN_R    25
#define PIN_G    26
#define PIN_B    27
#define PIN_PIR  14
#define PIN_LDR  35

#define TIMEOUT_MS 10000UL

// ── System state ───────────────────────────────────────────────────────────
enum Modo { AUTO, FIESTA, OFF };

struct Estado {
    Modo          modo             = AUTO;
    uint8_t       r = 0, g = 0, b = 0;
    uint8_t       brillo           = 100;
    bool          ledOn            = false;
    float         temp             = 0.0f;
    float         hum              = 0.0f;
    unsigned long ultimoMovimiento = 0;
    unsigned long ultimaLectura    = 0;
    unsigned long ultimaDHT        = 0;
};

extern Estado estado;

// ── Core functions ─────────────────────────────────────────────────────────
void setColor(uint8_t r, uint8_t g, uint8_t b);
void apagar();
void loopAuto();
void loopFiesta();

// Pure color formula used inside loopFiesta, exposed for direct testing.
void calcFiestaColor(uint8_t paso, uint8_t& r, uint8_t& g, uint8_t& b);

// ── Handler logic helpers (parameter parsing, no WebServer dependency) ─────
// These replicate exactly what handleColor / handleBrillo / handleModo do
// after the WebServer extracts the argument strings.
void applyColor(int r_in, int g_in, int b_in);   // handleColor body
void applyBrillo(int v_in);                        // handleBrillo body
void applyModo(const char* m);                     // handleModo body
