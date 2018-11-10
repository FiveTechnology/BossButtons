/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial

WiFiMulti wifiMulti;

const int button1Pin = 4; 
const int light1Ground = 15;
int buttonPressed = 0;

void setup() {

    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    // Built In LED
    pinMode(2, OUTPUT);
    pinMode(light1Ground, OUTPUT);
    digitalWrite(light1Ground, HIGH);
      
    for (uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        digitalWrite(2, HIGH);
        delay(1000);
        digitalWrite(2, LOW);
    }

     wifiMulti.addAP("bute-guest", "password1234");
}

void loop() {
    // wait for WiFi connection
    if ((wifiMulti.run() == WL_CONNECTED)) {
     
      if (digitalRead(button1Pin)) {
        buttonPressed = 1;
        digitalWrite(2, HIGH);
        USE_SERIAL.print("Button 1 Pressed\n");
      }

      if (buttonPressed) {
        // Toggle the button LED
        // TODO: This should be specified by the HTTP response
        digitalWrite(light1Ground, LOW);
        
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        char url[80];  //buffer used to format a line (+1 is for trailing 0)

        sprintf(url,"https://dustin.theboss.solutions/res/staff_planner/BossButtons.php?button=%d",buttonPressed); 
        http.begin(url);
        
        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
               String payload = http.getString();
               USE_SERIAL.println(payload);
            }
        } else {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
      }
    }
    buttonPressed = 0;
    digitalWrite(2, LOW);
}
