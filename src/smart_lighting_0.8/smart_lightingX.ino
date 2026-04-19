#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID     = "Mega_2.4G_6017";
const char* WIFI_PASSWORD = "UPdtCbKK";

#define PIN_R   25
#define PIN_G   26
#define PIN_B   27
#define PIN_PIR 14
#define PIN_LDR 35

#define TIMEOUT_MS 10000

enum Modo { AUTO, FIESTA, OFF };

struct Estado {
  Modo    modo   = AUTO;
  uint8_t r=0, g=0, b=0;
  uint8_t brillo = 100;
  bool    ledOn  = false;
  unsigned long ultimoMovimiento = 0;
  unsigned long ultimaLectura    = 0;
};

Estado estado;
WebServer server(80);

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(PIN_R, r * estado.brillo / 100);
  ledcWrite(PIN_G, g * estado.brillo / 100);
  ledcWrite(PIN_B, b * estado.brillo / 100);
  estado.r = r; estado.g = g; estado.b = b;
  estado.ledOn = (r || g || b);
}

void apagar() { setColor(0, 0, 0); }

void loopAuto() {
  unsigned long now = millis();
  if (now - estado.ultimaLectura < 200) return;
  estado.ultimaLectura = now;

  bool oscuro = digitalRead(PIN_LDR) == LOW;
  bool mov    = digitalRead(PIN_PIR) == HIGH;
  if (mov) estado.ultimoMovimiento = now;

  bool dentroDeTiempo = (now - estado.ultimoMovimiento < TIMEOUT_MS);

  if (!oscuro && dentroDeTiempo) {
    setColor(200, 140, 60);
  } else {
    apagar();
  }
}

void loopFiesta() {
  static uint8_t paso = 0;
  static unsigned long t = 0;
  if (millis() - t < 30) return;
  t = millis();
  uint8_t r, g, b;
  if (paso < 85)       { r=paso*3;        g=255-paso*3;  b=0; }
  else if (paso < 170) { uint8_t p=paso-85;  r=255-p*3; g=0;    b=p*3; }
  else                 { uint8_t p=paso-170; r=0;        g=p*3;  b=255-p*3; }
  setColor(r, g, b);
  paso++;
}

