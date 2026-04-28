#include <gtest/gtest.h>
#include "logic.h"
#include "mocks/arduino_mock.h"

class ColorTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_reset();
        estado = Estado{};
    }
};

// ── setColor: escritura en pines PWM ──────────────────────────────────────

TEST_F(ColorTest, SetColor_BrilloCompleto_EscribeValoresDirectos) {
    estado.brillo = 100;
    setColor(200, 140, 60);
    EXPECT_EQ(mock_ledc_get(PIN_R), 200u);
    EXPECT_EQ(mock_ledc_get(PIN_G), 140u);
    EXPECT_EQ(mock_ledc_get(PIN_B),  60u);
}

TEST_F(ColorTest, SetColor_Brillo50_EscribeValoresAMitad) {
    estado.brillo = 50;
    setColor(200, 140, 60);
    EXPECT_EQ(mock_ledc_get(PIN_R), 100u);
    EXPECT_EQ(mock_ledc_get(PIN_G),  70u);
    EXPECT_EQ(mock_ledc_get(PIN_B),  30u);
}

TEST_F(ColorTest, SetColor_BrilloCero_ApagaTodosLosCanales) {
    estado.brillo = 0;
    setColor(255, 255, 255);
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
    EXPECT_EQ(mock_ledc_get(PIN_G), 0u);
    EXPECT_EQ(mock_ledc_get(PIN_B), 0u);
}

TEST_F(ColorTest, SetColor_ActualizaRGBEnEstado) {
    setColor(10, 20, 30);
    EXPECT_EQ(estado.r, 10);
    EXPECT_EQ(estado.g, 20);
    EXPECT_EQ(estado.b, 30);
}

// ── setColor: ledOn ────────────────────────────────────────────────────────

TEST_F(ColorTest, SetColor_CualquierCanalDistintoCero_PoneLedOnTrue) {
    setColor(1, 0, 0);
    EXPECT_TRUE(estado.ledOn);

    setColor(0, 1, 0);
    EXPECT_TRUE(estado.ledOn);

    setColor(0, 0, 1);
    EXPECT_TRUE(estado.ledOn);
}

TEST_F(ColorTest, SetColor_TodosCerosPonenLedOnFalse) {
    estado.ledOn = true;
    setColor(0, 0, 0);
    EXPECT_FALSE(estado.ledOn);
}

// ── apagar ─────────────────────────────────────────────────────────────────

TEST_F(ColorTest, Apagar_ApagaTodosLosCanalesPWM) {
    estado.brillo = 100;
    setColor(255, 128, 64);
    apagar();
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
    EXPECT_EQ(mock_ledc_get(PIN_G), 0u);
    EXPECT_EQ(mock_ledc_get(PIN_B), 0u);
}

TEST_F(ColorTest, Apagar_PoneLedOnFalse) {
    setColor(100, 100, 100);
    ASSERT_TRUE(estado.ledOn);
    apagar();
    EXPECT_FALSE(estado.ledOn);
}

TEST_F(ColorTest, Apagar_ActualizaRGBACero) {
    setColor(200, 150, 80);
    apagar();
    EXPECT_EQ(estado.r, 0);
    EXPECT_EQ(estado.g, 0);
    EXPECT_EQ(estado.b, 0);
}
