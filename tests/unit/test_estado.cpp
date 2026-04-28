#include <gtest/gtest.h>
#include "logic.h"
#include "mocks/arduino_mock.h"

// Tests sobre el struct Estado: valores por defecto y transiciones de modo.

class EstadoTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_reset();
        estado = Estado{};
    }
};

// ── Valores por defecto ───────────────────────────────────────────────────

TEST_F(EstadoTest, ValoresPorDefecto_ModoEsAuto) {
    EXPECT_EQ(estado.modo, AUTO);
}

TEST_F(EstadoTest, ValoresPorDefecto_BrilloEs100) {
    EXPECT_EQ(estado.brillo, 100);
}

TEST_F(EstadoTest, ValoresPorDefecto_RGBSonCero) {
    EXPECT_EQ(estado.r, 0);
    EXPECT_EQ(estado.g, 0);
    EXPECT_EQ(estado.b, 0);
}

TEST_F(EstadoTest, ValoresPorDefecto_LedOnEsFalse) {
    EXPECT_FALSE(estado.ledOn);
}

TEST_F(EstadoTest, ValoresPorDefecto_TempYHumSonCero) {
    EXPECT_FLOAT_EQ(estado.temp, 0.0f);
    EXPECT_FLOAT_EQ(estado.hum,  0.0f);
}

TEST_F(EstadoTest, ValoresPorDefecto_TimestampsSonCero) {
    EXPECT_EQ(estado.ultimoMovimiento, 0UL);
    EXPECT_EQ(estado.ultimaLectura,    0UL);
    EXPECT_EQ(estado.ultimaDHT,        0UL);
}

// ── Transiciones de modo ──────────────────────────────────────────────────

TEST_F(EstadoTest, CambioModo_AutoAFiesta) {
    estado.modo = FIESTA;
    EXPECT_EQ(estado.modo, FIESTA);
}

TEST_F(EstadoTest, CambioModo_FiestaAOff) {
    estado.modo = FIESTA;
    estado.modo = OFF;
    EXPECT_EQ(estado.modo, OFF);
}

TEST_F(EstadoTest, CambioModo_OffDeVueltaAuto) {
    estado.modo = OFF;
    estado.modo = AUTO;
    EXPECT_EQ(estado.modo, AUTO);
}

// ── Coherencia entre setColor y Estado ───────────────────────────────────

TEST_F(EstadoTest, SetColor_MantieneCoherenciaLedOnYRGB) {
    setColor(255, 0, 0);
    EXPECT_TRUE(estado.ledOn);
    EXPECT_EQ(estado.r, 255);
    EXPECT_EQ(estado.g, 0);
    EXPECT_EQ(estado.b, 0);

    apagar();
    EXPECT_FALSE(estado.ledOn);
    EXPECT_EQ(estado.r, 0);
    EXPECT_EQ(estado.g, 0);
    EXPECT_EQ(estado.b, 0);
}

TEST_F(EstadoTest, BrilloNoAfectaValoresRGBGuardados) {
    // brillo sólo escala la señal PWM; estado.r/g/b guardan el valor lógico sin escalar.
    estado.brillo = 50;
    setColor(200, 100, 50);
    EXPECT_EQ(estado.r, 200);  // valor original, no escalado
    EXPECT_EQ(estado.g, 100);
    EXPECT_EQ(estado.b,  50);
    // Pin PWM sí está escalado
    EXPECT_EQ(mock_ledc_get(PIN_R), 100u);
    EXPECT_EQ(mock_ledc_get(PIN_G),  50u);
    EXPECT_EQ(mock_ledc_get(PIN_B),  25u);
}
