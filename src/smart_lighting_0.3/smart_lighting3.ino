/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║           SMART LIGHTING SYSTEM — ESP32 WROOM-32            ║
 * ║                  Sistema de Iluminación RGB                  ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  Repositorio : github.com/smart-lighting-system             ║
 * ║  Versión     : 2.0.0                                        ║
 * ║  Plataforma  : ESP32 WROOM-32 + Arduino IDE                 ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  Alumnos:                                                    ║
 * ║    · Luis Alejandro Alcocer Marín                           ║
 * ║    · Luis Antonio Padilla Mondragón                         ║
 * ║    · Juan Luis Ramírez Hernández                            ║
 * ║    · Uriel Everardo Sánchez Rangel                          ║
 * ╠══════════════════════════════════════════════════════════════╣
 * ║  Hardware:                                                   ║
 * ║    · LED RGB cátodo común x2                                ║
 * ║    · Sensor PIR (detección de movimiento)                   ║
 * ║    · LDR (fotorresistencia, detección de luz ambiente)      ║
 * ╚══════════════════════════════════════════════════════════════╝
 *
 * MODOS DE OPERACIÓN:
 *  - AUTO   : Enciende solo si está oscuro Y hay movimiento.
 *             Se apaga X segundos después de perder movimiento.
 *  - DÍA    : Blanco frío fijo (simula luz de trabajo)
 *  - NOCHE  : Azul tenue fijo (luz nocturna suave)
 *  - FIESTA : Ciclo de colores animado
 *  - OFF    : Apagado total
 *
 * ENDPOINTS HTTP:
 *  GET /           → App web
 *  GET /modo?m=X   → Cambiar modo (auto|dia|noche|fiesta|off)
 *  GET /color?r=X&g=X&b=X → Color RGB manual
 *  GET /status     → JSON con estado actual
 *  GET /calibrar   → JSON con valores crudos del LDR y PIR
 */

#include <WiFi.h>
#include <WebServer.h>
// ArduinoJson no requerido — JSON construido manualmente

// ════════════════════════════════════════════════
//  CONFIGURACIÓN — Editar antes de flashear
// ════════════════════════════════════════════════

const char* WIFI_SSID     = "Mega_2.4G_6017";       // Nombre de tu red WiFi
const char* WIFI_PASSWORD = "UPdtCbKK";   // Contraseña de tu red WiFi

// ════════════════════════════════════════════════
//  PINES DE HARDWARE
// ════════════════════════════════════════════════

// LED RGB #1 (ya conectado)
#define PIN_R1   4
#define PIN_G1   5
#define PIN_B1   18

// LED RGB #2 (próximamente)
#define PIN_R2   16
#define PIN_G2   17
#define PIN_B2   19

// Sensores
#define PIN_PIR  27   // Sensor de movimiento (digital)
#define PIN_LDR  34   // Fotorresistencia (analógico, solo lectura)

// ════════════════════════════════════════════════
//  CANALES PWM (LEDC del ESP32)
// ════════════════════════════════════════════════
// El ESP32 tiene 16 canales PWM independientes.
// Asignamos 3 por LED (R, G, B).

#define CH_R1 0
#define CH_G1 1
#define CH_B1 2
#define CH_R2 3
#define CH_G2 4
#define CH_B2 5

#define PWM_FRECUENCIA 5000   // Hz — suficiente para LEDs sin parpadeo visible
#define PWM_RESOLUCION 8      // bits → valores de 0 a 255

// ════════════════════════════════════════════════
//  PARÁMETROS DE LÓGICA AUTO
// ════════════════════════════════════════════════

// LDR: valor analógico 0–4095 (ADC de 12 bits)
// Valor ALTO = poca luz (más resistencia = menor voltaje ≠... espera, depende del divisor)
// Calibrar con /calibrar y ajustar este umbral
#define LDR_UMBRAL_OSCURO   2000    // Por encima de este valor se considera "oscuro"

// Tiempo sin movimiento antes de apagar en modo AUTO (milisegundos)
#define TIMEOUT_SIN_MOVIMIENTO_MS  15000  // 15 segundos

// Brillo del modo AUTO (0-255)
#define BRILLO_AUTO  200

// Intervalo mínimo entre lecturas de sensores (ms) — evita lecturas ruidosas
#define INTERVALO_SENSORES_MS  200

// ════════════════════════════════════════════════
//  ESTRUCTURAS Y TIPOS
// ════════════════════════════════════════════════

