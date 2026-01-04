#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <LoRa.h>

/* ================= WIFI ================= */

const char* AP_SSID = "Streetlight-Gateway";
const char* AP_PASS = "12345678";

/* ================= LORA ================= */

#define LORA_SS   D8
#define LORA_RST  D0
#define LORA_DIO0 D1

/* ================= DATA ================= */

struct NodeData {
  float lat;
  float lon;
  int pwm;
  float voltage;
  float current;
  int fault;
  unsigned long lastSeen;
};

#define MAX_NODES 50

struct NodeEntry {
  String id;
  NodeData data;
};

NodeEntry nodes[MAX_NODES];
int nodeCount = 0;

/* ================= SERVER ================= */

ESP8266WebServer server(80);

/* ================= DASHBOARD HTML ================= */
/* Paste your FINAL CMS.html content below */
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Ward-Level Streetlight Dashboard</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<!-- Leaflet -->
<link rel="stylesheet" href="https://unpkg.com/leaflet/dist/leaflet.css"/>

<style>
:root {
  --bg:#020617;
  --panel:rgba(255,255,255,0.05);
  --text:#e5e7eb;
  --muted:#9ca3af;
  --ok:#22c55e;
  --fault:#ef4444;
}

html,body{
  margin:0;
  height:100%;
  background:radial-gradient(circle at top,#0b1020,#020617);
  color:var(--text);
  font-family:Inter,system-ui,-apple-system,Segoe UI,Roboto;
}

header{
  padding:18px 22px;
  display:flex;
  justify-content:space-between;
  align-items:center;
}

h1{font-size:20px;margin:0;font-weight:600;}
small{color:var(--muted);}

.card{
  margin:18px;
  padding:18px;
  background:var(--panel);
  border-radius:14px;
  box-shadow:0 10px 40px rgba(0,0,0,0.5);
}

.stats{
  display:flex;
  gap:18px;
}

.stat{
  padding:14px 18px;
  border-radius:12px;
  background:rgba(255,255,255,0.04);
  min-width:120px;
  text-align:center;
}

.stat strong{font-size:22px;display:block;}

#map{
  height:420px;
  border-radius:12px;
  overflow:hidden;
}

table{
  width:100%;
  border-collapse:collapse;
  font-size:14px;
}

th,td{
  padding:10px;
  border-bottom:1px solid rgba(255,255,255,0.06);
  text-align:left;
}

.status-ok{color:var(--ok);font-weight:600;}
.status-fault{color:var(--fault);font-weight:600;}
</style>
</head>

<body>

<header>
  <h1>Ward-Level Streetlight Dashboard</h1>
  <small>Transport: LoRa → Gateway</small>
</header>

<div class="card">
  <div class="stats">
    <div class="stat"><small>Total Nodes</small><strong id="total">0</strong></div>
    <div class="stat"><small>Faults</small><strong id="faults">0</strong></div>
    <div class="stat"><small>Offline</small><strong id="offline">0</strong></div>
  </div>
</div>

<div class="card">
  <div id="map"></div>
</div>

<div class="card">
  <table>
    <thead>
      <tr>
        <th>ID</th>
        <th>PWM</th>
        <th>Voltage (V)</th>
        <th>Current (A)</th>
        <th>Status</th>
      </tr>
    </thead>
    <tbody id="tableBody"></tbody>
  </table>
</div>

<!-- Leaflet -->
<script src="https://unpkg.com/leaflet/dist/leaflet.js"></script>

<script>
/* ================= MAP ================= */

const map = L.map("map").setView([18.525,73.856],15);
L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png").addTo(map);

/* ================= STATE ================= */

const markers = {};
const tableBody = document.getElementById("tableBody");

/* ================= SAFETY ================= */

function safeLatLng(d){
  if(typeof d.lat !== "number") return null;
  if(typeof d.lon !== "number") return null;
  return [d.lat,d.lon];
}

/* ================= RENDER ================= */

