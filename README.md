# 💡 Smart Lighting System
Sistema embebido de iluminación inteligente con control automático por sensores
y app web en tiempo real, desarrollado sobre ESP32 WROOM-32.

## 📖 Documentación
- [Overview del Proyecto](./docs/overview/Overview.md)
- [Requerimientos del Sistema (SRS)](./docs/architecture/SRS.md)

## 🚀 Estado del Proyecto
**Fase actual:** Funcional — en pruebas finales
**Versión:** 1.0.0

## ✨ Funcionalidades
- **Modo AUTO:** enciende automáticamente si detecta oscuridad + movimiento.
  Se apaga a los 10 segundos sin movimiento.
- **Modo FIESTA:** ciclo de colores RGB animado.
- **App web** accesible desde cualquier dispositivo en la red local.
- **Control manual** de color RGB y brillo desde la app.
- **Monitoreo en tiempo real** de temperatura, humedad, luz y movimiento.
- **Pantalla OLED** con datos del sistema sin necesidad de abrir la app.

## 👥 Equipo de Desarrollo
| Rol | Responsable |
|-----|-------------|
| **Project Manager** | Luis Antonio Padilla Mondragón |
| **Hardware Lead** | Uriel Everardo Sánchez Rangel |
| **Software Lead** | Luis Alejandro Alcocer Marín |
| **Testing & QA** | Juan Luis Ramírez Hernández |

## 🛠️ Hardware
| Componente | Función | Pin ESP32 |
|---|---|---|
| LED RGB cátodo común | Iluminación principal | GPIO 25, 26, 27 |
| Sensor PIR | Detección de movimiento | GPIO 14 |
| Módulo LDR | Detección de luz ambiente | GPIO 35 |
| Sensor DHT11 | Temperatura y humedad | GPIO 13 |
| Pantalla OLED 128x64 | Display local (I2C) | GPIO 21, 22 |

## 💻 Stack Tecnológico
- **Microcontrolador:** ESP32 WROOM-32
- **IDE:** Arduino IDE 2.x con ESP32 Core 3.3.8
- **Firmware:** C++ (Arduino framework)
- **App web:** HTML + CSS + JavaScript (embebida en Flash del ESP32)
- **Comunicación:** HTTP REST sobre WiFi

## 🌐 Endpoints HTTP
| Endpoint | Descripción |
|---|---|
| `GET /` | App web |
| `GET /modo?m=auto\|fiesta\|off` | Cambiar modo |
| `GET /color?r=X&g=X&b=X` | Color manual RGB |
| `GET /brillo?v=X` | Intensidad 0–100% |
| `GET /status` | Estado del sistema en JSON |

## 📂 Estructura
smart-lighting-system/
├── docs/
│   ├── architecture/    # SRS
│   └── overview/        # Overview
├── src/
│   ├── smart_lighting.ino           # Firmware (sin comentarios)
│   └── smart_lighting_comentado.ino # Firmware documentado
├── hardware/            # Esquemas de conexión
└── tests/               # Pruebas y calibración

## 📋 Roadmap
- [x] Definición de requerimientos
- [x] Diseño de arquitectura
- [x] Conexión y prueba de hardware
- [x] Firmware con control PWM y servidor HTTP
- [x] App web con control en tiempo real
- [x] Integración DHT11 + OLED
- [ ] Pruebas finales y entrega
