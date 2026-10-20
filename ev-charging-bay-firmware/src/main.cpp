#include <Arduino.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include <WiFi.h>
#include "Network.h"
#include "Telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"


void setup()
{

    //initialise serial monitor
    Serial.begin(115200);
    dht.begin();  // initialise sesnor
    //config esp32 with real time
    configTime(0,0,"pool.ntp.org","time.nist.gov");
    //initialise peripheral pins
   pinMode(BTN_PLUGIN , INPUT_PULLUP);
   pinMode(BTN_PLUGOUT , INPUT_PULLUP);
   pinMode(RELAY_PIN , OUTPUT);
   pinMode(LED_GREEN ,OUTPUT);
   pinMode(LED_YELLOW ,OUTPUT);
   pinMode(LED_RED ,OUTPUT);

   //connect board to wifi
   connectWiFi();

   //config mqtt server
   // here we are passig mqtt server address of thingsboard
    mqtt.setServer(MQTT_SERVER, MQTT_PORT); 


    mqtt.setServer(MQTT_SERVER, MQTT_PORT); 
    mqtt.setCallback(mqttCallback);   // add this line


   //connect board to server
   // here we are passing ACCESS TOKEN and device ID
   connectMQTT();
   
}

unsigned long now;
unsigned long last_print;

void loop()
{
    mqtt.loop();
     // push it to cloud for every 5sec
    now = millis();
    if((now - last_print) > 2000)
    {
        last_print = now;
        // read data from sensors //voltage,current,temperature,power,bay_status
        sample_sensors();

        //run AI to get the prediction 
        runEdgeAIInference();

        //decide load based on predictions made
        runOptimization();


        //publish the data
        publishTelemetry();

        

    }

    //calling the switch status function
    plug_status();
    updateLeds();
   
}

