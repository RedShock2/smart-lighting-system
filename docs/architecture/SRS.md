# System Requirements Specification (SRS)
## Sistema de Iluminación Inteligente

**Versión:** 1.0  
**Fecha:** 16 de Febrero del 2026
**Estado:** Borrador Inicial

---

## 1. Introducción

### 1.1 Propósito
Este documento especifica los requerimientos funcionales y no funcionales del Sistema de Iluminación Inteligente para ambientes residenciales.

### 1.2 Alcance
Sistema embebido que automatiza el control de iluminación mediante:
- Detección de movimiento
- Control automático de luces
- Respeto a preferencias manuales del usuario

### 1.3 Definiciones y Acrónimos
- **PIR:** Passive Infrared (Sensor de movimiento)
- **GPIO:** General Purpose Input/Output
- **PWM:** Pulse Width Modulation
- **Timeout:** Tiempo de espera antes de apagado automático

---

## 2. Descripción General

### 2.1 Perspectiva del Producto
Sistema autónomo que complementa la instalación eléctrica existente sin requerir modificaciones estructurales mayores.

### 2.2 Funciones del Producto
1. Detección automática de presencia
2. Activación/desactivación de iluminación
3. Interpretación de comandos manuales
4. Configuración de parámetros operativos
5. Indicación de estado del sistema

### 2.3 Características de Usuarios
- **Usuarios finales:** Personas sin conocimientos técnicos
- **Instaladores:** Conocimiento básico de electricidad
- **Mantenimiento:** Acceso a configuración mediante interfaz simple

---

## 3. Requerimientos Funcionales

### RF-001: Detección de Movimiento
**Prioridad:** Alta  
**Descripción:** El sistema debe detectar movimiento humano en un rango de 3-7 metros.

**Criterios de aceptación:**
- Ángulo de detección: 110° mínimo
- Tiempo de respuesta: < 500ms
- Falsos positivos: < 5% en condiciones normales

### RF-002: Encendido Automático
**Prioridad:** Alta  
**Descripción:** Al detectar movimiento, la luz debe encenderse automáticamente si está apagada.

**Criterios de aceptación:**
- Delay de activación: < 1 segundo
- Nivel de iluminación: 100% por defecto (configurable)

### RF-003: Apagado Automático
**Prioridad:** Alta  
**Descripción:** Sin detección de movimiento por período configurable, la luz debe apagarse.

**Criterios de aceptación:**
- Timeout configurable: 30s - 15min
- Default: 5 minutos
- Advertencia antes de apagado (opcional): parpadeo 10s antes

### RF-004: Detección de Apagado Manual
**Prioridad:** Alta  
**Descripción:** Si el usuario apaga manualmente la luz, el sistema debe desactivar la automatización temporalmente.

**Criterios de aceptación:**
- Detectar cambio de estado no generado por el sistema
- Período de desactivación: configurable (default 30min)
- Reactivación automática tras período o detección de nuevo ciclo

### RF-005: Modo Manual Override
**Prioridad:** Media  
**Descripción:** Usuario puede forzar encendido/apagado independientemente de detección.

**Criterios de aceptación:**
- Doble pulsación = modo manual permanente
- Triple pulsación = volver a modo automático

### RF-006: Configuración de Parámetros
**Prioridad:** Media  
**Descripción:** Permitir ajuste de:
- Sensibilidad del sensor
- Tiempo de timeout
- Nivel de iluminación
- Período de override manual

**Criterios de aceptación:**
- Interfaz simple (botones/potenciómetros o app)
- Persistencia de configuración (EEPROM)

---

## 4. Requerimientos No Funcionales

### RNF-001: Confiabilidad
- **MTBF:** > 8,000 horas de operación continua
- **Tasa de fallo:** < 1% en 1 año

### RNF-002: Performance
- **Tiempo de respuesta sensor:** < 500ms
- **Tiempo activación relé:** < 100ms
- **Consumo en standby:** < 3W

### RNF-003: Seguridad Eléctrica
- Aislamiento óptico en etapa de potencia
- Protección contra sobrecarga
- Cumplimiento NOM-001-SEDE

### RNF-004: Usabilidad
- Instalación sin herramientas especiales
- Configuración intuitiva (< 5 minutos)
- Indicadores LED de estado

### RNF-005: Mantenibilidad
- Componentes reemplazables fácilmente
- Firmware actualizable (si usa ESP32)
- Diagnóstico de errores mediante LEDs

### RNF-006: Compatibilidad
- Voltaje: 110-127V AC (México)
- Carga máxima: 500W (mínimo)
- Tipos de lámpara: Incandescente, LED, fluorescente

---

## 5. Requerimientos de Interfaz

### 5.1 Interfaz de Hardware
- **Entrada:** Sensor PIR, interruptor manual
- **Salida:** Relé SPDT 10A, LED indicador
- **Alimentación:** Transformador AC/DC 5V 1A

### 5.2 Interfaz de Usuario
- **LED Estado:** 
  - Verde fijo: Modo automático activo
  - Rojo fijo: Modo manual
  - Parpadeo: Configuración/error
  
- **Botón Config:**
  - Presión corta: Cambiar sensibilidad
  - Presión larga (3s): Entrar a configuración
  - Doble click: Toggle modo manual/auto

---

## 6. Diagrama de Contexto del Sistema
```
[Usuario] ──┐
            │
            ▼
      ┌──────────────────────┐
      │   Sistema de         │
      │   Iluminación        │◄──── [Sensor PIR]
      │   Inteligente        │
      └──────────────────────┘
            │
            ├──► [Lámpara/Foco]
            └──► [LED Indicador]
```

---

## 7. Casos de Uso Principales

### CU-001: Encendido por Detección
**Actor:** Sistema (automático)  
**Precondición:** Modo automático activo, luz apagada  
**Flujo:**
1. Sensor detecta movimiento
2. Sistema verifica estado actual (apagado)
3. Sistema activa relé
4. Luz enciende
5. Inicia contador de timeout

### CU-002: Usuario Apaga Manualmente
**Actor:** Usuario  
**Precondición:** Luz encendida (auto o manual)  
**Flujo:**
1. Usuario acciona interruptor físico
2. Sistema detecta cambio de estado no generado por él
3. Sistema entra en modo "override manual"
4. Desactiva automatización por 30min (configurable)
5. Indica modo manual (LED rojo)

### CU-003: Apagado Automático
**Actor:** Sistema  
**Precondición:** Luz encendida, sin movimiento  
**Flujo:**
1. Timeout alcanzado sin nueva detección
2. (Opcional) Advertencia: parpadeo 10s antes
3. Sistema desactiva relé
4. Luz apaga
5. Resetea timeout, espera nuevo movimiento

---

## Notas para Desarrollo

**Priorización de implementación:**
1. ✅ Fase 1: Detección básica y encendido/apagado automático (RF-001, RF-002, RF-003)
2. ✅ Fase 2: Detección de override manual (RF-004)
3. 🔄 Fase 3: Configuración y modos avanzados (RF-005, RF-006)

**Tecnologías sugeridas:**
- **Microcontrolador:** ESP32 (permite WiFi futuro) o Arduino Nano
- **Sensor:** HC-SR501 (ajustable, económico)
- **Relé:** Songle SRD-05VDC con optoacoplador

