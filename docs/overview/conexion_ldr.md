# Documentación de conexión del módulo LDR en protoboard

## 1. Datos generales, objetivo y alcance

* **Proyecto:** Sistema de Iluminación Inteligente
* **Módulo documentado:** LDR
* **Responsable:** Uriel Everardo
* **Sprint:** Sprint 2
* **Versión del documento:** 1.5
* **Fecha de creación:** 17/03/2026
* **Última actualización:** 10/04/2026

**Objetivo**
Documentar la conexión del módulo LDR en protoboard dentro de un sistema embebido de iluminación inteligente, estableciendo una relación clara entre el armado físico y el firmware del proyecto, con el fin de contribuir a la automatización de iluminación y optimización del consumo energético.

**Alcance**
Este documento se enfoca únicamente en las actividades asignadas a Uriel Everardo:

* documentar la conexión del módulo LDR en protoboard
* registrar evidencia fotográfica del armado
* validar que el hardware observado coincida con el firmware base del proyecto

Quedan fuera de este documento tareas de desarrollo de firmware, pruebas QA e integración general del sistema.

---

## 2. Descripción general del módulo LDR

El módulo LDR forma parte de un sistema de iluminación inteligente diseñado para automatizar el encendido de luces en función de la presencia de personas y la condición de luz ambiental.

Su función principal es detectar el nivel de iluminación del entorno. Esta información se combina con la señal del sensor PIR para determinar si el sistema debe encender o apagar la iluminación.

Dentro del modo automático del sistema, el encendido del LED depende de dos condiciones:

* detección de movimiento
* condición de oscuridad

La interpretación exacta de la señal del LDR depende del comportamiento físico del módulo y no debe asumirse sin validación directa.

---

## 3. Componentes utilizados e información inicial del módulo

### Componentes utilizados

| Componente     | Cantidad | Descripción / nota         |
| -------------- | -------: | -------------------------- |
| ESP32          |        1 | Microcontrolador principal |
| Módulo LDR     |        1 | Sensor de luz ambiente     |
| Protoboard     |        1 | Plataforma de pruebas      |
| Cables Dupont  |  ------- | -------                    |
| LED RGB        |        1 | Iluminación del sistema    |
| Resistencia(s) |  ------- | -------                    |
| Sensor PIR     |        1 | Detección de movimiento    |
| Sensor DHT11   |        1 | Temperatura y humedad      |
| Pantalla OLED  |        1 | Visualización local        |

### Información inicial del módulo LDR

* **Nombre del módulo:** -------
* **Modelo / fabricante:** -------
* **Tipo de salida usada:** Digital (según firmware y comportamiento del sistema)
* **Voltaje de operación:** -------
* **Observación inicial:** El módulo cuenta con ajuste de sensibilidad mediante potenciómetro

---

## 4. Pines del sistema relacionados

Con base en el firmware del sistema:

| Señal           |        Pin |
| --------------- | ---------: |
| `PIR_PIN`       |         14 |
| `LED_PIN (RGB)` | 25, 26, 27 |
| `LDR_PIN`       |         35 |

**Observación:** estos valores corresponden a la configuración en código y deben validarse contra el hardware real.

---

## 5. Tabla de conexiones del módulo LDR

| Elemento                          | Conexión esperada / observada | Estado                         |
| --------------------------------- | ----------------------------- | ------------------------------ |
| VCC del módulo LDR                | -------                       | -------                        |
| GND del módulo LDR                | -------                       | -------                        |
| Salida del módulo LDR             | GPIO 35                       | Definido en firmware           |
| Relación con ESP32                | Entrada digital               | Pendiente de validación física |
| Relación con rieles de protoboard | -------                       | -------                        |

---

## 6. Descripción general del armado en protoboard

El módulo LDR se integra al circuito general del sistema junto con el ESP32, sensor PIR, LED RGB, DHT11 y pantalla OLED.

Su función dentro del montaje es proporcionar la condición de luz ambiental para la lógica de control del sistema.

**Descripción general del armado:** `-------`

---

## 7. Posición física en protoboard

* **Ubicación del módulo LDR en protoboard:** -------
* **Orientación del módulo:** -------
* **Fila(s) ocupadas:** -------
* **Relación física con el ESP32:** -------
* **Relación física con el sensor PIR:** -------
* **Relación física con el LED:** -------

---

## 8. Diagrama textual de conexión

```text
ESP32
├── GPIO 14  -> PIR
├── GPIO 25  -> LED R
├── GPIO 26  -> LED G
├── GPIO 27  -> LED B
└── GPIO 35  -> LDR

Alimentación y tierras:
ESP32  -> -------
LDR    -> -------
PIR    -> -------
LED    -> -------
Protoboard rails -> -------
```

---

## 9. Evidencia fotográfica

* `hardware/ldr/protoboard_1` → -------
* `hardware/ldr/protoboard_2` → -------
* `hardware/ldr/protoboard_3` → -------
* `hardware/ldr/protoboard_4` → -------

---

## 10. Validación con firmware

El firmware del sistema utiliza el módulo LDR mediante lectura digital en el pin GPIO 35.

Esta señal se evalúa dentro del modo automático en conjunto con el sensor PIR para determinar el encendido del sistema de iluminación.

**Validación física contra firmware:** `-------`

---

## 11. Resultados observados

* Lectura del LDR en condiciones de luz: -------
* Lectura del LDR en condiciones de oscuridad: -------
* Comportamiento del sistema en modo automático: -------

---

## 12. Lógica observada del módulo

De acuerdo con el firmware:

* Se utiliza lectura digital del LDR
* Se evalúa una condición lógica para determinar oscuridad

Durante pruebas del sistema, se reportó que el comportamiento del módulo LDR estaba invertido respecto a la lógica esperada en el firmware, por lo que fue necesario ajustar la interpretación en el código.

Sin embargo, la relación exacta entre:

* LOW → oscuro
* HIGH → oscuro

no ha sido validada físicamente en este documento.

**Lógica real del módulo:** `-------`

---

## 13. Prueba funcional del sistema

* Encendido con oscuridad y movimiento: -------
* Apagado sin movimiento: -------
* Comportamiento con luz ambiente: -------

---

## 14. Incidencias y observaciones técnicas

* Se detectó durante pruebas que la lógica del módulo LDR se encontraba invertida respecto a lo esperado
* Se realizaron ajustes en firmware para corregir este comportamiento (según QA)

---

---

## 15. Conclusión, trabajo pendiente e historial de cambios

**Conclusión**
El módulo LDR se integra correctamente a nivel de firmware en el sistema mediante el pin GPIO 35 y cumple un papel clave en la detección de condiciones de iluminación para la automatización del encendido. No obstante, la validación física completa del comportamiento del sensor permanece pendiente.

**Trabajo pendiente**

* Verificar conexiones físicas reales
* Confirmar comportamiento HIGH/LOW del módulo
* Registrar evidencia fotográfica
* Completar validación funcional en hardware

**Historial de cambios**

* v1.4 → Base del documento
* v1.5 → Integración completa, alineación con firmware y registro de incidencias (posibles cambios)
