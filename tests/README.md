# Tests — Smart Lighting System

Suite de unit tests en C++ (Google Test) para la lógica del sketch `smart_lighting_1.0`.

## Estructura

```
tests/
├── CMakeLists.txt        # configuración de build (descarga Google Test automáticamente)
├── logic.h / logic.cpp   # lógica del sistema extraída y desacoplada del hardware
├── mocks/                # simulación de la API Arduino/ESP32 (ledcWrite, digitalRead, millis…)
└── unit/                 # archivos de test
    ├── test_color.cpp      → setColor, apagar, escalado de brillo PWM
    ├── test_auto_mode.cpp  → loopAuto: throttling 200 ms, oscuridad, movimiento, timeout
    ├── test_fiesta.cpp     → fórmula de colores del arco iris (las 3 fases) + integración
    ├── test_handlers.cpp   → lógica de los endpoints HTTP (color, brillo, modo)
    └── test_estado.cpp     → valores por defecto del struct Estado y transiciones de modo
```

## Cómo correr los tests

```bash
cd tests
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_TLS_VERIFY=0
cmake --build build
./build/smart_lighting_tests.exe
```

> Google Test se descarga automáticamente al hacer el primer `cmake`.

---

## Notas

Hola miss, los unit tests y las otras pruebas ya estaban hechos desde antes pero mis conocimientos en git son tan buenos que no los habia subido hasta que me dijeron y revise el repo.
