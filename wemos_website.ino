/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "Wemos";
const char *password = "thereisnospoon";

const char *webSite = "<!DOCTYPE html><html><head> <meta charset=\"utf-8\" /> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <title>Page Title</title> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <style> * { font-family: sans-serif; text-decoration: none; box-sizing: border-box; } :root { --primary-color: rgb(20, 200, 260); --background-color: white; } body { background-color: var(--primary-color); margin: 0; } .button { display: inline-block; padding: 10px; color: white; background-color: var(--primary-color); cursor: pointer; } #container { width: 400px; max-width: 90vw; padding: 20px; margin: 20px auto; background-color: white; } </style></head><body> <div id=\"container\"> <h1>Wemos D1 R2</h1> <a id=\"ledBtn\" class=\"button\" onclick=\"toggleLed()\">Toggle LED</a> <p id=\"ledStatus\">LED is Off</p> <a id=\"testing\" class=\"button\" onclick=\"handleTest()\">Test POST args</a> </div> <script> if(typeof(Storage) != \"undefined\") { console.log(\"Saving compatible\"); if(!localStorage[\"builtinLed\"]) { localStorage[\"builtinLed\"] = \"Off\"; } else { handleLed(); } console.log(localStorage); } else { console.log(\"Not compatible\"); } function toggleLed() { if(localStorage[\"builtinLed\"] == \"Off\") { localStorage[\"builtinLed\"] = \"On\"; } else { localStorage[\"builtinLed\"] = \"Off\"; } handleLed(); } function handleLed() { var ledStatus = localStorage[\"builtinLed\"]; var xhttp = new XMLHttpRequest(); xhttp.open(\"POST\", \"/post\", true); xhttp.send(\"ledStatus=\" + ledStatus); document.getElementById(\"ledStatus\").innerHTML = \"LED is \" + localStorage[\"builtinLed\"]; } function handleTest() { var xhttp = new XMLHttpRequest(); xhttp.open(\"POST\", \"/handle_test\", true); xhttp.send(\"cheese=burger&ham=sandwich\"); } </script></body></html>";


ESP8266WebServer server(80);

/* Go to http://192.168.4.1 in a web browser */

void handleRoot() {
	server.send(200, "text/html", webSite);
}

void handleTest() {
  if(server.args() > 0) {
    Serial.println("Found arguments");
    Serial.print("POST Arguments: " );
    Serial.println(server.args());
    for (int i = 0; i < server.args(); i++){
      Serial.print("Name: "); Serial.println(server.argName(i));
      Serial.print("Value: "); Serial.println(server.arg(i));
    }
  }
}

void handleLed(String(ledStatus)) {
  if(ledStatus == "On") {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("User toggled the LED: On");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("User toggled the LED: Off");
  }
}

void handlePost() {
  if(server.args() > 0) {
    Serial.println("Found arguments");
    Serial.print("POST Arguments: " );
    Serial.println(server.args());
    for(int i = 0; i < server.args(); i++){
      if(server.argName(i) == "ledStatus") {
        handleLed(server.arg(i));
      }
      Serial.print("Name: "); Serial.println(server.argName(i));
      Serial.print("Value: "); Serial.println(server.arg(i));
    }
  }
}


void setup() {
	delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/post", HTTP_POST, handlePost);
  server.on("/handle_test", HTTP_POST, handleTest);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
