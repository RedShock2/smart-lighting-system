#include <gtest/gtest.h>
#include "logic.h"
#include "mocks/arduino_mock.h"

// Tests para la lógica de los HTTP handlers (sin dependencia de WebServer).
// applyColor / applyBrillo / applyModo replican exactamente el cuerpo de
// handleColor / handleBrillo / handleModo del sketch v1.0.

class HandlersTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_reset();
        estado = Estado{};
    }
};

// ── applyColor (handleColor) ──────────────────────────────────────────────

TEST_F(HandlersTest, ApplyColor_ValoresNormales_ActualizaEstadoYLED) {
    applyColor(100, 150, 200);
    EXPECT_EQ(estado.r, 100);
    EXPECT_EQ(estado.g, 150);
    EXPECT_EQ(estado.b, 200);
    EXPECT_TRUE(estado.ledOn);
}

TEST_F(HandlersTest, ApplyColor_CambiaModoAOff) {
    estado.modo = AUTO;
    applyColor(100, 100, 100);
    EXPECT_EQ(estado.modo, OFF);
}

TEST_F(HandlersTest, ApplyColor_RojoFueraDeRango_SeClampea255) {
    applyColor(300, 0, 0);
    EXPECT_EQ(estado.r, 255);
}

TEST_F(HandlersTest, ApplyColor_ValorNegativo_SeClampea0) {
    applyColor(-10, 0, 0);
    EXPECT_EQ(estado.r, 0);
}

TEST_F(HandlersTest, ApplyColor_TodosCerosApagaLED) {
    estado.ledOn = true;
    applyColor(0, 0, 0);
    EXPECT_FALSE(estado.ledOn);
}

// ── applyBrillo (handleBrillo) ────────────────────────────────────────────

TEST_F(HandlersTest, ApplyBrillo_ValorNormal_ActualizaBrillo) {
    applyBrillo(75);
    EXPECT_EQ(estado.brillo, 75);
}

TEST_F(HandlersTest, ApplyBrillo_MayorA100_SeClampea100) {
    applyBrillo(150);
    EXPECT_EQ(estado.brillo, 100);
}

TEST_F(HandlersTest, ApplyBrillo_Negativo_SeClampea0) {
    applyBrillo(-5);
    EXPECT_EQ(estado.brillo, 0);
}

TEST_F(HandlersTest, ApplyBrillo_ReaplicaColorActual) {
    // Fijar un color, luego cambiar el brillo y verificar los pines PWM.
    estado.r = 200; estado.g = 100; estado.b = 50;
    applyBrillo(50);
    EXPECT_EQ(mock_ledc_get(PIN_R), 100u);  // 200 * 50/100
    EXPECT_EQ(mock_ledc_get(PIN_G),  50u);  // 100 * 50/100
    EXPECT_EQ(mock_ledc_get(PIN_B),  25u);  //  50 * 50/100
}

TEST_F(HandlersTest, ApplyBrillo_Cero_ApagaTodosLosPines) {
    estado.r = 255; estado.g = 255; estado.b = 255;
    applyBrillo(0);
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
    EXPECT_EQ(mock_ledc_get(PIN_G), 0u);
    EXPECT_EQ(mock_ledc_get(PIN_B), 0u);
}

// ── applyModo (handleModo) ────────────────────────────────────────────────

TEST_F(HandlersTest, ApplyModo_Auto_CambiaModoAuto) {
    estado.modo = OFF;
    applyModo("auto");
    EXPECT_EQ(estado.modo, AUTO);
}

TEST_F(HandlersTest, ApplyModo_Fiesta_CambiaModoFiesta) {
    applyModo("fiesta");
    EXPECT_EQ(estado.modo, FIESTA);
}

TEST_F(HandlersTest, ApplyModo_Off_CambiaModoOffYApaga) {
    setColor(200, 140, 60);
    ASSERT_TRUE(estado.ledOn);
    applyModo("off");
    EXPECT_EQ(estado.modo, OFF);
    EXPECT_FALSE(estado.ledOn);
    EXPECT_EQ(mock_ledc_get(PIN_R), 0u);
}

TEST_F(HandlersTest, ApplyModo_StringDesconocida_NoModificaModo) {
    estado.modo = AUTO;
    applyModo("invalido");
    EXPECT_EQ(estado.modo, AUTO);
}
