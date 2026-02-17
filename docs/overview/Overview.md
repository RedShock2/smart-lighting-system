# Sistema de Iluminación Inteligente para Hogar

## 📝 Descripción del Proyecto

Sistema embebido de iluminación automatizada que detecta la presencia de personas en una habitación y controla las luces de forma inteligente. El sistema enciende automáticamente las luces cuando detecta movimiento, pero respeta la decisión del usuario si este apaga manualmente la luz, interpretando esta acción como preferencia de mantener la habitación a oscuras.

**¿Qué problema resuelve?**
- Ahorro energético al evitar luces encendidas en espacios vacíos
- Comodidad al automatizar la iluminación según presencia
- Respeto a las preferencias del usuario

**Funcionamiento básico:**
1. Sensor de movimiento detecta presencia → Luz se enciende automáticamente
2. Si usuario apaga la luz manualmente → Sistema interpreta preferencia y desactiva automatización temporal
3. Sin movimiento por período definido → Luz se apaga automáticamente

## 🎯 Objetivo del Proyecto

Desarrollar un sistema embebido de bajo costo que automatice la iluminación residencial mediante sensores de movimiento, optimizando el consumo energético mientras mantiene control intuitivo para el usuario.

**Objetivos específicos:**
- Implementar detección de movimiento confiable con sensor PIR
- Desarrollar lógica de control que diferencie entre apagado automático y manual
- Crear interfaz de configuración para tiempos de espera y sensibilidad
- Lograr eficiencia energética con consumo < 5W en standby

## ✅ Factibilidad del Proyecto

### Factibilidad Técnica
- **Sensores:** PIR (HC-SR501) - bajo costo, ampliamente disponible
- **Controlador:** Arduino/ESP32 - documentación extensa, fácil programación
- **Actuador:** Relé para control de carga AC o LED inteligente
- **Complejidad:** Media - requiere manejo de interrupciones y estados

### Factibilidad Económica
**Presupuesto estimado:** $800 - $1,755 MXN

| Componente | Costo aprox. |
|------------|--------------|
| Microcontrolador (ESP32/Arduino) | $150-300 |
| Sensor PIR HC-SR501 | $50-80 |
| Módulo Relé | $40-60 |
| Fuente de alimentación | $100-150 |
| Componentes varios (cables, PCB, caja) | $200-300 |
| LED/Foco prueba | $100-200 |
| Casa de madera | $255 |

### Factibilidad de Tiempo
**Duración estimada:** 8-10 semanas

- Semanas 1-2: Investigación y diseño
- Semanas 3-4: Prototipo inicial y pruebas de sensores
- Semanas 5-6: Desarrollo de lógica de control
- Semanas 7-8: Integración y testing
- Semanas 9-10: Documentación y refinamiento

### Riesgos Identificados
1. **Falsos positivos del sensor PIR** - Mitigación: calibración y filtrado de señal
2. **Interferencia eléctrica** - Mitigación: aislamiento óptico en relé
3. **Detección de apagado manual** - Mitigación: sensor de corriente adicional

## 📐 Arquitectura y Requerimientos

Para consultar los requerimientos detallados del sistema (SRS), ver:
👉 [System Requirements Specification (SRS)](./architecture/SRS.md)
