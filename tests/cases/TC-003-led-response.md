# TC-003 — Respuesta del LED (Lógica Completa)

**Responsable:** Juan Luis (Testing & QA)  
**Sprint:** 2  
**Estado:** 🔄 Pendiente

---

## Descripción

Validar que el LED responde correctamente a todas las combinaciones posibles de la tabla lógica del sistema.

## Precondiciones

- Firmware v0.2.0 cargado
- PIR calentado (60 segundos encendido)
- LDR calibrado con potenciómetro
- Serial Monitor abierto

## Casos a Probar

### Caso A — Hay luz + movimiento → LED OFF
1. Dejar luz del cuarto encendida
2. Mover la mano frente al PIR
3. **Esperado:** LED permanece apagado
4. Serial debe mostrar: `PIR: MOVIMIENTO | Luz: HAY LUZ | LED: OFF`

### Caso B — Oscuro + sin movimiento → LED OFF
1. Tapar el LDR
2. Quedarse inmóvil
3. **Esperado:** LED permanece apagado
4. Serial debe mostrar: `PIR: SIN MOV | Luz: OSCURO | LED: OFF`

### Caso C — Oscuro + movimiento → LED ON ✅
1. Tapar el LDR
2. Mover la mano frente al PIR
3. **Esperado:** LED enciende
4. Serial debe mostrar: `>> LED ON ← oscuro + movimiento`

### Caso D — LED apaga tras 10 segundos sin movimiento
1. Con LED encendido (caso C)
2. Dejar de moverse frente al PIR
3. Cronometrar 10 segundos
4. **Esperado:** LED se apaga automáticamente
5. Serial debe mostrar: `>> LED OFF`

## Tabla de Resultados

| Caso | Luz | Movimiento | LED Esperado | LED Real | Resultado |
|---|---|---|---|---|---|
| A | ☀️ Hay luz | ✅ Sí | ⚫ OFF | | ⬜ |
| B | 🌑 Oscuro | ❌ No | ⚫ OFF | | ⬜ |
| C | 🌑 Oscuro | ✅ Sí | 🟡 ON | | ⬜ |
| D | 🌑 Oscuro | ❌ (10s) | ⚫ OFF | | ⬜ |

## Fecha de Ejecución

| Campo | Valor |
|---|---|
| Fecha | |
| Firmware | v0.2.0 |
| Resultado Global | ⬜ PASS / ⬜ FAIL |
| Observaciones | |
