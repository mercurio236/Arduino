#include "WiFiEsp.h"
#include "SoftwareSerial.h"

SoftwareSerial Serial1(6, 7);

char ssid[] = "Inst Brasilia de Tec e Inov 2G";
char pass[] = "#ibti@2019";

int status = WL_IDLE_STATUS;

WiFiEspServer server(80);
RingBuffer buf(8);

int statusLed = LOW;

void sendHttpResponse(WiFiEspClient client){
    client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
    client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
    client.println("");
    client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
    client.println("<html>"); //ABRE A TAG "html"
    client.println("<head>"); //ABRE A TAG "head"
    client.println("<link rel='icon' type='image/png' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/icon_mws.png'/>"); //DEFINIÇÃO DO ICONE DA PÁGINA
    client.println("<link rel='stylesheet' type='text/css' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/webpagecss.css' />"); //REFERENCIA AO ARQUIVO CSS (FOLHAS DE ESTILO)
    //client.println("<title>MasterWalker Shop - Modulo WiFi ESP8266 para Arduino</title>"); //ESCREVE O TEXTO NA PÁGINA
    client.println("</head>"); //FECHA A TAG "head"
    
    //AS LINHAS ABAIXO CRIAM A PÁGINA HTML
    client.println("<body>"); //ABRE A TAG "body"
    //client.println("<img alt='masterwalkershop' src='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/logo_mws.png' height='156' width='700' />"); //LOGO DA MASTERWALKER SHOP
    client.println("<p style='line-height:2'><font>Modulo WiFi ESP8266 para Arduino</font></p>"); //ESCREVE O TEXTO NA PÁGINA
    client.println("<font>ESTADO ATUAL DO LED</font>"); //ESCREVE O TEXTO NA PÁGINA
    
    if (statusLed == HIGH){ //SE VARIÁVEL FOR IGUAL A HIGH (1), FAZ
      client.println("<p style='line-height:0'><font color='green'>LIGADO</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
      client.println("<a href=\"/L\">APAGAR</a>"); //COMANDO PARA APAGAR O LED (PASSA O PARÂMETRO /L)
    }else{ //SENÃO, FAZ
      if (statusLed == LOW){ //SE VARIÁVEL FOR IGUAL A LOW (0), FAZ
      client.println("<p style='line-height:0'><font color='red'>DESLIGADO</font></p>"); //ESCREVE "DESLIGADO" NA PÁGINA
      client.println("<a href=\"/H\">ACENDER</a>"); //COMANDO PARA ACENDER O LED (PASSA O PARÂMETRO /H)
      }
    }
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
    client.println("</body>"); //FECHA A TAG "body"
    client.println("</html>"); //FECHA A TAG "html"
    delay(1); //INTERVALO DE 1 MILISSEGUNDO

}

void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
 digitalWrite(LED_BUILTIN, LOW);
 Serial.begin(9600);
 Serial1.begin(9600);
 WiFi.init(&Serial1);
 WiFi.config(IPAddress(192,168,2,88)); 

  if(WiFi.status() == WL_NO_SHIELD){
    while(true);
  }
  while(status != WL_CONNECTED){
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
}

void loop() {
  WiFiEspClient client = server.available();

  if(client){
    buf.init();
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        buf.push(c);

        if(buf.endsWith("\r\n\r\n")){
        sendHttpResponse(client);
        break;
        }
        if(buf.endsWith("GET /H")){
          digitalWrite(LED_BUILTIN, HIGH);
          statusLed = 1;
        }
        else{
          if(buf.endsWith("GET /L")){
            digitalWrite(LED_BUILTIN, LOW);
            statusLed = 0;
          }
        }
      }
    }
    client.stop();
  }
}

  
