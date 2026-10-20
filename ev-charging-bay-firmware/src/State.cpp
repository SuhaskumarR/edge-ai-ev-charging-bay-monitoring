#include "State.h"


String bayStatus = "FREE";
float voltage = 0.0, current = 0.0, power = 0.0, energyWh = 0.0, temperature = 0.0;

unsigned long sessionStartMs = 0;

// Edeg AI variables
float predictedArrivalProb = 0.0;
int predictedDurationMin = 0.0;
int lastHourOfDay = 12;

//Decision Varibales

String loadDecision = "ALLOW";
int throttleLevel = 100;

float predictionThreshold = 0.5;  // its range is 0 to 1 where it is kept initially as 1
int peakTariffStartHr = 18;
int peakTariffEndHr   = 21;
bool overloadActive = false;

int overloadCurrentA = 16;
int maxStationLoadW = 3000;

int DUTY_CYCLE_WINDOW_MS = 2000 ; //2sec duty cycle we are going to apply

bool manualOverrideActive = false;