/*
    tuitwall is an Arduino program that shows tweets, provided by
    a backend that interfaces Twitter, in a LED matrix.
    Copyright (C) 2011 Santiago Reig

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduBus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with tuitwall. If not, see <http://www.gnu.org/licenses/>.
*/

// NOTE: This sketch only compiles on Arduino v1.0 or higher

#include <SPI.h>
#include <Ethernet.h>
#include "font_5x4.h"
#include "HT1632.h"

#define API_KEY "4Z17HHTeWELgxqIsB3WMWfu9V1SESwh6YKoG77Nr" // usa una nueva cadena distinta a la por defecto para garantizar la seguridad
#define SERVER "tuitwall.kungfulabs.com"
#define GET_REQUEST "GET http://" SERVER "/fetch.php?api=" API_KEY
#define INTERVAL 12000  // cada cuantos milisegundos pedir el tweet
#define VEC_LENGTH 200  // longitud del vector donde se va a almacenar el tweet TODO: longitud maxima tweet con RT incluido?
#define SPEED 40        // cambiar para variar la velocidad
#define TIMEOUT 2000    // tiempo maximo para recibir un tweet

// Introduce la direccion MAC de tu placa ethernet abajo
// Las nuevas placas de ethernet tienen la direccion MAC imprimida en una pegatina
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
char msg[VEC_LENGTH] = "Conectando...";

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("tuitwall BETA4"));
  Serial.println(F("KungFu Labs - http://www.kungfulabs.com"));
  Serial.println(F("tuitwall Copyright (C) 2011  Santiago Reig"));
  Serial.println();

  Serial.println(F("## Iniciando sistemas ##"));
  Serial.println();
  Serial.print(F("Panel led............ "));
  HT1632.begin(7,6,5);
  centerText("Welcome");
  Serial.println(F("OK"));

  Serial.print(F("Ethernet............. "));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("ERROR - DHCP"));
    centerText("E-DHCP");
    // no se pudo obtener una IP, detenemos la ejecucion del programa
    Serial.println(F("## Ejecucion detenida ##"));
    for(;;);
  }
  Serial.println(F("OK"));
  // esperamos un segundo para que se inicie la shield Ethernet
  delay(1000);
  Serial.println();
  Serial.println(F("## Ejecucion programa ##"));
}

void loop()
{
  Serial.println();
  // pedimos el último tweet
  getTweet(msg);
  // lo mostramos en pantalla haciendo un scroll completo
  scrollText(msg);
}

void scrollText(char text[]){
  // calculamos la achura del texto
  static int wd;
  wd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
  // hacemos la animación de desplazamiento horizontal (scroll)
  for(int offset=0; offset<=OUT_SIZE+wd; offset++){
    showText(text,offset);
    delay(SPEED);
  }
}

void showText(char text[], int offset){
  // vaciamos la memoria, dibujamos el texto en memoria y lo renderizamos en la matriz de LEDs
  HT1632.clear();
  HT1632.drawText(text, OUT_SIZE - offset, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
  HT1632.render();
}

void centerText(char text[]){
  int wd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
  int offset;
  if (wd < OUT_SIZE) offset = int(((OUT_SIZE+float(wd))/2)+0.5);
  else offset = OUT_SIZE;
  showText(text, offset);
}

void getTweet(char tweet[]){
  static long previousTime = 0;  // última vez que pedimos un tweet
  static boolean skipWait;       // ¿debemos saltarnos la espera?
  static int pos;                // posición del vector donde guardar un nuevo dato
  static char buf[VEC_LENGTH];   // vector donde guardar temporalmente el tweet hasta saber que lo tenemos completo
  static char c;                 // donde guardar temporalmente el últmo caracter recibido

  // si no tenemos que saltarnos la espera y no ha pasado INTERVAL milisegundos desde la ultima petición
  // no pedimos el nuevo tweet. Ésto es debido a que twitter limita el número de peticiones por hora a 350
  // https://dev.twitter.com/docs/rate-limiting#rest
  if (!skipWait && (millis()-INTERVAL < previousTime)) return;

  skipWait = true;
  previousTime = millis();
  pos = 0;
  strcpy(buf,"1");                // iniciamos la cadena a un valor conocido distinto de \0 para saber si hubo error al recibir

  Serial.print(F("Conectando........... "));
  // nos conectamos al servidor al puerto 80 (protocolo http)
  if (client.connect(SERVER, 80)) {
    Serial.println(F("OK"));
    // hacemos la petición del tweet mediante GET
    client.println(GET_REQUEST);
    client.println();
  }
  else {
    // si no conseguimos conectarnos al servidor
    Serial.println(F("ERROR"));
    return;
  }

  Serial.print(F("Recibiendo........... "));
  while (client.connected() && (millis()-TIMEOUT < previousTime)) {
    if (client.available()){
      c = client.read();
      // si es un carácter normal, lo guardamos en el vector temporal
      buf[pos++] = c;
      // si estamos en la última posición del vector, forzamos a escribir el delimitador de
      // cadena de texto (\0) para así evitar sobreescribir memoria descontroladamente si
      // el tweet tiene una longitud mayor a VEC_LENGTH
      if (pos == VEC_LENGTH){
        c = '\0';
        Serial.print(F("OVERFLOW - "));
      }
      // el último carácter de la cadena es el 0, por lo que sabemos que es el fin de la cadena
      if (c == '\0') {
        Serial.println(F("OK"));
        // cortamos la conexion con el servidor
        client.stop();
        // copiamos la cadena recibida a la que mostramos en la barra
        strcpy(tweet, buf);
        Serial.print(F("Tweet: "));
        Serial.println(tweet);
        // no ha habido timeout, por lo que no deberemos saltarnos el límite de tiempo
        skipWait = false;
        return;
      }
    }
  }

  // si se ha cerrado la conexión y el último carácter es distinto de 0, error
  if (buf[pos] != '\0' && !client.connected()){
    client.stop();
    Serial.println(F("ERROR - Unknown response"));
    return;
  }
  // si ha habido timeout, cerramos la conexión
  client.stop();
  Serial.println(F("ERROR - Timeout"));
}