function render(data){
  tableBody.innerHTML="";
  let total=0,faults=0,offline=0;
  const now=Date.now();

  for(const id in data){
    const d=data[id];
    total++;

    const pos=safeLatLng(d);
    if(!pos) continue;

    const isFault=d.fault!==0;
    if(isFault) faults++;

    if(!markers[id]){
      markers[id]=L.circleMarker(pos,{
        radius:7,
        color:isFault?"#ef4444":"#22c55e",
        fillOpacity:0.85
      }).addTo(map);
      markers[id].bindPopup(id);
    }else{
      markers[id].setLatLng(pos);
      markers[id].setStyle({
        color:isFault?"#ef4444":"#22c55e"
      });
    }

    if(now-d.last_seen>90000) offline++;

    const tr=document.createElement("tr");
    tr.innerHTML=`
      <td>${id}</td>
      <td>${d.pwm}</td>
      <td>${d.voltage.toFixed(2)}</td>
      <td>${d.current.toFixed(2)}</td>
      <td class="${isFault?"status-fault":"status-ok"}">
        ${isFault?"FAULT":"OK"}
      </td>`;
    tableBody.appendChild(tr);
  }

  document.getElementById("total").innerText=total;
  document.getElementById("faults").innerText=faults;
  document.getElementById("offline").innerText=offline;
}

/* ================= LIVE FETCH ================= */

function fetchData(){
  fetch("/data")
    .then(r=>r.json())
    .then(render)
    .catch(()=>console.warn("Gateway not reachable"));
}

setInterval(fetchData,2000);
fetchData();
</script>

</body>
</html>
)rawliteral";

/* ================= UTILS ================= */

int findNodeIndex(String id) {
  for (int i = 0; i < nodeCount; i++) {
    if (nodes[i].id == id) return i;
  }
  return -1;
}

void assignCoordinates(NodeData &d, int index) {
  // Fixed cluster for demo (replace with real GPS registry later)
  d.lat = 18.5250 + (index * 0.0003);
  d.lon = 73.8560 + (index * 0.0003);
}

/* ================= LORA RECEIVE ================= */

void onLoRaReceive(int packetSize) {
  if (packetSize == 0) return;

  String payload = "";
  while (LoRa.available()) {
    payload += (char)LoRa.read();
  }

  // Expected format:
  // {"id":"SL-001","pwm":150,"v":12.1,"i":0.42,"fault":0}

  int idPos = payload.indexOf("\"id\":\"");
  if (idPos < 0) return;

  String id = payload.substring(idPos + 6, payload.indexOf("\"", idPos + 6));

  int idx = findNodeIndex(id);
  if (idx < 0 && nodeCount < MAX_NODES) {
    idx = nodeCount++;
    nodes[idx].id = id;
    assignCoordinates(nodes[idx].data, idx);
  }

  if (idx < 0) return;

  NodeData &d = nodes[idx].data;

  d.pwm     = payload.substring(payload.indexOf("\"pwm\":") + 6).toInt();
  d.voltage = payload.substring(payload.indexOf("\"v\":") + 4).toFloat();
  d.current = payload.substring(payload.indexOf("\"i\":") + 4).toFloat();
  d.fault   = payload.substring(payload.indexOf("\"fault\":") + 8).toInt();

  d.lastSeen = millis();
}

/* ================= HTTP HANDLERS ================= */

void handleRoot() {
  server.send_P(200, "text/html", DASHBOARD_HTML);
}

void handleData() {
  String json = "{";

  for (int i = 0; i < nodeCount; i++) {
    NodeData &d = nodes[i].data;

    json += "\"" + nodes[i].id + "\":{";
    json += "\"lat\":" + String(d.lat, 6) + ",";
    json += "\"lon\":" + String(d.lon, 6) + ",";
    json += "\"pwm\":" + String(d.pwm) + ",";
    json += "\"voltage\":" + String(d.voltage, 2) + ",";
    json += "\"current\":" + String(d.current, 2) + ",";
    json += "\"fault\":" + String(d.fault) + ",";
    json += "\"last_seen\":" + String(d.lastSeen);
    json += "}";

    if (i < nodeCount - 1) json += ",";
  }

  json += "}";
  server.send(200, "application/json", json);
}

/* ================= SETUP ================= */

void setup() {
  Serial.begin(9600);

  /* WiFi AP */
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println("Gateway AP started");
  Serial.println(WiFi.softAPIP());

  /* Web Server */
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();

  /* LoRa */
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa failed");
    while (1);
  }

  LoRa.onReceive(onLoRaReceive);
  LoRa.receive();
}

/* ================= LOOP ================= */

void loop() {
  server.handleClient();
}
