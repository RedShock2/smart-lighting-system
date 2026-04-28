# TC-002 — Detección de Nivel de Luz (LDR)

**Responsable:** Juan Luis (Testing & QA)  
**Sprint:** 2  
**Estado:** 🔄 Pendiente

---

## Descripción

Verificar que el módulo LDR detecta correctamente la diferencia entre luz ambiental y oscuridad.

## Precondiciones

- ESP32 encendido con firmware v0.2.0
- Módulo LDR conectado: VCC→3.3V, GND→GND, DO→GPIO34
- Potenciómetro del módulo LDR calibrado previamente
- Serial Monitor abierto a 115200 baud

## Pasos

1. Abrir Serial Monitor
2. Dejar el LDR expuesto a luz normal del cuarto
3. Verificar que Serial muestra `HAY LUZ`
4. Tapar el sensor LDR con la mano completamente
5. Verificar que Serial muestra `OSCURO`
6. Destapar el sensor
7. Verificar que regresa a `HAY LUZ`

## Resultado Esperado

```
PIR: SIN MOV | Luz: HAY LUZ | LED: OFF   ← con luz
PIR: SIN MOV | Luz: OSCURO  | LED: OFF   ← tapando LDR (sin movimiento)
PIR: SIN MOV | Luz: HAY LUZ | LED: OFF   ← destapando
```

## Resultado Real

| Campo | Valor |
|---|---|
| Fecha | |
| Firmware | v0.2.0 |
| Resultado | ⬜ PASS / ⬜ FAIL |
| Observaciones | |

## Bugs Conocidos

| Bug | Descripción |
|---|---|
| BUG-02 | Si la lógica está invertida, cambiar `LDR_DARK_VALUE` en config.h |
