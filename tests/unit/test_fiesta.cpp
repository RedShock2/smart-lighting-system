#include <gtest/gtest.h>
#include "logic.h"
#include "mocks/arduino_mock.h"

// Nota: loopFiesta() usa variables estáticas internas (paso, t) que persisten
// entre tests del mismo proceso. Por eso sus pruebas de timing se realizan
// sobre calcFiestaColor() — la fórmula pura extraída — que no tiene estado
// estático y es completamente predecible.

class FiestaColorFormulaTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_reset();
        estado = Estado{};
    }
};

// ── Fase 1: paso 0–84  →  R sube, G baja, B = 0 ─────────────────────────

TEST_F(FiestaColorFormulaTest, Fase1_Paso0_VerdeMaximoRojoYAzulCero) {
    uint8_t r, g, b;
    calcFiestaColor(0, r, g, b);
    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, 255);
    EXPECT_EQ(b, 0);
}

TEST_F(FiestaColorFormulaTest, Fase1_Paso84_RojoCasiMaximoVerdeCasiCero) {
    uint8_t r, g, b;
    calcFiestaColor(84, r, g, b);
    EXPECT_EQ(r, 252);   // 84 * 3
    EXPECT_EQ(g,   3);   // 255 - 84 * 3
    EXPECT_EQ(b,   0);
}

TEST_F(FiestaColorFormulaTest, Fase1_AzulSiempreCeroEnTodosLosPasos) {
    for (uint8_t p = 0; p < 85; ++p) {
        uint8_t r, g, b;
        calcFiestaColor(p, r, g, b);
        EXPECT_EQ(b, 0) << "b debe ser 0 en paso " << (int)p;
    }
}

TEST_F(FiestaColorFormulaTest, Fase1_RojoSubeConPaso) {
    uint8_t r1, g1, b1, r2, g2, b2;
    calcFiestaColor(10, r1, g1, b1);
    calcFiestaColor(20, r2, g2, b2);
    EXPECT_GT(r2, r1);
    EXPECT_LT(g2, g1);
}

TEST_F(FiestaColorFormulaTest, Fase1_FormulasExactas) {
    uint8_t r, g, b;
    calcFiestaColor(42, r, g, b);
    EXPECT_EQ(r, 42 * 3);
    EXPECT_EQ(g, 255 - 42 * 3);
    EXPECT_EQ(b, 0);
}

// ── Fase 2: paso 85–169  →  R baja, G = 0, B sube ────────────────────────

TEST_F(FiestaColorFormulaTest, Fase2_Paso85_RojoMaximoVerdeYAzulCero) {
    uint8_t r, g, b;
    calcFiestaColor(85, r, g, b);
    EXPECT_EQ(r, 255);
    EXPECT_EQ(g, 0);
    EXPECT_EQ(b, 0);
}

TEST_F(FiestaColorFormulaTest, Fase2_Paso169_RojoCasiCeroAzulCasiMaximo) {
    uint8_t r, g, b;
    calcFiestaColor(169, r, g, b);
    uint8_t p = 169 - 85;            // 84
    EXPECT_EQ(r, static_cast<uint8_t>(255 - p * 3));  // 3
    EXPECT_EQ(g, 0);
    EXPECT_EQ(b, static_cast<uint8_t>(p * 3));         // 252
}

TEST_F(FiestaColorFormulaTest, Fase2_VerdeSiempreCeroEnTodosLosPasos) {
    for (uint8_t p = 85; p < 170; ++p) {
        uint8_t r, g, b;
        calcFiestaColor(p, r, g, b);
        EXPECT_EQ(g, 0) << "g debe ser 0 en paso " << (int)p;
    }
}

TEST_F(FiestaColorFormulaTest, Fase2_FormulasExactas) {
    uint8_t r, g, b;
    uint8_t paso = 120;
    uint8_t p    = paso - 85;         // 35
    calcFiestaColor(paso, r, g, b);
    EXPECT_EQ(r, static_cast<uint8_t>(255 - p * 3));
    EXPECT_EQ(g, 0);
    EXPECT_EQ(b, static_cast<uint8_t>(p * 3));
}

