# 📋 Plan de Pruebas — Smart Lighting System

**Versión:** 0.1.0  
**Responsable:** Juan Luis (Testing & QA)

---

## 1. Objetivo

Validar que el sistema de iluminación inteligente funciona correctamente bajo las condiciones definidas en la lógica del sistema.

---

## 2. Alcance

Las pruebas cubren:
- Detección de movimiento (PIR HC-SR501)
- Detección de nivel de luz (módulo LDR LM393)
- Control del LED en respuesta a ambos sensores
- Comportamiento en condiciones de borde

---

## 3. Entorno de Pruebas

| Item | Detalle |
|---|---|
| Hardware | ESP32 Wroom-32 + PIR + LDR + LED |
| Software | Firmware v0.2.0+ |
| Herramienta | Arduino IDE / PlatformIO Serial Monitor (115200 baud) |
| Condiciones | Cuarto con luz controlable, temperatura ambiente |

---

## 4. Casos de Prueba

| ID | Descripción | Estado |
|---|---|---|
| TC-001 | Detección de movimiento con PIR | 🔄 Pendiente |
| TC-002 | Detección de nivel de luz con LDR | 🔄 Pendiente |
| TC-003 | LED enciende con oscuridad + movimiento | 🔄 Pendiente |
| TC-004 | LED apaga tras 10 seg sin movimiento | 🔄 Pendiente |
| TC-005 | LED no enciende con luz aunque haya movimiento | 🔄 Pendiente |
| TC-006 | LED no enciende en oscuridad sin movimiento | 🔄 Pendiente |

---

## 5. Criterios de Aceptación

- ✅ El PIR detecta movimiento en rango de 1–3 metros
- ✅ El LDR distingue entre luz ambiental y oscuridad
- ✅ El LED responde en menos de 500ms tras detectar condición
- ✅ El LED se apaga exactamente a los 10 segundos sin movimiento
- ✅ El Serial Monitor muestra información legible en tiempo real
