#include <gtest/gtest.h>
#include "logic.h"
#include "mocks/arduino_mock.h"

// Helpers para legibilidad
static void simularOscuro()   { mock_digital_set(PIN_LDR, HIGH); } // LDR HIGH → sin luz ambiente → oscuro
static void simularConLuz()   { mock_digital_set(PIN_LDR, LOW);  } // LDR LOW  → hay luz  → NO oscuro
static void simularMovimiento(){ mock_digital_set(PIN_PIR, HIGH); }
static void simularSinMovimiento(){ mock_digital_set(PIN_PIR, LOW); }

class AutoModeTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_reset();
        estado = Estado{};
        // Arrancamos en t=5000 para que la primera llamada siempre supere la
        // ventana de 200 ms (ultimaLectura = 0 por defecto).
        mock_millis_set(5000);
    }
};

// ── Ventana de throttling (200 ms) ────────────────────────────────────────

TEST_F(AutoModeTest, PrimeraLlamada_Ejecuta_ActualizaUltimaLectura) {
    simularOscuro();
    simularMovimiento();
    loopAuto();
    EXPECT_EQ(estado.ultimaLectura, 5000UL);
}

TEST_F(AutoModeTest, SegundaLlamadaDentroDeVentana_NoEjecuta) {
    simularConLuz();
    simularSinMovimiento();
    loopAuto();                          // ejecuta, apaga

    mock_millis_advance(100);            // sólo 100 ms después
    simularOscuro();
    simularMovimiento();
    uint32_t r_antes = mock_ledc_get(PIN_R);
    loopAuto();                          // debe omitirse
    EXPECT_EQ(mock_ledc_get(PIN_R), r_antes);  // LED no cambia
}

TEST_F(AutoModeTest, LlamadaFueraDeVentana_Ejecuta) {
    loopAuto();
    mock_millis_advance(200);            // exactamente 200 ms → debe ejecutar
    simularOscuro();
    simularMovimiento();
    loopAuto();
    EXPECT_EQ(estado.ultimaLectura, 5200UL);
}

// ── Lógica de encendido ───────────────────────────────────────────────────

TEST_F(AutoModeTest, OscuroConMovimientoReciente_EnciendeBlancoCálido) {
    // Registramos movimiento reciente manualmente
    estado.ultimoMovimiento = 4000;      // hace 1000 ms (< TIMEOUT_MS)
    simularOscuro();
    simularSinMovimiento();              // el timeout de estado es lo que importa
    loopAuto();
    EXPECT_EQ(mock_ledc_get(PIN_R), 200u);
    EXPECT_EQ(mock_ledc_get(PIN_G), 140u);
    EXPECT_EQ(mock_ledc_get(PIN_B),  60u);
    EXPECT_TRUE(estado.ledOn);
}

TEST_F(AutoModeTest, OscuroSinMovimientoPrevio_Apaga) {
    // Para que el timeout (10 000 ms) esté vencido, now debe superar TIMEOUT_MS.
    // SetUp fija millis=5000; aquí lo adelantamos a 15 000 ms.
    mock_millis_set(15000);
    estado.ultimaLectura    = 0;        // garantiza ejecución (15000-0 > 200)
    estado.ultimoMovimiento = 0;        // 15000-0 = 15000 > TIMEOUT_MS → timeout expirado
    simularOscuro();
    simularSinMovimiento();
    loopAuto();
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
    EXPECT_FALSE(estado.ledOn);
}

TEST_F(AutoModeTest, ConLuzAmbiental_Apaga_Independientemente_DelMovimiento) {
    estado.ultimoMovimiento = 4900;     // movimiento muy reciente
    simularConLuz();                    // LDR LOW → hay luz → oscuro=false → !oscuro=false
    simularMovimiento();
    loopAuto();
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
    EXPECT_FALSE(estado.ledOn);
}

TEST_F(AutoModeTest, TimeoutAgotado_AunqueOscuro_Apaga) {
    // ultimoMovimiento=0, ahora=5000 → 5000-0=5000 < TIMEOUT(10000)? Sí aún dentro.
    // Ponemos un tiempo mayor al timeout.
    mock_millis_set(15000);
    estado.ultimaLectura    = 0;
    estado.ultimoMovimiento = 0;        // hace 15000 ms → timeout expirado
    simularOscuro();
    simularSinMovimiento();
    loopAuto();
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
    EXPECT_FALSE(estado.ledOn);
}

// ── Detección de movimiento actualiza timestamp ───────────────────────────

TEST_F(AutoModeTest, PIRActivo_ActualizaUltimoMovimiento) {
    simularOscuro();
    simularMovimiento();
    loopAuto();
    EXPECT_EQ(estado.ultimoMovimiento, 5000UL);
}

TEST_F(AutoModeTest, PIRInactivo_NoActualizaUltimoMovimiento) {
    estado.ultimoMovimiento = 1234;
    simularOscuro();
    simularSinMovimiento();
    loopAuto();
    EXPECT_EQ(estado.ultimoMovimiento, 1234UL);
}

// ── El brillo afecta el blanco cálido ─────────────────────────────────────

TEST_F(AutoModeTest, BrilloAlMitad_EscalaBlancoCálido) {
    estado.brillo           = 50;
    estado.ultimoMovimiento = 4000;
    simularOscuro();
    simularSinMovimiento();
    loopAuto();
    EXPECT_EQ(mock_ledc_get(PIN_R), 100u);  // 200 * 50/100
    EXPECT_EQ(mock_ledc_get(PIN_G),  70u);  // 140 * 50/100
    EXPECT_EQ(mock_ledc_get(PIN_B),  30u);  //  60 * 50/100
}
