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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND ConTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ConTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR ConSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTIon) HOWEVER CAUSED AND on
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#include "mypage.h"


/* Set these to your desired credentials. */
const char *ssid = "Wemos";
const char *password = "nospoons";

String myHeader = MAIN_header;
String myFooter = MAIN_footer;
String myIndex = MAIN_index;
String myTest = MAIN_test;

String testFormat = "This {} a {} format";
String testArr[] = {"is", "test"};
int testArrLen = 2;

ESP8266WebServer server(80);

/* Go to http://192.168.4.1 in a web browser */

void handleRoot() {
  server.send(200, "text/html", String(myHeader + myIndex + myFooter));
}

void handleTestPage() {
  server.send(200, "text/html", String(myHeader + myTest + myFooter));
}

void handleTest() {
  if(server.args() > 0) {
    Serial.print("POST Arguments: " );
    Serial.println(server.args());
    for (int i = 0; i < server.args(); i++){
      Serial.print("Name: "); Serial.println(server.argName(i));
      Serial.print("Value: "); Serial.println(server.arg(i));
    }
  }
}

void handleOnOff(String statusHighLow, String statusPin) {
  if(statusPin == "builtin") {
    if(statusHighLow == "HIGH") {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    if(statusPin == "4") {
      Serial.println("Make pin 4 code");
    }
  }
}

void handlePost() {
  if(server.args() > 0) {
    Serial.print("POST Arguments: " );
    Serial.println(server.args());
    for(int i = 0; i < server.args(); i++){
      if(server.argName(i) == "digitalStatus") {
        handleOnOff(server.arg(i), server.arg(i+1));
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
  server.on("/test_arguments", handleTestPage);
  server.on("/post", HTTP_POST, handlePost);
  server.on("/handle_test", HTTP_POST, handleTest);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
