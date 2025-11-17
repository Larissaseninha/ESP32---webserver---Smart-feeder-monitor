Projeto Smart feeder monitor com ESP32.

Integrantes do projeto:
KADISH BERNARDO
LARISSA SENA PEREIRA
MURILO MENDES SANTO SARDÃO


Sistema IoT de monitoramento do nível de ração em comedouros de gatos, capaz de enviar uma requisição Wi-Fi quando acionado o alerta para dispensar ração no pote.

Funcionalidades:

Servidor Web: O ESP32 hospeda uma página HTML acessível pela rede local.
Controle Wi-Fi: Conecta-se a uma rede Wi-Fi local para receber requisições.
Interface Simples: A página HTML possui dois botões grandes (Ligar / Desligar) otimizados para uso em celulares.
Feedback de Estado: A página é atualizada para mostrar o estado atual do LED (Ligado ou Desligado).

Hardware Necessário:

Placa ESP32 Dev Module (ou similar)
LED (no projeto, foi utilizado o LED onboard do pino 2)
Cabo USB para alimentação e upload do código

Como Usar (Passo a Passo):

Para rodar este projeto na sua própria placa, siga os passos:
Clone ou Baixe: Baixe os arquivos deste repositório.
Abra na IDE: Abra o arquivo .ino na sua Arduino IDE (ou editor de preferência).
Instale as Bibliotecas: Certifique-se de que tem as bibliotecas WiFi.h e WebServer.h (geralmente já incluídas no pacote do ESP32).
Altere as Credenciais: No topo do código, altere as seguintes linhas com o nome e a senha da sua rede Wi-Fi:

const char* ssid = "SEU_WIFI_AQUI";
const char* password = "SUA_SENHA_AQUI";
Carregue o Código: Conecte seu ESP32 ao PC e faça o upload do código.

Encontre o IP: Abra o Monitor Serial (com a velocidade em 115200). O ESP32 irá se conectar ao Wi-Fi e imprimir o endereço de IP dele.

Wi-Fi Conectado!
IP: 192.168.1.10


Acesse pelo Celular:

Garanta que seu celular está conectado na mesma rede Wi-Fi que o ESP32.
Abra o navegador do celular e digite o endereço de IP que apareceu no Monitor Serial (ex: http://192.168.1.10).
A página com os botões "LIGAR" e "DESLIGAR" irá aparecer.