// Modos de operación
enum Modo {
  MODO_AUTO,
  MODO_FIESTA,
  MODO_OFF
};

// Estado completo del sistema
struct EstadoSistema {
  Modo     modo;
  uint8_t  r, g, b;          // Color actual (0-255 cada canal)
  uint8_t  brillo;            // Intensidad global 0-100 (%)
  int      valorLDR;          // Lectura cruda del LDR (0-4095)
  bool     estaOscuro;        // true si LDR indica oscuridad
  bool     hayMovimiento;     // true si PIR detecta presencia
  bool     ledEncendido;      // true si hay salida de luz activa
  unsigned long ultimoMovimiento;   // millis() del último PIR=HIGH
  unsigned long ultimaLecturaMS;    // millis() de la última lectura de sensores
};

// ════════════════════════════════════════════════
//  VARIABLES GLOBALES
// ════════════════════════════════════════════════

EstadoSistema estado = {
  MODO_AUTO,    // modo inicial
  0, 0, 0,      // color apagado
  100,          // brillo al 100%
  0,            // LDR sin leer
  false,        // no oscuro
  false,        // sin movimiento
  false,        // LED apagado
  0,            // sin último movimiento
  0             // sin última lectura
};

WebServer servidor(80);

// ════════════════════════════════════════════════
//  HTML DE LA APP WEB (almacenado en Flash)
// ════════════════════════════════════════════════
// PROGMEM evita que ocupe RAM dinámica (que es limitada en ESP32)

