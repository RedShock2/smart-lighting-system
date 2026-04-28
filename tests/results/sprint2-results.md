# 📊 Resultados de Pruebas — Sprint 2

**Responsable:** Juan Luis (Testing & QA)  
**Versión firmware:** 0.2.0

---

## Resumen

| Caso | Descripción | Resultado |
|---|---|---|
| TC-001 | Detección PIR | ⬜ Pendiente |
| TC-002 | Detección LDR | ⬜ Pendiente |
| TC-003 A | Luz + movimiento → OFF | ⬜ Pendiente |
| TC-003 B | Oscuro + sin mov → OFF | ⬜ Pendiente |
| TC-003 C | Oscuro + movimiento → ON | ⬜ Pendiente |
| TC-003 D | Apagado automático 10s | ⬜ Pendiente |

---

## Bugs Encontrados en Sprint 2

| ID | Descripción | Severidad | Estado |
|---|---|---|---|
| BUG-01 | GPIO 34 flota sin LDR conectado | Alta | 🔄 En revisión |
| BUG-02 | Lógica LDR invertida en algunos módulos | Media | ✅ Mitigado con config.h |
| BUG-03 | PIR da falsos positivos en primeros 60s | Baja | ✅ Documentado en BOM |

---

## Notas del QA

> Completar tras ejecutar las pruebas del sprint 2.
