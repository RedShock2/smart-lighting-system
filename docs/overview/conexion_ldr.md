# Documentación de conexión del módulo LDR en protoboard

## 1. Datos generales

**Proyecto:** Sistema de Iluminación Inteligente  
**Módulo documentado:** LDR  
**Responsable:** Uriel Everardo  
**Sprint:** Sprint 2  
**Versión del documento:** 1.0  
**Fecha de creación:** 17/03  
**Última actualización:** Hoy


## 2. Objetivo

Documentar la conexión física del módulo LDR dentro del sistema, así como dejar una base inicial de referencia para su posterior validación con el firmware y con el armado real en protoboard.


## 3. Alcance

Este documento cubre de manera inicial:

- descripción general del módulo LDR dentro del proyecto
- componentes relacionados con su conexión
- información base del hardware utilizado

Queda pendiente para versiones posteriores:

- tabla de conexiones detallada
- evidencia fotográfica
- validación con firmware
- resultados de prueba
- incidencias encontradas


## 4. Descripción general

El módulo LDR forma parte del sistema de iluminación inteligente del proyecto. Su función es detectar la condición de luz ambiental y permitir que el sistema determine si existe oscuridad o presencia de luz.

Esta lectura será utilizada junto con la detección de movimiento para activar o no el LED según la lógica definida en el sprint.

En esta primera versión se establece la base documental del módulo para después completar el detalle del cableado, evidencia física y validaciones.


## 5. Componentes utilizados

| Componente | Cantidad | Descripción / nota |
|---|---:|---|
| ESP32 | 1 | Placa principal del sistema |
| Módulo LDR | 1 | ------- |
| Protoboard | 1 | ------- |
| Cables Dupont | ------- | ------- |
| LED | 1 | Salida visual del sistema |
| Resistencia para LED | ------- | ------- |
| Sensor PIR | 1 | Sensor de movimiento usado junto con el LDR |


## 6. Información inicial del módulo LDR

**Nombre del módulo:** -------  
**Modelo / fabricante:** -------  
**Tipo de salida usada:** -------  
**Voltaje de operación:** -------  
**Observación inicial:** -------  


## 7. Pendientes para la siguiente versión

- agregar pines del sistema relacionados
- documentar tabla de conexiones del LDR
- describir armado general en protoboard
