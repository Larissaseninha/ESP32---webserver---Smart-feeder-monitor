Projeto Smart feeder monitor com ESP32.

Integrantes do projeto:
KADISH BERNARDO;
LARISSA SENA PEREIRA;
MURILO MENDES SANTO SARDÃO


Sistema IoT para monitoramento e acionamento de comedouros de gatos. O sistema controla a liberação de ração através de um temporizador preciso, podendo ser acionado tanto remotamente (via Web App no celular) quanto localmente (via botão físico).

O diferencial deste código é a sincronização em tempo real: se alguém apertar o botão físico no comedouro, o aplicativo no celular atualiza o status instantaneamente, mostrando que o sistema está "Em Uso".

Para rodar este projeto na sua própria máquina:
Clone ou Baixe: Baixe os arquivos deste repositório.
Abra na IDE o arquivo.ino, neste projeto utilizamos o simulador Wokwi.

Instale as Bibliotecas: Certifique-se de que tem as bibliotecas WiFi.h e WebServer.h (geralmente já incluídas no pacote do ESP32).
Altere as Credenciais: No topo do código, altere as seguintes linhas com o nome e a senha da sua rede Wi-Fi em:

const char* ssid = "SEU_WIFI_AQUI";
const char* password = "SUA_SENHA_AQUI";

(se não estiver usando o simulador Wokwi).

Carregue o Código: Conecte seu ESP32, se estiver utilizando a placa do ESP32, ao PC e faça o upload do código.
Caso contrário, é possível prosseguir apenas simu vom a simulação no Wokwi. 
Encontre o IP: conecte na velocidade em 115200. O ESP32 irá se conectar ao Wi-Fi e imprimir o endereço de IP dele.

ex: Conectado!
IP: 192.168.1.10

Acesse pelo Celular:
Garanta que seu celular está conectado na mesma rede Wi-Fi que o ESP32.
Abra o navegador do celular e digite o endereço de IP que apareceu no Monitor Serial (ex: http://192.168.1.10).

A página gerenciador de timer irá aparecer com o status "Aguardando..." e o botão "LIGAR TIMER (5s)" irá aparecer.



