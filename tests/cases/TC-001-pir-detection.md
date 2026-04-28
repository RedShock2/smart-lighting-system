# TC-001 — Detección de Movimiento PIR

**Responsable:** Juan Luis (Testing & QA)  
**Sprint:** 1  
**Estado:** 🔄 Pendiente

---

## Descripción

Verificar que el sensor PIR HC-SR501 detecta correctamente movimiento humano y lo reporta al ESP32.

## Precondiciones

- ESP32 encendido con firmware v0.1.0+
- PIR conectado: VCC→5V, GND→GND, OUT→GPIO14
- Serial Monitor abierto a 115200 baud
- **Esperar 60 segundos** tras encender para que el PIR se caliente

## Pasos

1. Abrir Serial Monitor
2. Mantenerse inmóvil frente al PIR por 10 segundos
3. Observar valor de PIR en Serial Monitor → debe ser `0`
4. Mover la mano frente al PIR
5. Observar valor de PIR en Serial Monitor → debe cambiar a `1`
6. Volver a estar inmóvil
7. Observar que el valor regresa a `0`

## Resultado Esperado

```
PIR: 0 | LDR: X   ← sin movimiento
PIR: 1 | LDR: X   ← con movimiento
PIR: 0 | LDR: X   ← sin movimiento de nuevo
```

## Resultado Real

> *Completar tras ejecutar la prueba*

| Campo | Valor |
|---|---|
| Fecha | |
| Firmware | |
| Resultado | ⬜ PASS / ⬜ FAIL |
| Observaciones | |

## Bugs Encontrados

> *Ninguno por ahora*
