#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SEU_WIFI_AQUI";     // O nome da rede Wi-Fi da sua sala/casa
const char* password = "SUA_SENHA_AQUI"; // A senha do Wi-Fi

// --- Cria o objeto do Servidor Web na porta 80 ---
WebServer server(80);

// --- Definições dos Pinos ---
// Usaremos o LED embutido (onboard) no pino 2
const int PINO_LED = 2;

// --- Variável de Estado ---
bool ledEstaLigado = false; // Controla o estado atual do LED

// --- Handlers (Manipuladores) do Servidor Web ---

// Esta função será chamada quando alguém acessar a rota "/ligar"
void handleLigar() {
  digitalWrite(PINO_LED, HIGH); // Liga o LED
  ledEstaLigado = true;
  Serial.println("Comando HTTP: LIGAR");
  
  // Redireciona o navegador de volta para a página principal
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecionando...");
}

// Esta função será chamada quando alguém acessar a rota "/desligar"
void handleDesligar() {
  digitalWrite(PINO_LED, LOW); // Desliga o LED
  ledEstaLigado = false;
  Serial.println("Comando HTTP: DESLIGAR");
  
  // Redireciona o navegador de volta para a página principal
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecionando...");
}

// Esta função constrói e envia a página HTML principal
void handleRoot() {
  String html = "<html>";
  html += "<head><title>Controle de LED - ESP32</title>";
  // Meta tag para fazer a página se adaptar ao celular
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  
  // CSS para deixar os botões grandes e amigáveis ao toque
  html += "<style>";
  html += " body { font-family: sans-serif; text-align: center; margin-top: 50px; }";
  html += " a { display: block; padding: 20px; margin: 20px auto; width: 80%;";
  html += "     font-size: 2em; text-decoration: none; border-radius: 10px; }";
  html += " .ligar { background-color: #5B5; color: white; }"; // Botão Ligar (Verde)
  html += " .desligar { background-color: #D33; color: white; }"; // Botão Desligar (Vermelho)
  html += " .estado { font-size: 1.2em; }";
  html += "</style></head>";
  
  // Corpo da página
  html += "<body>";
  html += "<h1>Servidor Web ESP32</h1>";
  
  // Mostra o estado atual do LED
  if (ledEstaLigado) {
    html += "<p class='estado'>O LED esta: <strong>LIGADO</strong></p>";
  } else {
    html += "<p class='estado'>O LED esta: <strong>DESLIGADO</strong></p>";
  }
  
  // Adiciona os botões (que são links)
  html += "<a class='ligar' href='/ligar'>LIGAR O LED</a>";
  html += "<a class='desligar' href='/desligar'>DESLIGAR O LED</a>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html); // Envia a página pronta para o navegador
}

// ---------------------------------------------

void setup() {
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);
  
  Serial.begin(115200);
  Serial.println("Iniciando...");

  // --- 1. Conectar ao Wi-Fi ---
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // Espera a conexão
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); // ESSA LINHA É CRUCIAL!

  // --- 2. Configurar o Servidor Web ---
  server.on("/", handleRoot);
  server.on("/ligar", handleLigar);
  server.on("/desligar", handleDesligar);

  server.begin(); // Inicia o servidor web
  Serial.println("Servidor HTTP iniciado.");

  // --- Adicionado para teste no Wokwi ---
  Serial.println("\n--- SIMULACAO DE REQUISICAO HTTP ---");
  Serial.println("Digite 'ligar' ou 'desligar' e pressione Enter.");
}

void loop() {
  // Esta linha processa as requisições HTTP
  server.handleClient(); 

  // --- GATILHO DE SIMULAÇÃO ---
  // Este bloco permite simular os cliques do celular via Monitor Serial
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    
    if (comando == "ligar") {
      // Simula o acesso a /ligar
      handleLigar();
    } 
    else if (comando == "desligar") {
      // Simula o acesso a /desligar
      handleDesligar();
    }
  }
}