// ── Fase 3: paso 170–255  →  R = 0, G sube, B baja ──────────────────────

TEST_F(FiestaColorFormulaTest, Fase3_Paso170_AzulMaximoRojoYVerdeCero) {
    uint8_t r, g, b;
    calcFiestaColor(170, r, g, b);
    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, 0);
    EXPECT_EQ(b, 255);
}

TEST_F(FiestaColorFormulaTest, Fase3_Paso254_VerdeCasiMaximoAzulCasiCero) {
    uint8_t r, g, b;
    calcFiestaColor(254, r, g, b);
    uint8_t p = 254 - 170;           // 84
    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, static_cast<uint8_t>(p * 3));        // 252
    EXPECT_EQ(b, static_cast<uint8_t>(255 - p * 3));  // 3
}

TEST_F(FiestaColorFormulaTest, Fase3_RojoSiempreCeroEnTodosLosPasos) {
    for (int p = 170; p <= 255; ++p) {
        uint8_t r, g, b;
        calcFiestaColor(static_cast<uint8_t>(p), r, g, b);
        EXPECT_EQ(r, 0) << "r debe ser 0 en paso " << p;
    }
}

TEST_F(FiestaColorFormulaTest, Fase3_FormulasExactas) {
    uint8_t r, g, b;
    uint8_t paso = 200;
    uint8_t p    = paso - 170;        // 30
    calcFiestaColor(paso, r, g, b);
    EXPECT_EQ(r, 0);
    EXPECT_EQ(g, static_cast<uint8_t>(p * 3));
    EXPECT_EQ(b, static_cast<uint8_t>(255 - p * 3));
}

// ── Propiedades globales ──────────────────────────────────────────────────

TEST_F(FiestaColorFormulaTest, TodosLosPasos_SiempreHayAlMenosUnCanalNoNulo) {
    for (int p = 0; p <= 255; ++p) {
        uint8_t r, g, b;
        calcFiestaColor(static_cast<uint8_t>(p), r, g, b);
        EXPECT_TRUE(r > 0 || g > 0 || b > 0)
            << "Ningún canal puede ser 0 simultáneamente en paso " << p;
    }
}

TEST_F(FiestaColorFormulaTest, PasosConsecutivosProducenColoresDiferentes) {
    // Excepción conocida: paso 255 y paso 0 producen el mismo color (0,255,0).
    // El test cubre todos los demás pares consecutivos.
    for (int p = 0; p < 254; ++p) {
        uint8_t r1, g1, b1, r2, g2, b2;
        calcFiestaColor(static_cast<uint8_t>(p),     r1, g1, b1);
        calcFiestaColor(static_cast<uint8_t>(p + 1), r2, g2, b2);
        bool diferente = (r1 != r2 || g1 != g2 || b1 != b2);
        EXPECT_TRUE(diferente)
            << "paso " << p << " y " << (p + 1) << " producen el mismo color";
    }
}

// ── loopFiesta: integración mínima ───────────────────────────────────────
// Solo verificamos que loopFiesta llama a setColor y produce un color válido
// cuando se invoca con suficiente tiempo transcurrido.

class FiestaIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_reset();
        estado = Estado{};
        // Tiempo muy grande para garantizar que la ventana interna de 30 ms
        // esté expirada respecto al valor estático `t` de cualquier ejecución previa.
        mock_millis_set(1'000'000'000UL);
    }
};

TEST_F(FiestaIntegrationTest, PrimeraEjecucion_SetColorLlamadaConColorValido) {
    loopFiesta();
    // Con brillo=100, calcFiestaColor nunca produce (0,0,0), así que ledOn=true.
    EXPECT_TRUE(estado.ledOn);
}

TEST_F(FiestaIntegrationTest, EjecucionInmediata_NoModificaLED) {
    loopFiesta();                        // ejecuta, actualiza t interno a 1B

    bool ledOn_snap = estado.ledOn;
    uint32_t r_snap = mock_ledc_get(PIN_R);

    // Llamada inmediata (0 ms después) → debe ser ignorada
    loopFiesta();
    EXPECT_EQ(estado.ledOn,          ledOn_snap);
    EXPECT_EQ(mock_ledc_get(PIN_R),  r_snap);
}
