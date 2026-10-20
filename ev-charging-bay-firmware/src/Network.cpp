#include <WiFi.h>
#include "network.h"
#include "rpc.h"
#include "config.h"



WiFiClient espClient;
PubSubClient mqtt(espClient);

// ---------------------------------------------------------------------
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? " connected." : " FAILED (will retry).");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  char buf[256];
  if (length >= sizeof(buf)) length = sizeof(buf) - 1;
  memcpy(buf, payload, length);
  buf[length] = '\0';

  String t = String(topic);
  String requestId = t.substring(t.lastIndexOf('/') + 1);
  handleRpc(requestId, buf);
}

void connectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;
  Serial.print("Connecting to ThingsBoard MQTT...");
  if (mqtt.connect(BAY_ID, TB_TOKEN, "")){
    Serial.println(" connected.");
    mqtt.subscribe("v1/devices/me/rpc/request/+");
    
  } else {
    Serial.print(" failed, rc=");
    Serial.println(mqtt.state());
    delay(1000);
  }
}

// ---------------------------------------------------------------------
// Single MQTT callback, dispatched by topic: attribute push/response vs.
// RPC request. This is where Phase 6's two device-side features live.
// ---------------------------------------------------------------------
