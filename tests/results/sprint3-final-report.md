# 📊 Reporte Final de Calidad — v1.0.0

**Responsable:** Juan Luis (Testing & QA)  
**Sprint:** 3 — Release final

---

## Resumen Ejecutivo

| Total casos | Ejecutados | PASS | FAIL | Pendientes |
|---|---|---|---|---|
| 7 | 0 | 0 | 0 | 7 |

> Completar tras ejecutar la suite completa.

---

## Resultados por Caso

| ID | Descripción | Resultado | Fecha |
|---|---|---|---|
| TC-001 | Detección PIR | ⬜ | |
| TC-002 | Detección LDR | ⬜ | |
| TC-003-A | Luz + mov → OFF | ⬜ | |
| TC-003-B | Oscuro + sin mov → OFF | ⬜ | |
| TC-003-C | Oscuro + mov → ON | ⬜ | |
| TC-004 | Apagado automático 10s | ⬜ | |
| TC-005 | Reactivación tras timeout | ⬜ | |

---

## Estado de Bugs

| ID | Descripción | Severidad | Estado Final |
|---|---|---|---|
| BUG-01 | GPIO 34 flota sin LDR | Alta | ✅ Documentado en schematic final |
| BUG-02 | Lógica LDR invertida | Media | ✅ Resuelto con LDR_DARK_VALUE en config.h |
| BUG-03 | PIR falsos positivos 60s | Baja | ✅ Documentado en banner de inicio |

---

## Observaciones Finales del QA

> Completar tras ejecutar las pruebas.

---

## Firma de Aceptación

| Rol | Nombre | Firma |
|---|---|---|
| QA | Juan Luis | |
| Software Lead | Luis Alejandro | |
| PM | Luis Antonio | |
