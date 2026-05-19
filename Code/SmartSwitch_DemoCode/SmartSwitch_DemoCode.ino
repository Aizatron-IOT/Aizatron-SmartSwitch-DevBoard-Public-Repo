#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

// ======================================================
// ACCESS POINT
// ======================================================
const char* apSSID = "Aizatron_SmartSwitch_Demo";
const char* apPassword = "SmartSwitch";

// ======================================================
// SERVERS
// ======================================================
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// ======================================================
// GPIO STRUCTURE
// ======================================================
struct GPIOPin {

  uint8_t pin;

  String name;

  String group;

  bool state;

  bool output;

  bool controllable;
};

// ======================================================
// GPIO LIST
// ======================================================
GPIOPin gpioList[] = {

  // ==================================================
  // RELAY HEADER (8)
  // ==================================================
  {4,  "Relay 1", "Relay", false, true, true},
  {16, "Relay 2", "Relay", false, true, true},
  {17, "Relay 3", "Relay", false, true, true},
  {23, "Relay 4", "Relay", false, true, true},
  {27, "Relay 5", "Relay", false, true, true},
  {26, "Relay 6", "Relay", false, true, true},
  {33, "Relay 7", "Relay", false, true, true},
  {19, "Relay 8", "Relay", false, true, true},

  // ==================================================
  // HEADER A (9)
  // Extra GPIOs FIRST
  // ==================================================
  {36, "INPUT ONLY", "Header A", false, false, false},
  {39, "INPUT ONLY", "Header A", false, false, false},
  {34, "INPUT ONLY", "Header A", false, false, false},
  {35, "INPUT ONLY", "Header A", false, false, false},

  {32, "GPIO 32", "Header A", false, true, true},
  {25, "GPIO 25", "Header A", false, true, true},
  {14, "GPIO 14", "Header A", false, true, true},
  {12, "GPIO 12", "Header A", false, true, true},
  {2,  "LED",  "Header A", false, true, true},

  // ==================================================
  // HEADER B (9)
  // 1 + 3 in positions 2 and 3
  // ==================================================
  {22, "GPIO 22", "Header B", false, true, true},

  {1, "TX", "Header B", false, false, false},
  {3, "RX", "Header B", false, false, false},

  {21, "GPIO 21", "Header B", false, true, true},
  {18, "GPIO 18", "Header B", false, true, true},
  {5,  "GPIO 5",  "Header B", false, true, true},
  {0,  "BOOT",  "Header B", false, true, true},
  {13, "GPIO 13", "Header B", false, true, true},
  {15, "GPIO 15", "Header B", false, true, true}
};
const int gpioCount = sizeof(gpioList) / sizeof(gpioList[0]);

// ======================================================
// FIND GPIO INDEX
// ======================================================
int findGPIOIndexByPin(int pin) {
  for (int i = 0; i < gpioCount; i++) {
    if (gpioList[i].pin == pin)
      return i;
  }
  return -1;
}

// ======================================================
// BROADCAST STATE
// ======================================================
void broadcastGPIOState(int index) {
  String json = "{";

  json += "\"pin\":";
  json += gpioList[index].pin;
  json += ",";

  json += "\"state\":";
  json += gpioList[index].state ? "true" : "false";

  json += "}";

  webSocket.broadcastTXT(json);
}

// ======================================================
// SET GPIO STATE
// ======================================================
void setGPIOState(int index, bool state) {
  gpioList[index].state = state;
  digitalWrite(gpioList[index].pin, state ? HIGH : LOW);

  Serial.print(gpioList[index].name);
  Serial.print(" -> ");
  Serial.println(state ? "ON" : "OFF");

  broadcastGPIOState(index);
}

// ======================================================
// WEBSOCKET EVENTS
// ======================================================
void webSocketEvent(
  uint8_t num,
  WStype_t type,
  uint8_t * payload,
  size_t length
) {

  switch(type) {
    case WStype_CONNECTED: {
      Serial.println("Client Connected");

      // Send all states
      for (int i = 0; i < gpioCount; i++) {
        String json = "{";

        json += "\"pin\":";
        json += gpioList[i].pin;
        json += ",";

        json += "\"state\":";
        json += gpioList[i].state ? "true" : "false";

        json += "}";

        webSocket.sendTXT(num, json);
      }

      break;
    }

    case WStype_TEXT: {
      String msg = (char*)payload;
      // FORMAT:
      // TOGGLE:4
      if (msg.startsWith("TOGGLE:")) {
        int pin = msg.substring(7).toInt();
        int index = findGPIOIndexByPin(pin);
        if (index != -1) {
          // ONLY controllable pins
          if (!gpioList[index].controllable)
            return;
          bool newState = !gpioList[index].state;
          setGPIOState(index, newState);
        }
      }
      break;
    }

    case WStype_DISCONNECTED:
      Serial.println("Client Disconnected");
      break;

    default:
      break;
  }
}

