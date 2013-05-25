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

#define API_KEY            "4Z17HHTeWELgxqIsB3WMWfu9V1SESwh6YKoG77Nr" // change the default value for security purposes
#define SERVER             "tuitwall.kungfulabs.com"
const char get_request[] = "GET http://" SERVER "/fetch.php?api=" API_KEY;
const int interval       = 12000;  // minimum time between tweet retrievals
const int vec_length     = 200;    // maximum tweet lenght in characters
const int speed          = 40;     // change to control text scroll speed
const int timeout        = 2000;   // maximum time to receive a tweet

// Add your ethernet shield MAC address
// New ethernet shields have the MAC address printed on a sticker
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
char msg[vec_length] = "Conectando...";

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
    // couldn't get an IP, DHCP error, halt execution
    Serial.println(F("## Execution halted ##"));
    for(;;);
  }
  Serial.println(F("OK"));
  // wait a second for the ethernet shield to initialize
  delay(1000);
  Serial.println();
  Serial.println(F("## Program executing ##"));
}

void loop()
{
  // get last tweet
  getTweet(msg);
  // show it with a scrolling motion
  scrollText(msg);
}

void scrollText(char text[]){
  // calculate text width
  int wd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
  // do the horizontal scrolling motion
  for(int offset=0; offset<=OUT_SIZE+wd; offset++){
    showText(text,offset);
    delay(speed);
  }
}

void showText(char text[], int offset){
  // empty memory, draw text in memory and render it on the LED matrix
  HT1632.clear();
  HT1632.drawText(text, OUT_SIZE - offset, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
  HT1632.render();
}

void centerText(char text[]){
  // calculate text width
  int wd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
  // if smaller than screen, align to the center, if bigger, align to the left
  int offset = (wd < OUT_SIZE) ? int(((OUT_SIZE+float(wd))/2)+0.5) : OUT_SIZE;
  showText(text, offset);
}

void getTweet(char tweet[]){
  static long previousTime = 0;  // last time we asked for a tweet
  static boolean skipWait;       // should we skip the wait? (for API call limit)
  int pos;                       // array position where to store new data
  char buf[vec_length];          // array where to store tweet until fully received
  char c;                        // last character received

  // if we don't have to skip the wait but, but we haven't wait the mimimum time between requests,
  // then we don't request a tweet. Twitter limits the number of requests per hour to 350
  // https://dev.twitter.com/docs/rate-limiting/1.1#rest
  if (!skipWait && (millis()-interval < previousTime)){
    Serial.println("Limiter active - Reuse tweet");
    return;
  }

  skipWait = true;                // by default we suppose we can skip the wait
  previousTime = millis();
  pos = 0;
  strcpy(buf,"1");                // initialize the array to a value different than /0 to know if there is an error when receving

  Serial.println();
  Serial.print(F("Connecting........... "));
  // connect to the server at port 80 (http)
  if (client.connect(SERVER, 80)) {
    Serial.println(F("OK"));
    // get the tweet through a GET request
    client.println(get_request);
    client.println();
  }
  else {
    // if failed to connect to the server
    Serial.println(F("ERROR"));
    return;
  }

  Serial.print(F("Receiving............ "));
  while (client.connected() && (millis()-timeout < previousTime)) {
    if (client.available()){
      c = client.read();
      // if is a standar character, store it in the temporal buffer
      buf[pos++] = c;
      // if we are in the last array position, force the character to be \0 to prevent indexing outside the array
      if (pos == vec_length){
        c = '\0';
        buf[pos] = c;
        Serial.print(F("OVERFLOW - "));
      }
      // last character is 0, so we know is the end of the string
      if (c == '\0') {
        Serial.println(F("OK"));
        // close the connection to the server
        client.stop();
        // copy the received array to the led matrix array to show it
        strcpy(tweet, buf);
        Serial.print(F("Tweet: "));
        Serial.println(tweet);
        // no timeout existed, so we shouldn't skip wait time
        skipWait = false;
        return;
      }
    }
  }

  // if connection is already closed and last character is different than 0, error
  if (buf[pos] != '\0' && !client.connected()){
    client.stop();
    Serial.println(F("ERROR - Unknown response"));
    return;
  }
  // if there has been a timeout, close the connection
  client.stop();
  Serial.println(F("ERROR - Timeout"));
}
