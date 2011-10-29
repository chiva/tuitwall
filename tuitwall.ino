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

// Introduce la direccion MAC de tu placa ethernet abajo
// Las nuevas placas de ethernet tienen la direccion MAC imprimida en una pegatina
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
char msg[VEC_LENGTH] = "Iniciando...";

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("tuitwall BETA1"));
  Serial.println(F("KungFu Labs - http://www.kungfulabs.com"));
  Serial.println(F("tuitwall Copyright (C) 2011  Santiago Reig"));
  Serial.println();

  Serial.println(F("## Iniciando sistemas ##"));
  Serial.println();
  Serial.print(F("Panel led............ "));
  HT1632.begin(5,6,7);
  int wd = HT1632.getTextWidth("Welcome", FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
  showText("Welcome", wd-1);
  Serial.println(F("OK"));
  Serial.print(F("Ethernet............. "));

  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("ERROR - DHCP"));
    int wd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
    showText("Error-DHCP", wd-1);
    // no se pudo obtener una IP, detenemos la ejecucion del programa
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
  int wd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
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

void getTweet(char tweet[]){
  static long previousTime = 0;   // última vez que pedimos un tweet
  static boolean skipWait = true; // ¿debemos saltarnos la espera?
  
  // si no tenemos que saltarnos la espera y no ha pasado INTERVAL milisegundos desde la ultima petición
  // no pedimos el nuevo tweet. Ésto es debido a que twitter limita el número de peticiones por hora a 350
  // https://dev.twitter.com/docs/rate-limiting#rest
  if (!skipWait && (millis()-INTERVAL < previousTime)) return;

  int pos = 0;            // posición del vector donde guardar un nuevo dato
  char buf[200];          // vector donde guardar temporalmente el tweet hasta saber que lo tenemos completo
  boolean timeout = true; // damos por supuesto que ha ocurrido un timeout hasta que se demuestre lo contrario
  
  previousTime = millis();
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
  }
  
  Serial.print(F("Recibiendo........... "));
  while (client.connected() && (millis()-2000 < previousTime)) {
    char c = client.read();
    // si es un carácter normal, lo guardamos en el vector temporal
    if (c != -1) buf[pos++] = c;
    // si estamos en la última posición del vector, forzamos a escribir el delimitador de
    // cadena de texto (\0) para así evitar sobreescribir memoria descontroladamente si
    // el tweet tiene una longitud mayor a VEC_LENGTH
    if (pos == VEC_LENGTH) c = '\0';
    // el último carácter de la cadena es el 0, por lo que sabemos que es el fin de la cadena
    if (c == 0) {
      Serial.println(F("OK"));
      // cortamos la conexion con el servidor
      client.stop();
      // copiamos la cadena recibida a la que mostramos en la barra
      strcpy(tweet, buf);
      Serial.print(F("Tweet: "));
      Serial.println(tweet);
      // no ha habido timeout, por lo que no deberemos saltarnos el límite de tiempo
      timeout = false;
      skipWait = false;
    }
  }
  // si ha habido timeout, cerramos la conexión y nos saltamos el límite de tiempo,
  // para no tener que esperar a actualizar (queremos recibirlo cuanto antes para actualizar)
  if (timeout){
    client.stop();
    Serial.println(F("ERROR - Timeout"));
    skipWait = true;
  }
}
