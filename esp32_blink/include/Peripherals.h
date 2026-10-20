#ifndef PERIPHERALS_H
#define PERIPHERALS_H
#include <DHT.h>

extern DHT dht;


void sample_sensors(void);
void plug_status(void);

void update_led_status(void);
float recentAvgCurrent(void);


#endif