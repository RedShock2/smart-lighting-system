# Documentación de conexión del módulo LDR en protoboard

## 1. Datos generales, objetivo y alcance

- **Proyecto:** Sistema de Iluminación Inteligente
- **Módulo documentado:** LDR
- **Responsable:** Uriel Everardo
- **Sprint:** Sprint 2
- **Versión del documento:** 1.4
- **Fecha de creación:** 17/03/2026
- **Última actualización:** 10/04/2026

**Objetivo**  
Documentar la conexión del módulo LDR en protoboard dentro del Sistema de Iluminación Inteligente, así como dejar una base clara para relacionar el armado físico con el firmware del proyecto.

**Alcance**  
Este documento se enfoca únicamente en las actividades asignadas a Uriel Everardo:

- documentar la conexión del módulo LDR en protoboard
- registrar evidencia fotográfica del armado
- validar que el hardware observado coincida con el firmware base del proyecto

Quedan fuera de este documento tareas de implementación de `pir.cpp`, `ldr.cpp`, integración de `main.cpp`, casos de prueba de QA y planeación del sprint.

## 2. Descripción general del módulo LDR

El módulo LDR forma parte del Sistema de Iluminación Inteligente basado en ESP32, PIR, LED y LDR. Su función es aportar información sobre la condición de luz ambiental para que el sistema pueda evaluar, junto con la detección de movimiento, si debe activarse o no el LED.

De acuerdo con la lógica funcional esperada del sprint, el LED debe encenderse únicamente cuando exista oscuridad y movimiento simultáneamente. Sin embargo, la lógica real del módulo LDR no debe asumirse sin validación física, ya que puede variar según el tipo de módulo utilizado.

## 3. Componentes utilizados e información inicial del módulo

### Componentes utilizados

| Componente | Cantidad | Descripción / nota |
|---|---:|---|
| ESP32 | 1 | Placa principal del sistema |
| Módulo LDR | 1 | ------- |
| Protoboard | 1 | Superficie de montaje para pruebas |
| Cables Dupont | ------- | ------- |
| LED | 1 | Salida visual del sistema |
| Resistencia para LED | ------- | ------- |
| Sensor PIR | 1 | Sensor de movimiento usado junto con el LDR |

### Información inicial del módulo LDR

- **Nombre del módulo:** -------
- **Modelo / fabricante:** -------
- **Tipo de salida usada:** -------
- **Voltaje de operación:** -------
- **Observación inicial:** -------

## 4. Pines del sistema relacionados

Con base en la referencia técnica del proyecto, los pines base del sistema son los siguientes:

| Señal | Pin base |
|---|---:|
| `PIR_PIN` | 14 |
| `LED_PIN` | 26 |
| `LDR_PIN` | 34 |

**Observación:** estos pines se toman como referencia documental y deben compararse contra el armado físico real antes de dar por concluida la validación.

## 5. Tabla de conexiones del módulo LDR

| Elemento | Conexión esperada / observada | Estado |
|---|---|---|
| VCC del módulo LDR | ------- | ------- |
| GND del módulo LDR | ------- | ------- |
| Salida del módulo LDR | GPIO 34 (base del proyecto) | Pendiente de confirmación física |
| Relación con ESP32 | ------- | ------- |
| Relación con rieles de protoboard | ------- | ------- |

## 6. Descripción general del armado en protoboard

El módulo LDR forma parte del montaje general del sistema en protoboard junto con el ESP32, el sensor PIR y el LED. En esta versión del documento se deja asentado que el sensor participa en el armado de prueba del Sprint 2, pero la descripción exacta del recorrido de cableado y la distribución final de conexiones continúa pendiente de confirmación física.

**Descripción general del armado:** `-------`

## 7. Posición física en protoboard

- **Ubicación del módulo LDR en protoboard:** -------
- **Orientación del módulo:** -------
- **Fila(s) ocupadas:** -------
- **Relación física con el ESP32:** -------
- **Relación física con el sensor PIR:** -------
- **Relación física con el LED:** -------

## 8. Diagrama textual de conexión

El diagrama textual representa una referencia base del sistema y debe entenderse solo como apoyo documental mientras no se complete la verificación física final:

```text
ESP32
├── GPIO 14  -> PIR
├── GPIO 26  -> LED
└── GPIO 34  -> LDR

Alimentación y tierras:
ESP32  -> -------
LDR    -> -------
PIR    -> -------
LED    -> -------
Protoboard rails -> -------