const char HTML_APP[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Smart Lighting</title>
  <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700;900&family=Rajdhani:wght@300;400;600&display=swap" rel="stylesheet"/>
  <style>
    :root {
      --bg:        #080c14;
      --surface:   #0e1420;
      --surface2:  #141c2e;
      --accent:    #00d4ff;
      --accent2:   #7b2fff;
      --gold:      #ffd166;
      --danger:    #ff4757;
      --success:   #2ed573;
      --text:      #e8eaf0;
      --muted:     #5a6478;
      --border:    rgba(0,212,255,0.15);
      --glow:      0 0 20px rgba(0,212,255,0.3);
      --glow-lg:   0 0 40px rgba(0,212,255,0.2), 0 0 80px rgba(123,47,255,0.1);
    }

    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

    body {
      font-family: 'Rajdhani', sans-serif;
      background: var(--bg);
      color: var(--text);
      min-height: 100vh;
      overflow-x: hidden;
    }

    /* Fondo con rejilla animada */
    body::before {
      content: '';
      position: fixed;
      inset: 0;
      background-image:
        linear-gradient(rgba(0,212,255,0.03) 1px, transparent 1px),
        linear-gradient(90deg, rgba(0,212,255,0.03) 1px, transparent 1px);
      background-size: 40px 40px;
      pointer-events: none;
      z-index: 0;
    }

    .container {
      position: relative;
      z-index: 1;
      max-width: 420px;
      margin: 0 auto;
      padding: 24px 16px 40px;
    }

    /* ── HEADER ── */
    header {
      text-align: center;
      margin-bottom: 28px;
    }

    .logo-ring {
      width: 80px; height: 80px;
      margin: 0 auto 16px;
      border-radius: 50%;
      border: 2px solid var(--accent);
      display: flex; align-items: center; justify-content: center;
      box-shadow: var(--glow);
      animation: pulse-ring 3s ease-in-out infinite;
      position: relative;
    }
    .logo-ring::before {
      content: '';
      position: absolute;
      inset: 6px;
      border-radius: 50%;
      border: 1px solid rgba(0,212,255,0.3);
    }
    .logo-icon { font-size: 32px; }

    @keyframes pulse-ring {
      0%, 100% { box-shadow: 0 0 20px rgba(0,212,255,0.3); }
      50%       { box-shadow: 0 0 40px rgba(0,212,255,0.6), 0 0 60px rgba(123,47,255,0.3); }
    }

    h1 {
      font-family: 'Orbitron', monospace;
      font-size: 20px;
      font-weight: 900;
      letter-spacing: 3px;
      text-transform: uppercase;
      background: linear-gradient(135deg, var(--accent), var(--accent2));
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      background-clip: text;
    }
    .subtitle {
      font-size: 12px;
      letter-spacing: 2px;
      color: var(--muted);
      text-transform: uppercase;
      margin-top: 4px;
    }

    /* ── TARJETA DE ESTADO ── */
    .status-card {
      background: var(--surface);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 20px;
      margin-bottom: 20px;
      position: relative;
      overflow: hidden;
    }
    .status-card::before {
      content: '';
      position: absolute;
      top: 0; left: 0; right: 0;
      height: 2px;
      background: linear-gradient(90deg, var(--accent), var(--accent2));
    }

    .status-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 8px 0;
      border-bottom: 1px solid rgba(255,255,255,0.05);
      font-size: 15px;
    }
    .status-row:last-child { border-bottom: none; padding-bottom: 0; }
    .status-label { color: var(--muted); font-weight: 600; letter-spacing: 1px; font-size: 12px; text-transform: uppercase; }
    .status-value { font-weight: 600; }
    .tag {
      display: inline-block;
      padding: 3px 10px;
      border-radius: 20px;
      font-size: 12px;
      font-weight: 700;
      letter-spacing: 1px;
      text-transform: uppercase;
    }
    .tag-on    { background: rgba(46,213,115,0.15); color: var(--success); border: 1px solid rgba(46,213,115,0.3); }
    .tag-off   { background: rgba(255,71,87,0.15);  color: var(--danger);  border: 1px solid rgba(255,71,87,0.3); }
    .tag-auto  { background: rgba(0,212,255,0.15);  color: var(--accent);  border: 1px solid rgba(0,212,255,0.3); }

    /* Indicador de color actual */
    .color-preview {
      width: 100%;
      height: 8px;
      border-radius: 4px;
      margin-top: 14px;
      transition: background 0.5s ease;
      border: 1px solid rgba(255,255,255,0.1);
    }

    /* ── BOTONES DE MODO ── */
    .section-label {
      font-family: 'Orbitron', monospace;
      font-size: 10px;
      letter-spacing: 3px;
      color: var(--muted);
      text-transform: uppercase;
      margin-bottom: 12px;
      display: flex;
      align-items: center;
      gap: 10px;
    }
    .section-label::after {
      content: '';
      flex: 1;
      height: 1px;
      background: var(--border);
    }

    .modos-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 10px;
      margin-bottom: 10px;
    }

    .btn-modo {
      background: var(--surface);
      border: 1px solid var(--border);
      border-radius: 12px;
      padding: 16px 12px;
      color: var(--text);
      font-family: 'Rajdhani', sans-serif;
      font-size: 15px;
      font-weight: 600;
      letter-spacing: 1px;
      cursor: pointer;
      transition: all 0.2s ease;
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 6px;
    }
    .btn-modo .icon { font-size: 24px; }
    .btn-modo:hover {
      border-color: var(--accent);
      background: var(--surface2);
      transform: translateY(-2px);
      box-shadow: var(--glow);
    }
    .btn-modo.activo {
      border-color: var(--accent);
      background: rgba(0,212,255,0.08);
      box-shadow: var(--glow);
      color: var(--accent);
    }
    .btn-modo.activo-fiesta {
      border-color: var(--accent2);
      background: rgba(123,47,255,0.1);
      box-shadow: 0 0 20px rgba(123,47,255,0.3);
      color: var(--accent2);
      animation: fiesta-btn 1s ease-in-out infinite alternate;
    }
    @keyframes fiesta-btn {
      from { border-color: var(--accent2); }
      to   { border-color: var(--gold); box-shadow: 0 0 20px rgba(255,209,102,0.3); }
    }

    .btn-off {
      width: 100%;
      background: rgba(255,71,87,0.08);
      border: 1px solid rgba(255,71,87,0.3);
      border-radius: 12px;
      padding: 16px;
      color: var(--danger);
      font-family: 'Orbitron', monospace;
      font-size: 13px;
      font-weight: 700;
      letter-spacing: 2px;
      cursor: pointer;
      transition: all 0.2s ease;
      margin-bottom: 20px;
    }
    .btn-off:hover {
      background: rgba(255,71,87,0.15);
      box-shadow: 0 0 20px rgba(255,71,87,0.2);
      transform: translateY(-1px);
    }
    .btn-off.activo {
      background: rgba(255,71,87,0.2);
      box-shadow: 0 0 30px rgba(255,71,87,0.3);
    }

    /* ── COLOR MANUAL ── */
    .color-section {
      background: var(--surface);
      border: 1px solid var(--border);
      border-radius: 16px;
      padding: 20px;
      margin-bottom: 20px;
    }

    .sliders { display: flex; flex-direction: column; gap: 12px; margin-top: 14px; }

    .slider-row {
      display: grid;
      grid-template-columns: 28px 1fr 40px;
      align-items: center;
      gap: 10px;
    }
    .slider-letter {
      font-family: 'Orbitron', monospace;
      font-size: 11px;
      font-weight: 700;
    }
    .slider-letter.r { color: #ff6b6b; }
    .slider-letter.g { color: #51cf66; }
    .slider-letter.b { color: #339af0; }

    input[type=range] {
      -webkit-appearance: none;
      width: 100%;
      height: 6px;
      border-radius: 3px;
      outline: none;
      cursor: pointer;
    }
    input[type=range].r { background: linear-gradient(90deg, #1a0000, #ff6b6b); }
    input[type=range].g { background: linear-gradient(90deg, #001a00, #51cf66); }
    input[type=range].b { background: linear-gradient(90deg, #00001a, #339af0); }
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 18px; height: 18px;
      border-radius: 50%;
      background: white;
      box-shadow: 0 2px 6px rgba(0,0,0,0.4);
      border: 2px solid rgba(255,255,255,0.3);
    }

    .slider-val {
      font-family: 'Orbitron', monospace;
      font-size: 11px;
      color: var(--muted);
      text-align: right;
    }

    .btn-aplicar {
      width: 100%;
      margin-top: 14px;
      padding: 12px;
      background: linear-gradient(135deg, var(--accent), var(--accent2));
      border: none;
      border-radius: 10px;
      color: white;
      font-family: 'Orbitron', monospace;
      font-size: 12px;
      font-weight: 700;
      letter-spacing: 2px;
      cursor: pointer;
      transition: all 0.2s;
    }
    .btn-aplicar:hover {
      opacity: 0.85;
      transform: translateY(-1px);
      box-shadow: var(--glow-lg);
    }

    /* ── FOOTER / CRÉDITOS ── */
    footer {
      text-align: center;
      margin-top: 24px;
      padding-top: 20px;
      border-top: 1px solid var(--border);
    }
    .footer-title {
      font-family: 'Orbitron', monospace;
      font-size: 9px;
      letter-spacing: 3px;
      color: var(--muted);
      text-transform: uppercase;
      margin-bottom: 10px;
    }
    .integrantes {
      display: flex;
      flex-direction: column;
      gap: 4px;
    }
    .integrante {
      font-size: 12px;
      color: #3d4a5e;
      letter-spacing: 0.5px;
      transition: color 0.2s;
    }
    .integrante:hover { color: var(--accent); }

    /* ── INDICADOR DE CONEXIÓN ── */
    .conexion {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 6px;
      margin-bottom: 20px;
      font-size: 11px;
      color: var(--muted);
      letter-spacing: 1px;
    }
    .dot {
      width: 7px; height: 7px;
      border-radius: 50%;
      background: var(--success);
      animation: blink 2s ease-in-out infinite;
    }
    @keyframes blink {
      0%, 100% { opacity: 1; }
      50%       { opacity: 0.3; }
    }
  </style>
</head>
<body>
<div class="container">

  <!-- Header -->
  <header>
    <div class="logo-ring"><span class="logo-icon">💡</span></div>
    <h1>Smart Lighting</h1>
    <p class="subtitle">Sistema de Iluminación Inteligente</p>
  </header>

  <!-- Indicador de conexión -->
  <div class="conexion">
    <div class="dot"></div>
    <span>CONECTADO · ESP32 WROOM-32</span>
  </div>

  <!-- Tarjeta de estado — sensores con indicadores visuales -->
  <div class="status-card">
    <div class="status-row">
      <span class="status-label">Modo</span>
      <span class="status-value" id="st-modo"><span class="tag tag-auto">AUTO</span></span>
    </div>
    <div class="status-row">
      <span class="status-label">Luz ambiente</span>
      <span class="status-value" id="st-ldr">—</span>
    </div>
    <div class="status-row">
      <span class="status-label">Movimiento PIR</span>
      <span class="status-value" id="st-pir">—</span>
    </div>
    <div class="status-row">
      <span class="status-label">LED</span>
      <span class="status-value" id="st-led">—</span>
    </div>
    <div class="color-preview" id="color-preview"></div>
  </div>

  <!-- Modos — solo Auto, Fiesta y Off -->
  <div class="section-label">Modos de operación</div>
  <div class="modos-grid" style="grid-template-columns:1fr 1fr;">
    <button class="btn-modo activo" id="btn-auto" onclick="setModo('auto')">
      <span class="icon">🔄</span>Auto
    </button>
    <button class="btn-modo" id="btn-fiesta" onclick="setModo('fiesta')">
      <span class="icon">🎉</span>Fiesta
    </button>
  </div>
  <button class="btn-off" id="btn-off" onclick="setModo('off')">⭕ APAGAR</button>

  <!-- Intensidad global -->
  <div class="section-label">Intensidad</div>
  <div class="color-section">
    <div class="sliders">
      <div class="slider-row">
        <span class="slider-letter" style="color:#ffd166">☀</span>
        <input type="range" style="background:linear-gradient(90deg,#1a1a00,#ffd166)"
               id="sl-brillo" min="0" max="100" value="100"
               oninput="previewBrillo()"/>
        <span class="slider-val" id="val-brillo">100%</span>
      </div>
    </div>
    <button class="btn-aplicar" onclick="aplicarBrillo()">▶ APLICAR INTENSIDAD</button>
  </div>

  <!-- Control de color manual -->
  <div class="section-label">Color manual</div>
  <div class="color-section">
    <div class="sliders">
      <div class="slider-row">
        <span class="slider-letter r">R</span>
        <input type="range" class="r" id="sl-r" min="0" max="255" value="255" oninput="actualizarPreviewManual()"/>
        <span class="slider-val" id="val-r">255</span>
      </div>
      <div class="slider-row">
        <span class="slider-letter g">G</span>
        <input type="range" class="g" id="sl-g" min="0" max="255" value="0" oninput="actualizarPreviewManual()"/>
        <span class="slider-val" id="val-g">0</span>
      </div>
      <div class="slider-row">
        <span class="slider-letter b">B</span>
        <input type="range" class="b" id="sl-b" min="0" max="255" value="0" oninput="actualizarPreviewManual()"/>
        <span class="slider-val" id="val-b">0</span>
      </div>
    </div>
    <button class="btn-aplicar" onclick="aplicarColor()">▶ APLICAR COLOR</button>
  </div>

  <!-- Footer / Créditos -->
  <footer>
    <div class="footer-title">Integrantes del equipo</div>
    <div class="integrantes">
      <span class="integrante">Luis Alejandro Alcocer Marín</span>
      <span class="integrante">Luis Antonio Padilla Mondragón</span>
      <span class="integrante">Juan Luis Ramírez Hernández</span>
      <span class="integrante">Uriel Everardo Sánchez Rangel</span>
    </div>
  </footer>

</div>

<script>
  let modoActivo = 'auto';

  // ── Preview slider RGB ──
  function actualizarPreviewManual() {
    document.getElementById('val-r').textContent = document.getElementById('sl-r').value;
    document.getElementById('val-g').textContent = document.getElementById('sl-g').value;
    document.getElementById('val-b').textContent = document.getElementById('sl-b').value;
  }

  // ── Preview slider de brillo ──
  function previewBrillo() {
    document.getElementById('val-brillo').textContent =
      document.getElementById('sl-brillo').value + '%';
  }

  // ── Cambiar modo ──
  async function setModo(m) {
    await fetch('/modo?m=' + m);
    modoActivo = m;
    await actualizarStatus();
  }

  // ── Aplicar color manual ──
  async function aplicarColor() {
    const r = document.getElementById('sl-r').value;
    const g = document.getElementById('sl-g').value;
    const b = document.getElementById('sl-b').value;
    await fetch('/color?r=' + r + '&g=' + g + '&b=' + b);
    await actualizarStatus();
  }

  // ── Aplicar brillo ──
  async function aplicarBrillo() {
    const v = document.getElementById('sl-brillo').value;
    await fetch('/brillo?v=' + v);
    await actualizarStatus();
  }

  // ── Resaltar botón activo ──
  function actualizarBotones(m) {
    ['auto','fiesta','off'].forEach(id => {
      const btn = document.getElementById('btn-' + id);
      if (!btn) return;
      btn.className = id === 'off' ? 'btn-off' : 'btn-modo';
      if (id === m) {
        if (id === 'off')    btn.classList.add('activo');
        else if (id === 'fiesta') btn.classList.add('activo-fiesta');
        else btn.classList.add('activo');
      }
    });
  }

  // ── Obtener estado del ESP32 ──
  async function actualizarStatus() {
    try {
      const res  = await fetch('/status');
      const data = await res.json();

      // Modo
      const modoLabels = { auto:'AUTO', fiesta:'FIESTA', off:'OFF' };
      const modoClases = { auto:'tag-auto', fiesta:'tag-on', off:'tag-off' };
      document.getElementById('st-modo').innerHTML =
        '<span class="tag ' + (modoClases[data.modo]||'tag-auto') + '">'
        + (modoLabels[data.modo]||data.modo) + '</span>';

      // LDR — con barra de nivel visual
      const pct = Math.round((data.ldr / 4095) * 100);
      const luzColor = data.oscuro ? '#ffd166' : '#00d4ff';
      const luzLabel = data.oscuro ? '🌙 Oscuro' : '☀️ Con luz';
      document.getElementById('st-ldr').innerHTML =
        '<div style="display:flex;flex-direction:column;align-items:flex-end;gap:4px">'
        + '<span style="color:' + luzColor + ';font-weight:700">' + luzLabel + '</span>'
        + '<div style="width:80px;height:5px;border-radius:3px;background:#1a2332;overflow:hidden">'
        + '<div style="width:' + pct + '%;height:100%;background:' + luzColor + ';border-radius:3px;transition:width .4s"></div>'
        + '</div>'
        + '<span style="font-size:10px;color:#5a6478">' + data.ldr + ' / 4095</span>'
        + '</div>';

      // PIR — con indicador animado cuando detecta
      if (data.pir) {
        document.getElementById('st-pir').innerHTML =
          '<span class="tag tag-on" style="animation:pulse-ring .8s ease-in-out infinite">🚶 DETECTADO</span>';
      } else {
        document.getElementById('st-pir').innerHTML =
          '<span class="tag tag-off">— Sin movimiento</span>';
      }

      // LED
      document.getElementById('st-led').innerHTML =
        data.led
          ? '<span class="tag tag-on">ENCENDIDO</span>'
          : '<span class="tag tag-off">APAGADO</span>';

      // Color preview — usa el brillo real aplicado
      const factor = data.brillo / 100;
      document.getElementById('color-preview').style.background =
        'rgb(' + Math.round(data.r*factor) + ','
               + Math.round(data.g*factor) + ','
               + Math.round(data.b*factor) + ')';

      // Sincronizar slider de brillo con el ESP32
      document.getElementById('sl-brillo').value = data.brillo;
      document.getElementById('val-brillo').textContent = data.brillo + '%';

      actualizarBotones(data.modo);

    } catch(e) {
      console.warn('Sin respuesta del ESP32:', e);
    }
  }

  actualizarStatus();
  setInterval(actualizarStatus, 2000);
</script>
</body>
</html>
)rawliteral";


// ════════════════════════════════════════════════
//  FUNCIONES DE CONTROL DE LED
// ════════════════════════════════════════════════

/**
 * Aplica un color RGB a ambos LEDs usando PWM.
 * @param r,g,b  Valores 0–255 por canal
 */
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  // Aplicar brillo global: escalar cada canal según porcentaje 0-100
  uint8_t factor = estado.brillo;
  uint8_t rB = (uint8_t)((r * factor) / 100);
  uint8_t gB = (uint8_t)((g * factor) / 100);
  uint8_t bB = (uint8_t)((b * factor) / 100);

  // Core v3.x: ledcWrite usa el PIN directamente, no el canal
  ledcWrite(PIN_R1, rB);
  ledcWrite(PIN_G1, gB);
  ledcWrite(PIN_B1, bB);
  ledcWrite(PIN_R2, rB);
  ledcWrite(PIN_G2, gB);
  ledcWrite(PIN_B2, bB);

  estado.r = r;
  estado.g = g;
  estado.b = b;
  estado.ledEncendido = (rB || gB || bB);
}

/** Apaga todos los LEDs */
void apagar() {
  setColor(0, 0, 0);
}

/**
 * Genera un color del arcoíris según una posición 0–255.
 * Útil para el modo Fiesta.
 */
void colorRueda(uint8_t pos, uint8_t &r, uint8_t &g, uint8_t &b) {
  if (pos < 85) {
    r = pos * 3; g = 255 - pos * 3; b = 0;
  } else if (pos < 170) {
    pos -= 85;
    r = 255 - pos * 3; g = 0; b = pos * 3;
  } else {
    pos -= 170;
    r = 0; g = pos * 3; b = 255 - pos * 3;
  }
}


// ════════════════════════════════════════════════
//  LÓGICA DE MODOS
// ════════════════════════════════════════════════

/**
 * MODO AUTO: el núcleo del sistema inteligente.
 * Lee LDR y PIR, decide si encender o apagar.
 * Se llama cada iteración del loop().
 */
void ejecutarModoAuto() {
  unsigned long ahora = millis();

  // Limitar frecuencia de lectura para estabilidad
  if (ahora - estado.ultimaLecturaMS < INTERVALO_SENSORES_MS) return;
  estado.ultimaLecturaMS = ahora;

  // Leer sensores
  estado.valorLDR   = analogRead(PIN_LDR);
  estado.estaOscuro = (estado.valorLDR > LDR_UMBRAL_OSCURO);
  estado.hayMovimiento = digitalRead(PIN_PIR);

  // Actualizar timestamp de último movimiento
  if (estado.hayMovimiento) {
    estado.ultimoMovimiento = ahora;
  }

  // Lógica de encendido:
  //   1. Debe estar oscuro (no tiene sentido encender de día)
  //   2. Debe haber habido movimiento en los últimos TIMEOUT ms
  bool dentroDeTiempo = (ahora - estado.ultimoMovimiento) < TIMEOUT_SIN_MOVIMIENTO_MS;

  if (estado.estaOscuro && dentroDeTiempo) {
    // Blanco cálido — agradable para iluminación general nocturna
    setColor(BRILLO_AUTO, (uint8_t)(BRILLO_AUTO * 0.7), (uint8_t)(BRILLO_AUTO * 0.3));
  } else {
    apagar();
  }
}

/**
 * MODO FIESTA: ciclo de colores animado.
 * Usa server.handleClient() dentro del loop para no bloquear el WiFi.
 */
void ejecutarModoFiesta() {
  static uint8_t paso = 0;
  static unsigned long ultimoCambio = 0;

  if (millis() - ultimoCambio < 30) return;  // ~33 fps
  ultimoCambio = millis();

  uint8_t r, g, b;
  colorRueda(paso, r, g, b);
  setColor(r, g, b);

  paso++;  // desborda naturalmente de 255 → 0
}


// ════════════════════════════════════════════════
//  MANEJADORES HTTP
// ════════════════════════════════════════════════

/** GET / → Sirve la app web */
void handleRoot() {
  servidor.send_P(200, "text/html", HTML_APP);
}

/** GET /modo?m=auto|fiesta|off → Cambia el modo */
void handleModo() {
  if (!servidor.hasArg("m")) {
    servidor.send(400, "text/plain", "Falta parametro m");
    return;
  }

  String m = servidor.arg("m");

  if      (m == "auto")   { estado.modo = MODO_AUTO;   }
  else if (m == "fiesta") { estado.modo = MODO_FIESTA; }
  else if (m == "off")    { estado.modo = MODO_OFF; apagar(); }
  else {
    servidor.send(400, "text/plain", "Modo desconocido");
    return;
  }

  servidor.send(200, "text/plain", "ok");
}

/** GET /color?r=X&g=X&b=X → Color manual (fuerza modo OFF para no pisarse) */
void handleColor() {
  if (!servidor.hasArg("r") || !servidor.hasArg("g") || !servidor.hasArg("b")) {
    servidor.send(400, "text/plain", "Faltan parametros r, g, b");
    return;
  }

  uint8_t r = constrain(servidor.arg("r").toInt(), 0, 255);
  uint8_t g = constrain(servidor.arg("g").toInt(), 0, 255);
  uint8_t b = constrain(servidor.arg("b").toInt(), 0, 255);

  // Salir del modo auto/fiesta para que no pise el color manual
  estado.modo = MODO_OFF;
  setColor(r, g, b);

  servidor.send(200, "text/plain", "ok");
}

/** GET /status → JSON con estado completo del sistema */
void handleStatus() {
  // Leer sensores siempre frescos para la UI
  estado.valorLDR      = analogRead(PIN_LDR);
  estado.estaOscuro    = (estado.valorLDR > LDR_UMBRAL_OSCURO);
  estado.hayMovimiento = digitalRead(PIN_PIR);

  String modos[] = {"auto", "fiesta", "off"};

  String json = "{";
  json += "\"modo\":\"" + modos[estado.modo] + "\",";
  json += "\"ldr\":"     + String(estado.valorLDR) + ",";
  json += "\"oscuro\":"  + String(estado.estaOscuro    ? "true" : "false") + ",";
  json += "\"pir\":"     + String(estado.hayMovimiento ? "true" : "false") + ",";
  json += "\"led\":"     + String(estado.ledEncendido  ? "true" : "false") + ",";
  json += "\"brillo\":"  + String(estado.brillo) + ",";
  json += "\"r\":"       + String(estado.r) + ",";
  json += "\"g\":"       + String(estado.g) + ",";
  json += "\"b\":"       + String(estado.b);
  json += "}";

  servidor.send(200, "application/json", json);
}

/**
 * GET /calibrar → Valores crudos para calibrar el umbral del LDR.
 * Útil durante la puesta a punto del hardware.
 */
void handleCalibrar() {
  int ldr = analogRead(PIN_LDR);
  bool pir = digitalRead(PIN_PIR);

  String json = "{";
  json += "\"ldr_raw\":"     + String(ldr) + ",";
  json += "\"ldr_umbral\":"  + String(LDR_UMBRAL_OSCURO) + ",";
  json += "\"ldr_oscuro\":"  + String(ldr > LDR_UMBRAL_OSCURO ? "true" : "false") + ",";
  json += "\"pir_raw\":"     + String(pir ? 1 : 0);
  json += "}";

  servidor.send(200, "application/json", json);
}

/** GET /brillo?v=0-100 → Ajusta la intensidad global sin cambiar el color */
void handleBrillo() {
  if (!servidor.hasArg("v")) {
    servidor.send(400, "text/plain", "Falta parametro v");
    return;
  }
  estado.brillo = constrain(servidor.arg("v").toInt(), 0, 100);
  // Re-aplicar el color actual con el nuevo brillo
  setColor(estado.r, estado.g, estado.b);
  servidor.send(200, "text/plain", "ok");
}

/** 404 para rutas desconocidas */
void handleNotFound() {
  servidor.send(404, "text/plain", "Ruta no encontrada");
}


// ════════════════════════════════════════════════
//  SETUP
// ════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n╔══ Smart Lighting System — ESP32 ══╗");

  // ── Configurar PWM (API Core v3.x) ──
  // ledcAttach(pin, frecuencia, resolucion) — ya no se usan canales explícitos
  // LED 1
  ledcAttach(PIN_R1, PWM_FRECUENCIA, PWM_RESOLUCION);
  ledcAttach(PIN_G1, PWM_FRECUENCIA, PWM_RESOLUCION);
  ledcAttach(PIN_B1, PWM_FRECUENCIA, PWM_RESOLUCION);

  // LED 2
  ledcAttach(PIN_R2, PWM_FRECUENCIA, PWM_RESOLUCION);
  ledcAttach(PIN_G2, PWM_FRECUENCIA, PWM_RESOLUCION);
  ledcAttach(PIN_B2, PWM_FRECUENCIA, PWM_RESOLUCION);

  // ── Configurar sensores ──
  pinMode(PIN_PIR, INPUT);
  // PIN_LDR es analógico, no necesita pinMode

  // ── Secuencia de inicio (indica que el sistema arrancó) ──
  Serial.println("║ Iniciando secuencia de arranque...");
  setColor(255, 0, 0);   delay(300);
  setColor(0, 255, 0);   delay(300);
  setColor(0, 0, 255);   delay(300);
  apagar();

  // ── Conexión WiFi ──
  Serial.print("║ Conectando a WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 30) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n║ ⚠️  No se pudo conectar al WiFi. Reiniciando...");
    ESP.restart();
  }

  Serial.println("\n║ ✅ WiFi conectado");
  Serial.print("║ IP: ");
  Serial.println(WiFi.localIP());

  // ── Registrar rutas HTTP ──
  servidor.on("/",         HTTP_GET, handleRoot);
  servidor.on("/modo",     HTTP_GET, handleModo);
  servidor.on("/color",    HTTP_GET, handleColor);
  servidor.on("/brillo",   HTTP_GET, handleBrillo);
  servidor.on("/status",   HTTP_GET, handleStatus);
  servidor.on("/calibrar", HTTP_GET, handleCalibrar);
  servidor.onNotFound(handleNotFound);
  servidor.begin();

  Serial.println("║ ✅ Servidor HTTP iniciado");
  Serial.println("╚════════════════════════════════════╝\n");
}


// ════════════════════════════════════════════════
//  LOOP PRINCIPAL
// ════════════════════════════════════════════════

void loop() {
  // Atender peticiones HTTP entrantes
  servidor.handleClient();

  // Ejecutar lógica según el modo activo
  switch (estado.modo) {
    case MODO_AUTO:
      ejecutarModoAuto();
      break;

    case MODO_FIESTA:
      ejecutarModoFiesta();
      break;

    // DÍA, NOCHE y OFF se configuran una vez al cambiar de modo
    // y no necesitan lógica recurrente
    default:
      break;
  }
}
