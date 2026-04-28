#include "arduino_mock.h"

static uint32_t      ledc_vals[40]    = {};
static int           digital_vals[40] = {};
static unsigned long mock_time        = 0;

void mock_reset() {
    memset(ledc_vals,    0, sizeof(ledc_vals));
    memset(digital_vals, 0, sizeof(digital_vals));
    mock_time = 0;
}

uint32_t mock_ledc_get(uint8_t pin)       { return ledc_vals[pin]; }
void     mock_digital_set(uint8_t pin, int v) { digital_vals[pin] = v; }
void     mock_millis_set(unsigned long ms)    { mock_time = ms; }
void     mock_millis_advance(unsigned long ms){ mock_time += ms; }

void          ledcWrite(uint8_t pin, uint32_t v)             { ledc_vals[pin] = v; }
void          ledcAttach(uint8_t, uint32_t, uint8_t)         {}
int           digitalRead(uint8_t pin)                        { return digital_vals[pin]; }
void          pinMode(uint8_t, uint8_t)                       {}
unsigned long millis()                                         { return mock_time; }
