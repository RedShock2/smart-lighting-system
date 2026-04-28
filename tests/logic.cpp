#include "logic.h"
#include "mocks/arduino_mock.h"
#include <cstring>

// ── Global state ───────────────────────────────────────────────────────────
Estado estado;

// ── Color control ──────────────────────────────────────────────────────────
void setColor(uint8_t r, uint8_t g, uint8_t b) {
    ledcWrite(PIN_R, r * estado.brillo / 100);
    ledcWrite(PIN_G, g * estado.brillo / 100);
    ledcWrite(PIN_B, b * estado.brillo / 100);
    estado.r = r;
    estado.g = g;
    estado.b = b;
    estado.ledOn = (r || g || b);
}

void apagar() { setColor(0, 0, 0); }

// ── Auto mode ──────────────────────────────────────────────────────────────
void loopAuto() {
    unsigned long now = millis();
    if (now - estado.ultimaLectura < 200) return;
    estado.ultimaLectura = now;

    // LDR: LOW = bright (lit room), HIGH = dark (no ambient light)
    // The module's comparator inverts the signal, so digitalRead == LOW means "there IS light".
    bool oscuro         = digitalRead(PIN_LDR) == LOW;
    bool mov            = digitalRead(PIN_PIR) == HIGH;
    if (mov) estado.ultimoMovimiento = now;

    bool dentroDeTiempo = (now - estado.ultimoMovimiento < TIMEOUT_MS);

    // Turn on warm-white when the room is NOT bright (!oscuro) AND motion is recent.
    // "!oscuro" means LDR reads HIGH → ambient light is absent → it IS dark.
    if (!oscuro && dentroDeTiempo) {
        setColor(200, 140, 60);
    } else {
        apagar();
    }
}

// ── Fiesta mode ────────────────────────────────────────────────────────────
void calcFiestaColor(uint8_t paso, uint8_t& r, uint8_t& g, uint8_t& b) {
    if (paso < 85) {
        r = paso * 3;
        g = 255 - paso * 3;
        b = 0;
    } else if (paso < 170) {
        uint8_t p = paso - 85;
        r = 255 - p * 3;
        g = 0;
        b = p * 3;
    } else {
        uint8_t p = paso - 170;
        r = 0;
        g = p * 3;
        b = 255 - p * 3;
    }
}

void loopFiesta() {
    static uint8_t       paso = 0;
    static unsigned long t    = 0;
    if (millis() - t < 30) return;
    t = millis();

    uint8_t r, g, b;
    calcFiestaColor(paso, r, g, b);
    setColor(r, g, b);
    paso++;
}

// ── Handler helpers ────────────────────────────────────────────────────────
void applyColor(int r_in, int g_in, int b_in) {
    uint8_t r = static_cast<uint8_t>(constrain(r_in, 0, 255));
    uint8_t g = static_cast<uint8_t>(constrain(g_in, 0, 255));
    uint8_t b = static_cast<uint8_t>(constrain(b_in, 0, 255));
    estado.modo = OFF;
    setColor(r, g, b);
}

void applyBrillo(int v_in) {
    estado.brillo = static_cast<uint8_t>(constrain(v_in, 0, 100));
    setColor(estado.r, estado.g, estado.b);
}

void applyModo(const char* m) {
    if (strcmp(m, "auto")   == 0) { estado.modo = AUTO; }
    else if (strcmp(m, "fiesta") == 0) { estado.modo = FIESTA; }
    else if (strcmp(m, "off")    == 0) { estado.modo = OFF; apagar(); }
}
