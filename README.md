# Trabalho Final
Códigos e bibliotecas para o trabalho final de redes industriais

A pasta  *arduino_ethernet_udp* possui exemplo de código para um arduino enviar e receber dados por um socket UDP através da  interface Ethernet. Mais especificamente, o código envia dois floats e e verifica se existem dados para serem lidos do socket. Caso exista, o arduino imprime pela serial os dados recebidos pelo socket UDP.

A pasta *arduino_esp_wifi_udp* possui um exemplo de código para enviar e receber dados por um socket UDP através da  interface WiFi que o ESP8266 fornece. Mais especificamente, o arduino envia dois floats e verifica se existem dados para serem lidos do socket UDP. Caso exista, o arduino imprime pela serial os dados recebidos pelo socket UDP. 
OBS1: É necessário incluir a biblioteca WiFiEsp, uma versão dessa biblioteca pode ser encontrada aqui: https://drive.google.com/open?id=1sXC40L9S7Njgbn_M5p7vWci-QDN9oRzi
OBS2: É necessário mudar o baudrate do ESP para 9600. Para mais detalhes, consulte o documento em disponível em [1].

A pasta *matlab_udp* possui exemplos de scripts matlab. São três exemplos: *udp_open_socket_and_send.m*, *netlab_udp_rx.m* e *netlab_udp_polling.m*. Mais informações está disponível no documento entrgue para vocês. Para mais detalhes, consulte o documento disponível em [1].

[1] https://docs.google.com/document/d/1CQJT3h2WCKlYyWcuzAPkP61BEpslamdjceSUzDeAxJw/edit?usp=sharing
