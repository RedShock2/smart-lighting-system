# TC-004 — Apagado Automático por Tiempo

**Responsable:** Juan Luis (Testing & QA)  
**Sprint:** 3  
**Estado:** 🔄 Pendiente

---

## Descripción

Verificar que el LED se apaga automáticamente exactamente 10 segundos después de que el PIR deja de detectar movimiento.

## Precondiciones

- Firmware v1.0.0 cargado
- PIR calentado (60 segundos encendido)
- LDR tapado (condición oscuro activa)
- LED encendido (condición OSCURo + MOVIMIENTO activa)

## Pasos

1. Con LED encendido, dejar de moverse completamente
2. Iniciar cronómetro en el momento exacto que dejas de moverte
3. Observar Serial Monitor
4. A los 10 segundos exactos el LED debe apagarse
5. Registrar tiempo real de apagado

## Resultado Esperado

```
>> LED ON  -- oscuro + movimiento detectado
PIR: MOVIMIENTO | Luz: OSCURO  | LED: ON
PIR: MOVIMIENTO | Luz: OSCURO  | LED: ON
PIR: SIN MOV    | Luz: OSCURO  | LED: ON   ← dejas de moverte
...  (10 segundos)
>> LED OFF -- condicion no cumplida
PIR: SIN MOV    | Luz: OSCURO  | LED: OFF
```

## Resultado Real

| Campo | Valor |
|---|---|
| Fecha | |
| Firmware | v1.0.0 |
| Tiempo real de apagado | ___ segundos |
| Resultado | ⬜ PASS / ⬜ FAIL |
| Observaciones | |

---

# TC-005 — Reactivación tras Apagado

**Sprint:** 3 | **Estado:** 🔄 Pendiente

## Descripción

Verificar que el sistema vuelve a encender el LED correctamente después de haberse apagado por tiempo.

## Pasos

1. Dejar que el LED se apague por timeout (TC-004)
2. Con LDR aún tapado, mover la mano frente al PIR
3. El LED debe volver a encender inmediatamente

## Resultado Esperado

```
>> LED OFF -- condicion no cumplida
PIR: SIN MOV | Luz: OSCURO | LED: OFF
>> LED ON  -- oscuro + movimiento detectado   ← al moverte de nuevo
```

## Resultado Real

| Campo | Valor |
|---|---|
| Fecha | |
| Resultado | ⬜ PASS / ⬜ FAIL |
| Observaciones | |