const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="es"><head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width,initial-scale=1"/>
<title>Smart Lighting</title>
<link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@700;900&family=Rajdhani:wght@400;600&display=swap" rel="stylesheet"/>
<style>
:root{--bg:#080c14;--s:#0e1420;--a:#00d4ff;--a2:#7b2fff;--g:#2ed573;--d:#ff4757;--m:#5a6478;--brd:rgba(0,212,255,0.15)}
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:'Rajdhani',sans-serif;background:var(--bg);color:#e8eaf0;min-height:100vh}
body::before{content:'';position:fixed;inset:0;background-image:linear-gradient(rgba(0,212,255,0.03) 1px,transparent 1px),linear-gradient(90deg,rgba(0,212,255,0.03) 1px,transparent 1px);background-size:40px 40px;pointer-events:none}
.wrap{position:relative;z-index:1;max-width:400px;margin:0 auto;padding:24px 16px 40px}
header{text-align:center;margin-bottom:24px}
.ring{width:72px;height:72px;margin:0 auto 14px;border-radius:50%;border:2px solid var(--a);display:flex;align-items:center;justify-content:center;font-size:28px;animation:pulse 3s ease-in-out infinite}
@keyframes pulse{0%,100%{box-shadow:0 0 20px rgba(0,212,255,0.3)}50%{box-shadow:0 0 40px rgba(0,212,255,0.6)}}
h1{font-family:'Orbitron',monospace;font-size:18px;letter-spacing:3px;background:linear-gradient(135deg,var(--a),var(--a2));-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text}
.sub{font-size:11px;letter-spacing:2px;color:var(--m);margin-top:4px}
.chip{display:flex;align-items:center;justify-content:center;gap:6px;font-size:11px;color:var(--m);letter-spacing:1px;margin-bottom:18px}
.dot{width:7px;height:7px;border-radius:50%;background:var(--g);animation:blink 2s infinite}
@keyframes blink{0%,100%{opacity:1}50%{opacity:0.3}}
.card{background:var(--s);border:1px solid var(--brd);border-radius:14px;padding:18px;margin-bottom:18px;position:relative;overflow:hidden}
.card::before{content:'';position:absolute;top:0;left:0;right:0;height:2px;background:linear-gradient(90deg,var(--a),var(--a2))}
.row{display:flex;justify-content:space-between;align-items:center;padding:7px 0;border-bottom:1px solid rgba(255,255,255,0.05);font-size:14px}
.row:last-child{border:none;padding-bottom:0}
.lbl{font-size:11px;color:var(--m);letter-spacing:1px;text-transform:uppercase;font-weight:600}
.tag{display:inline-block;padding:3px 9px;border-radius:20px;font-size:11px;font-weight:700;letter-spacing:1px;text-transform:uppercase}
.ton{background:rgba(46,213,115,0.15);color:var(--g);border:1px solid rgba(46,213,115,0.3)}
.toff{background:rgba(255,71,87,0.15);color:var(--d);border:1px solid rgba(255,71,87,0.3)}
.tau{background:rgba(0,212,255,0.15);color:var(--a);border:1px solid rgba(0,212,255,0.3)}
.preview{width:100%;height:8px;border-radius:4px;margin-top:12px;border:1px solid rgba(255,255,255,0.1);transition:background .5s}
.sec{font-family:'Orbitron',monospace;font-size:10px;letter-spacing:3px;color:var(--m);text-transform:uppercase;margin-bottom:10px;display:flex;align-items:center;gap:8px}
.sec::after{content:'';flex:1;height:1px;background:var(--brd)}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-bottom:10px}
.btn{background:var(--s);border:1px solid var(--brd);border-radius:12px;padding:14px;color:#e8eaf0;font-family:'Rajdhani',sans-serif;font-size:14px;font-weight:600;cursor:pointer;transition:all .2s;display:flex;flex-direction:column;align-items:center;gap:5px}
.btn .ic{font-size:22px}
.btn:hover{border-color:var(--a);background:#141c2e;transform:translateY(-2px)}
.btn.active{border-color:var(--a);background:rgba(0,212,255,0.08);color:var(--a)}
.btn.activef{border-color:var(--a2);background:rgba(123,47,255,0.1);color:var(--a2);animation:fb 1s infinite alternate}
@keyframes fb{from{border-color:var(--a2)}to{border-color:#ffd166}}
.boff{width:100%;background:rgba(255,71,87,0.08);border:1px solid rgba(255,71,87,0.3);border-radius:12px;padding:14px;color:var(--d);font-family:'Orbitron',monospace;font-size:12px;font-weight:700;letter-spacing:2px;cursor:pointer;transition:all .2s;margin-bottom:18px}
.boff:hover{background:rgba(255,71,87,0.18);transform:translateY(-1px)}
.boff.active{background:rgba(255,71,87,0.2)}
.panel{background:var(--s);border:1px solid var(--brd);border-radius:14px;padding:18px;margin-bottom:18px}
.srow{display:grid;grid-template-columns:26px 1fr 44px;align-items:center;gap:10px;margin-bottom:10px}
.slbl{font-family:'Orbitron',monospace;font-size:11px;font-weight:700}
.sval{font-family:'Orbitron',monospace;font-size:11px;color:var(--m);text-align:right}
input[type=range]{-webkit-appearance:none;width:100%;height:6px;border-radius:3px;outline:none;cursor:pointer}
input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:17px;height:17px;border-radius:50%;background:#fff;box-shadow:0 2px 5px rgba(0,0,0,0.4)}
.apply{width:100%;margin-top:10px;padding:11px;background:linear-gradient(135deg,var(--a),var(--a2));border:none;border-radius:10px;color:#fff;font-family:'Orbitron',monospace;font-size:11px;font-weight:700;letter-spacing:2px;cursor:pointer;transition:all .2s}
.apply:hover{opacity:.85;transform:translateY(-1px)}
footer{text-align:center;margin-top:20px;padding-top:18px;border-top:1px solid var(--brd)}
.ft{font-family:'Orbitron',monospace;font-size:9px;letter-spacing:3px;color:var(--m);margin-bottom:8px}
.names{display:flex;flex-direction:column;gap:3px}
.name{font-size:12px;color:#3d4a5e;transition:color .2s}
.name:hover{color:var(--a)}
</style></head>
<body><div class="wrap">
<header>
  <div class="ring">💡</div>
  <h1>SMART LIGHTING</h1>
  <p class="sub">Sistema de Iluminación Inteligente</p>
</header>
<div class="chip"><div class="dot"></div><span>CONECTADO · ESP32 WROOM-32</span></div>
<div class="card">
  <div class="row"><span class="lbl">Modo</span><span id="st-modo"><span class="tag tau">AUTO</span></span></div>
  <div class="row"><span class="lbl">Luz ambiente</span><span id="st-ldr">—</span></div>
  <div class="row"><span class="lbl">Movimiento PIR</span><span id="st-pir">—</span></div>
  <div class="row"><span class="lbl">LED</span><span id="st-led">—</span></div>
  <div class="preview" id="preview"></div>
</div>
<div class="sec">Modos</div>
<div class="grid">
  <button class="btn active" id="btn-auto" onclick="setModo('auto')"><span class="ic">🔄</span>Auto</button>
  <button class="btn" id="btn-fiesta" onclick="setModo('fiesta')"><span class="ic">🎉</span>Fiesta</button>
</div>
<button class="boff" id="btn-off" onclick="setModo('off')">⭕ APAGAR</button>
<div class="sec">Intensidad</div>
<div class="panel">
  <div class="srow">
    <span class="slbl" style="color:#ffd166">☀</span>
    <input type="range" id="sl-br" min="0" max="100" value="100"
           style="background:linear-gradient(90deg,#1a1a00,#ffd166)"
           oninput="document.getElementById('vbr').textContent=this.value+'%'"/>
    <span class="sval" id="vbr">100%</span>
  </div>
  <button class="apply" onclick="enviar('/brillo?v='+document.getElementById('sl-br').value)">▶ APLICAR</button>
</div>
<div class="sec">Color manual</div>
<div class="panel">
  <div class="srow">
    <span class="slbl" style="color:#ff6b6b">R</span>
    <input type="range" id="sl-r" min="0" max="255" value="255"
           style="background:linear-gradient(90deg,#1a0000,#ff6b6b)"
           oninput="document.getElementById('vr').textContent=this.value"/>
    <span class="sval" id="vr">255</span>
  </div>
  <div class="srow">
    <span class="slbl" style="color:#51cf66">G</span>
    <input type="range" id="sl-g" min="0" max="255" value="0"
           style="background:linear-gradient(90deg,#001a00,#51cf66)"
           oninput="document.getElementById('vg').textContent=this.value"/>
    <span class="sval" id="vg">0</span>
  </div>
  <div class="srow">
    <span class="slbl" style="color:#339af0">B</span>
    <input type="range" id="sl-b" min="0" max="255" value="0"
           style="background:linear-gradient(90deg,#00001a,#339af0)"
           oninput="document.getElementById('vb').textContent=this.value"/>
    <span class="sval" id="vb">0</span>
  </div>
  <button class="apply" onclick="aplicarColor()">▶ APLICAR COLOR</button>
</div>
<footer>
  <div class="ft">Integrantes del equipo</div>
  <div class="names">
    <span class="name">Luis Alejandro Alcocer Marín</span>
    <span class="name">Luis Antonio Padilla Mondragón</span>
    <span class="name">Juan Luis Ramírez Hernández</span>
    <span class="name">Uriel Everardo Sánchez Rangel</span>
  </div>
</footer>
</div>
<script>
async function enviar(url){
  await fetch(url);
  await new Promise(r=>setTimeout(r,250));
  await sync();
}
async function setModo(m){ await enviar('/modo?m='+m); }
async function aplicarColor(){
  const r=document.getElementById('sl-r').value;
  const g=document.getElementById('sl-g').value;
  const b=document.getElementById('sl-b').value;
  await enviar('/color?r='+r+'&g='+g+'&b='+b);
}
function marcaBotones(m){
  ['auto','fiesta','off'].forEach(id=>{
    const btn=document.getElementById('btn-'+id);
    if(!btn) return;
    btn.className=id==='off'?'boff':'btn';
    if(id===m){
      if(id==='off')         btn.classList.add('active');
      else if(id==='fiesta') btn.classList.add('activef');
      else                   btn.classList.add('active');
    }
  });
}
async function sync(){
  try{
    const d=await(await fetch('/status')).json();
    const ml={auto:'AUTO',fiesta:'FIESTA',off:'OFF'};
    const mc={auto:'tau',fiesta:'ton',off:'toff'};
    document.getElementById('st-modo').innerHTML='<span class="tag '+mc[d.modo]+'">'+ml[d.modo]+'</span>';
    document.getElementById('st-ldr').innerHTML=d.oscuro
      ?'<span class="tag ton">🌙 Oscuro</span>'
      :'<span class="tag toff">☀️ Con luz</span>';
    document.getElementById('st-pir').innerHTML=d.pir
      ?'<span class="tag ton">🚶 DETECTADO</span>'
      :'<span class="tag toff">— Sin movimiento</span>';
    document.getElementById('st-led').innerHTML=d.led
      ?'<span class="tag ton">ENCENDIDO</span>'
      :'<span class="tag toff">APAGADO</span>';
    const f=d.brillo/100;
    document.getElementById('preview').style.background=
      'rgb('+Math.round(d.r*f)+','+Math.round(d.g*f)+','+Math.round(d.b*f)+')';
    document.getElementById('sl-br').value=d.brillo;
    document.getElementById('vbr').textContent=d.brillo+'%';
    marcaBotones(d.modo);
  }catch(e){}
}
sync();
setInterval(sync,2000);
</script>
</body></html>
)rawliteral";

void handleRoot() { server.send_P(200, "text/html", HTML); }

void handleModo() {
  String m = server.arg("m");
  if      (m == "auto")   { estado.modo = AUTO; }
  else if (m == "fiesta") { estado.modo = FIESTA; }
  else if (m == "off")    { estado.modo = OFF; apagar(); }
  server.send(200, "text/plain", "ok");
}

void handleColor() {
  uint8_t r = constrain(server.arg("r").toInt(), 0, 255);
  uint8_t g = constrain(server.arg("g").toInt(), 0, 255);
  uint8_t b = constrain(server.arg("b").toInt(), 0, 255);
  estado.modo = OFF;
  setColor(r, g, b);
  server.send(200, "text/plain", "ok");
}

void handleBrillo() {
  estado.brillo = constrain(server.arg("v").toInt(), 0, 100);
  setColor(estado.r, estado.g, estado.b);
  server.send(200, "text/plain", "ok");
}

void handleStatus() {
  bool oscuro = digitalRead(PIN_LDR) != LOW;
  bool pir    = digitalRead(PIN_PIR) == HIGH;
  String modos[] = {"auto", "fiesta", "off"};
  String j = "{";
  j += "\"modo\":\"";   j += modos[estado.modo]; j += "\",";
  j += "\"oscuro\":";   j += oscuro ? "true" : "false"; j += ",";
  j += "\"pir\":";      j += pir    ? "true" : "false"; j += ",";
  j += "\"led\":";      j += estado.ledOn ? "true" : "false"; j += ",";
  j += "\"brillo\":";   j += estado.brillo; j += ",";
  j += "\"r\":";        j += estado.r; j += ",";
  j += "\"g\":";        j += estado.g; j += ",";
  j += "\"b\":";        j += estado.b; j += "}";
  server.send(200, "application/json", j);
}

void setup() {
  Serial.begin(115200);

  ledcAttach(PIN_R, 5000, 8);
  ledcAttach(PIN_G, 5000, 8);
  ledcAttach(PIN_B, 5000, 8);

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_LDR, INPUT);

  setColor(255, 0, 0); delay(300);
  setColor(0, 255, 0); delay(300);
  setColor(0, 0, 255); delay(300);
  apagar();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  server.on("/",       handleRoot);
  server.on("/modo",   handleModo);
  server.on("/color",  handleColor);
  server.on("/brillo", handleBrillo);
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("Listo");
}

void loop() {
  server.handleClient();
  if      (estado.modo == AUTO)   loopAuto();
  else if (estado.modo == FIESTA) loopFiesta();
}
