#include <WiFi.h>
#include <WebServer.h>

// --- Configurações do Wi-Fi ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const int WIFI_CHANNEL = 6; // Acelera a conexão no Wokwi

// --- Definições de Hardware ---
const int PINO_BOTAO = 0;
const int PINO_LED = 2;

// --- Variáveis de Estado ---
bool ledAcionado = false;
unsigned long tempoInicioAcionamento = 0;
const unsigned long DURACAO_ACIONAMENTO = 5000; // 5 segundos

// --- Servidor Web ---
WebServer server(80);

// =================================================================================
// CÓDIGO HTML + CSS + JAVASCRIPT (Armazenado na memória flash para economizar RAM)
// =================================================================================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Painel ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    /* Estilo Geral e Responsivo */
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f0f2f5; text-align: center; margin: 0; padding: 0; }
    h2 { color: #333; margin-bottom: 20px; }
    
    /* O Card Central */
    .card { background: white; max-width: 400px; margin: 50px auto; padding: 30px; border-radius: 15px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); }
    
    /* O Indicador de Status (Bolinha) */
    .status-box { margin-bottom: 30px; font-size: 1.2rem; color: #555; }
    .dot { height: 15px; width: 15px; border-radius: 50%; display: inline-block; margin-right: 10px; }
    .dot-off { background-color: #bbb; }
    .dot-on { background-color: #4CAF50; box-shadow: 0 0 10px #4CAF50; }
    
    /* O Botão Principal */
    .button {
      background-color: #008CBA; /* Azul */
      border: none; color: white; padding: 15px 32px;
      text-align: center; text-decoration: none; display: inline-block;
      font-size: 18px; margin: 4px 2px; cursor: pointer;
      border-radius: 50px; transition: 0.3s; width: 80%;
      box-shadow: 0 4px 6px rgba(0,0,0,0.2);
    }
    .button:active { transform: scale(0.98); }
    .button:disabled { background-color: #cccccc; cursor: not-allowed; }
    .button.busy { background-color: #f44336; } /* Vermelho quando ocupado */
    
    /* Rodapé */
    .footer { font-size: 0.8rem; color: #888; margin-top: 20px; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Gerenciador de Timer</h2>
    
    <div class="status-box">
      Status: <span id="status-text">Aguardando...</span>
      <span id="status-dot" class="dot dot-off"></span>
    </div>

    <button id="btn-acao" class="button" onclick="acionar()">LIGAR TIMER (5s)</button>
    
    <div class="footer">Painel de Controle IoT • ESP32</div>
  </div>

<script>
  // --- Lógica JavaScript (Roda no Celular/Navegador) ---
  
  // 1. Função para enviar o comando de ligar
  function acionar() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/ligar", true);
    xhttp.send();
    // Não precisamos mudar a tela aqui, a função de atualização fará isso
  }

  // 2. Função que roda a cada 1 segundo para atualizar o status
  setInterval(function() {
    getData();
  }, 1000);

  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        // O ESP32 retorna "1" (ligado) ou "0" (desligado)
        var estado = this.responseText;
        atualizarInterface(estado);
      }
    };
    xhttp.open("GET", "/status", true); // Chama a rota /status
    xhttp.send();
  }

  // 3. Atualiza as cores e textos da tela
  function atualizarInterface(estado) {
    var btn = document.getElementById("btn-acao");
    var dot = document.getElementById("status-dot");
    var txt = document.getElementById("status-text");

    if (estado == "1") {
      // LIGADO
      btn.innerHTML = "SISTEMA EM USO...";
      btn.classList.add("busy"); // Fica vermelho
      // btn.disabled = true; // Opcional: bloqueia o clique
      dot.className = "dot dot-on";
      txt.innerHTML = "ATIVO";
      txt.style.color = "#4CAF50";
    } else {
      // DESLIGADO
      btn.innerHTML = "LIGAR TIMER (5s)";
      btn.classList.remove("busy"); // Volta ao azul
      // btn.disabled = false;
      dot.className = "dot dot-off";
      txt.innerHTML = "Ocioso";
      txt.style.color = "#555";
    }
  }
</script>
</body>
</html>
)rawliteral";

// =================================================================================
// LÓGICA DO BACKEND (C++)
// =================================================================================

// Função Central de Lógica (Igual à sua)
void iniciarTimer(String origem) {
  digitalWrite(PINO_LED, HIGH);
  tempoInicioAcionamento = millis();
  ledAcionado = true;
  Serial.println("ACAO: " + origem + " -> LED LIGADO.");
}

// Rota Principal: Entrega a página bonita
void handleRoot() {
  server.send(200, "text/html", index_html);
}

// Rota de Comando: Recebe o pedido do JavaScript para ligar
void handleLigar() {
  if (!ledAcionado) {
    iniciarTimer("Via App Web");
    server.send(200, "text/plain", "OK");
  } else {
    server.send(200, "text/plain", "BUSY");
  }
}

// Rota de Status: Informa ao JavaScript se o LED está ligado ou não
// Isso permite que o celular saiba se alguém apertou o botão físico!
void handleStatus() {
  if (ledAcionado) {
    server.send(200, "text/plain", "1");
  } else {
    server.send(200, "text/plain", "0");
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  // Wi-Fi
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password, WIFI_CHANNEL);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());

  // Configura Rotas
  server.on("/", handleRoot);
  server.on("/ligar", handleLigar);
  server.on("/status", handleStatus); // Nova rota essencial

  server.begin();
  Serial.println("Servidor Web Profissional Iniciado.");
}

void loop() {
  server.handleClient(); // Atende as requisições (App)

  // --- Lógica do Botão Físico ---
  if (digitalRead(PINO_BOTAO) == LOW && !ledAcionado) {
    delay(50); // Debounce
    if (digitalRead(PINO_BOTAO) == LOW) {
      iniciarTimer("Via Botao Fisico");
    }
  }

  // --- Lógica do Timer ---
  if (ledAcionado) {
    if (millis() - tempoInicioAcionamento >= DURACAO_ACIONAMENTO) {
      digitalWrite(PINO_LED, LOW);
      ledAcionado = false;
      Serial.println("Timer finalizado. LED OFF.");
    }
  }
  
  delay(2); // Pequena pausa para estabilidade do Wi-Fi simulado
}