// ======================================================
// CREATE PIN HTML
// ======================================================
String createPinHTML(GPIOPin gpio) {
  String html = "";

  html += "<div class='pin-row'>";

  // ===== LABEL =====
  html += "<div class='pin-label'>";

  html += "<div class='pin-name'>";
  html += gpio.name;
  html += "</div>";

  html += "<div class='pin-number'>";
  html += "GPIO ";
  html += gpio.pin;
  html += "</div>";

  html += "</div>";

  // ===== BUTTON =====
  html += "<button ";

  html += "id='btn";
  html += gpio.pin;
  html += "' ";

  // Disabled GPIO
  if (!gpio.controllable) {
    html += "class='toggle-btn disabled' disabled>";
    html += "N/A";
  } else {
    html += "class='toggle-btn ";
    html += gpio.state ? "on" : "off";
    html += "' ";

    html += "onclick='toggleGPIO(";
    html += gpio.pin;
    html += ")'>";

    html += gpio.state ? "ON" : "OFF";
  }

  html += "</button>";
  html += "</div>";
  return html;
}

// ======================================================
// CREATE PANEL
// ======================================================
String createPanel(String title) {
  String html = "";

  html += "<div class='panel'>";

  html += "<div class='panel-title'>";
  html += title;
  html += "</div>";

  for (int i = 0; i < gpioCount; i++) {
    if (gpioList[i].group == title) {
      html += createPinHTML(gpioList[i]);
    }
  }

  html += "</div>";

  return html;
}

// ======================================================
// ROOT PAGE
// ======================================================
void handleRoot() {
  String html = R"rawliteral(

<html><head>
  <title>ESP32 Devboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * {
      box-sizing: border-box;
    }
      :root {
  /* DARK MODE (DEFAULT) */
  --bg: #0B0B0B;
  --panel: #39321d;
  --panel2: #1E1E1E;
  --text: #D4AF37;
  --muted: #B8962E;
  --shadow: rgba(212,175,55,0.18);
  --accent: #C89B3C;
  --on: #22C55E;
  --off: #DC2626;
  --disabled: #555;
}

body.light {
  /* LIGHT MODE */
  --bg: #FFFDF8;
  --panel: #FFFFFF;
  --panel2: #F7F1E3;
  --text: #7A5A12;
  --muted: #9A7A2A;
  --shadow: rgba(180,140,30,0.18);
  --accent: #B8860B;
  --on: #16A34A;
  --off: #DC2626;
  --disabled: #AAA;
}

    body {
      margin: 0;
      padding: 20px;
      background: var(--bg);
      font-family: Arial;
      color: var(--text);
      transition: 0.3s;
    }

    h1 {
      text-align: center;
      margin-bottom: 25px;
      font-size: 30px;
      color: var(--text);
      text-shadow: 0 0 10px var(--shadow);
    }

    .board {
      display: flex;
      justify-content: center;
      gap: 20px;
      align-items: flex-start;
      flex-wrap: wrap;
    }

    .panel {
      width: 300px;
      background: var(--panel);
      border: 1px solid rgba(212,175,55,0.15);
      border-radius: 18px;
      padding: 18px;
      box-shadow: 0 8px 24px var(--shadow);
    }

    .panel-title {
      text-align: center;
      font-size: 24px;
      font-weight: bold;
      margin-bottom: 18px;
      color: var(--accent);
    }

    .pin-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      background: var(--panel2);
      border-radius: 12px;
      padding: 12px;
      margin-bottom: 12px;
    }

    .pin-label {
      display: flex;
      flex-direction: column;
    }

    .pin-name {
      font-size: 17px;
      font-weight: bold;
    }

    .pin-number {
      font-size: 13px;
      color: var(--muted);
      margin-top: 4px;
    }

    .toggle-btn {
      width: 95px;
      border: none;
      border-radius: 10px;
      padding: 12px;
      font-size: 15px;
      font-weight: bold;
      color: white;
      cursor: pointer;
      transition: 0.15s;
    }

    .toggle-btn:hover {
      transform: scale(1.03);
    }

    .toggle-btn.on {
      background: var(--on);
box-shadow: 0 0 12px rgba(34,197,94,0.4);
    }

    .toggle-btn.off {
      background: var(--off);
    }

    .toggle-btn.disabled {
      background: var(--disabled);
      cursor: not-allowed;
      opacity: 0.6;
    }

    @media(max-width: 1000px) {
      .board {
        flex-direction: column;
        align-items: center;
      }
    }

