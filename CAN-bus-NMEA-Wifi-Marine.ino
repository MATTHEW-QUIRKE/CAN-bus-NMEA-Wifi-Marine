/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>

#define CONFIG_EEPROM_ADDRESS   100
#define WIFI_SIGNAL_POWER       20.5     // min: 0 - max: 20.5 dBm
#define BOOT_MODE_BUTTON_PIN    0
#define INDICATOR_LED_PIN       2
#define INDICATOR_LED_ON        0
#define INDICATOR_LED_OFF       1
#define RESET_CONFIG_THRESHOLD  5000  // in milliseconds

#define MAX_WIFI_SSID_SIZE      33
#define MAX_WIFI_PASSWORD_SIZE  33
#define MAX_MDNS_HOSTNAME_SIZE  33

// global values

#define HTTP_PORT       80
#define WEBSOCKET_PORT  81
#define DNS_PORT        53

#define WIFI_ACCESS_POINT_CHANNEL 6
#define WIFI_ACCESS_POINT_MAX_CONNECTIONS 4  // max 8
#define WIFI_ACCESS_POINT_DEVICE_IP  IPAddress(10, 1, 1, 1)
#define WIFI_ACCESS_POINT_GATEWAY_IP IPAddress(10, 1, 1, 1)
#define WIFI_ACCESS_POINT_SUBNET_IP  IPAddress(255, 255, 255, 0)


enum WifiMode: uint8_t {
    station = 1,
    access_point = 2,
};

enum TransmitMode: uint8_t {
    broadcast = 1,
    multicast = 2,
    unicast = 3,
};

struct Configuration {
    WifiMode wifi_mode = access_point;
    char wifi_ssid[MAX_WIFI_SSID_SIZE] = "matylda_nmea_bridge";
    char wifi_password[MAX_WIFI_PASSWORD_SIZE] = "matylda99";
    IPAddress static_ip_address = INADDR_ANY;
    char mdns_hostname[MAX_MDNS_HOSTNAME_SIZE] = "nmea_bridge";
    TransmitMode tx_mode = TransmitMode::broadcast;
    IPAddress tx_address = INADDR_ANY;
    uint16_t tx_port = 10110;
    uint16_t rx_port= 10110;
    uint8_t tx_baudrate = 1;
    uint8_t rx_baudrate= 1;
} config;

const char* display_wifi_mode(WifiMode mode) {
    if (mode == WifiMode::station) {
        return "Station";
    } else {
        return "Access Point";
    }
}

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  
  Serial.begin(115200);
  /* remove the password parameter if you want the AP to be open. */
  WiFi.softAP(config.wifi_ssid, config.wifi_password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
