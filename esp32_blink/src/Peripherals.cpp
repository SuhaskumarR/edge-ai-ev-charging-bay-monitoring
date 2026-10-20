#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);

float mapFloat(long x, long inMin, long inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / ((float)(inMax - inMin) + outMin);
}

void sample_sensors(void)
{  
   // raw means direct output of ADC that is from 0 to 4095
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095
    
    //but we want current range- [0,32A]
    //but we want voltage range- [0,250V]

    voltage = mapFloat(raw_voltage,0,4095,0,250);

    if(bayStatus == "CHARGING"){   // this means if not charging current will be zero
    current = mapFloat(raw_current,0,4095,0,32);
    }

    else{
      current = 0;
    }

    //read current and 5 values array
    
    //calculate power
    power = voltage * current;

      power = voltage * current;

      // //calculate energy (Wh) by integrating power over elapsed time
      // static unsigned long lastEnergyMs = millis();
      // unsigned long nowMs = millis();
      // unsigned long deltaMs = nowMs - lastEnergyMs;
      // lastEnergyMs = nowMs;
      
      // if (bayStatus == "CHARGING") {
      //     float deltaHours = deltaMs / 3600000.0f; // ms -> hours
      //     energyWh += power * deltaHours;           // Wh accumulated this tick
      // }

     


    //to read temperature 
    float t = dht.readTemperature();
    if(!(isnan(t))) temperature  = t;

    
   

}

float recentAvgCurrent(void)
{
   float sum =  0;
   //read recent 5  current values
   for(int i=0;i<5;i++)
   {
      sum = sum + current;
   
   }
   return sum/5;
}

bool plugin_flag_once = 1;
bool plugout_flag_once = 1;

void plug_status(void)
{
   bool pluginReading = digitalRead(BTN_PLUGIN);
   // detect the sw is pressed
      if (pluginReading == LOW && plugin_flag_once)
      {
         //session time
         sessionStartMs = millis();
   // plug in switch is pressed
   plugin_flag_once = 0;
   // change bay_status FREE to charging
   if (bayStatus == "FREE")
   {
      bayStatus = "CHARGING";
      //energyWh = 0.0;
      Serial.println("Bay1 plugin is detected, Bay is charging");
      // when there is plug_in turn on relay
      digitalWrite(RELAY_PIN,HIGH);
   }
   //update leds

   }

   if (pluginReading == HIGH)
   {
      plugin_flag_once = 1;
   }



   // plug out switch is pressed
   bool plugoutReading = digitalRead(BTN_PLUGOUT);
   // detect the sw is pressed
      if (plugoutReading == LOW && plugout_flag_once)
      {
   // plug in switch is pressed
   plugout_flag_once = 0;
   // change bay_status charging to free
   if (bayStatus == "CHARGING")
   {
      bayStatus = "FREE";
      Serial.println("Bay1 plugout is detected, Bay is FREE");
   }
   //update leds
   }

   if (plugoutReading == HIGH)
   {
      plugout_flag_once = 1;
   }
   // change bay_status  charging to FREE
   //update leds
}

/*void update_led_status(void)
{
   // if bay _status is charging then turn ON green LED
   if (bayStatus == "FREE")
   {
      digitalWrite(LED_GREEN , HIGH);
      digitalWrite(LED_YELLOW,LOW);
   }
   else{
      // if bay_status is not charging  then turn ON red LED
      digitalWrite(LED_GREEN , LOW);
      digitalWrite(LED_YELLOW,HIGH);
   }




}*/