.theme-toggle-wrap {
  display: flex;
  justify-content: center;
  margin-bottom: 25px;
}

.theme-toggle {
  border: none;
  padding: 12px 22px;
  border-radius: 12px;
  background: var(--panel);
  color: var(--text);
  font-weight: bold;
  font-size: 14px;
  cursor: pointer;
  box-shadow: 0 4px 16px var(--shadow);
  transition: 0.25s;
}

.theme-toggle:hover {
  transform: scale(1.03);
}

  </style>
</head>

<body>

<h1>AIZATRON</h1>
<h1>SmartSwitch Devboard Demo</h1>

<div class="theme-toggle-wrap">
  <button id="themeToggle" class="theme-toggle">
    LIGHT MODE
  </button>
</div>

<div class="board">

)rawliteral";

  // ==================================================
  // PANELS
  // ==================================================
  html += createPanel("Relay");
  html += createPanel("Header A");
  html += createPanel("Header B");

  // ==================================================
  // JAVASCRIPT
  // ==================================================
  html += R"rawliteral(

</div>

<script>

  const gateway = `ws://${window.location.hostname}:81/`;
  let websocket;
  window.addEventListener('load', onLoad);
  function onLoad() {
    initWebSocket();
  }

  function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onopen = function(event) {
      console.log("WebSocket Connected");
    };

    websocket.onclose = function(event) {
      console.log("WebSocket Disconnected");
      setTimeout(initWebSocket, 2000);
    };

    websocket.onmessage = function(event) {
      const data = JSON.parse(event.data);
      updateButton(data.pin, data.state);
    };
  }

  function toggleGPIO(pin) {
    websocket.send("TOGGLE:" + pin);
  }

  function updateButton(pin, state) {
    const btn = document.getElementById("btn" + pin);

    if (!btn)
      return;

    // Ignore disabled buttons
    if (btn.classList.contains("disabled"))
      return;

    if (state) {
      btn.classList.remove("off");
      btn.classList.add("on");
      btn.innerHTML = "ON";
    } else {
      btn.classList.remove("on");
      btn.classList.add("off");
      btn.innerHTML = "OFF";
    }
  }

const themeToggle = document.getElementById("themeToggle");

themeToggle.addEventListener("click", () => {
  document.body.classList.toggle("light");
  const isLight = document.body.classList.contains("light");
  themeToggle.innerHTML = isLight
    ? "DARK MODE"
    : "LIGHT MODE";
});
</script>


</body>
</html>

)rawliteral";

  server.send(200, "text/html", html);
}

// ======================================================
// SETUP GPIOS
// ======================================================

void setupGPIOs() {
  for (int i = 0; i < gpioCount; i++) {
    if (!gpioList[i].output)
      continue;
    pinMode(gpioList[i].pin, OUTPUT);
    digitalWrite(gpioList[i].pin, LOW);
    gpioList[i].state = false;
    Serial.print("Initialized: ");
    Serial.print(gpioList[i].name);
    Serial.print(" (GPIO ");
    Serial.print(gpioList[i].pin);
    Serial.println(")");
  }
}

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("Booting...");

  setupGPIOs();

  // ==================================================
  // ACCESS POINT
  // ==================================================
  WiFi.mode(WIFI_AP);
  bool result = WiFi.softAP(apSSID, apPassword);
  if (result) {
    Serial.println("Access Point Started");
  } else {
    Serial.println("AP Failed!");
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);

  // ==================================================
  // WEB SERVER
  // ==================================================
  server.on("/", handleRoot);

  server.begin();

  Serial.println("HTTP Server Started");

  // ==================================================
  // WEBSOCKET
  // ==================================================
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket Server Started");
}

// ======================================================
// LOOP
// ======================================================
void loop() {
  server.handleClient();
  webSocket.loop();
}