#include "Arduino.h"

/* OIL TEMPERATURE SENSOR ON PIN A0

ADC values are calculated as  ADC = ADCmax / Vcc * Vsens
                      where   Vsens = Vcc / (Rsens + Rbias) * Rsens
Entries in the oilADC array are calculated with Vcc = 5V and Rbias = 2700 ohms            

*/
#define OIL_TABLE_SIZE 13
#define OILTEMP_FILTER 16
#define ADC_NO_CONN    1000 
#define ADC_COLD       500 

//#define RAW_ADC

const uint16_t oilADC[OIL_TABLE_SIZE] =     {15,   18,    23,    29,    38,    50,    66,   87,  117, 158, 213, 288, 504};
const uint8_t oilDegrees[OIL_TABLE_SIZE] = {150,   140,   130,   120,   110,   100,   90,   80,  70,  60,  50,  40,  20};

uint8_t getOilTemp()
{
  static uint16_t adcValue = 0;
  static bool oilCold = true;
  uint8_t i = 1;
  uint8_t oilLast;

  adcValue  = analogRead(A0);

  #ifdef RAW_ADC
    return adcValue / 4;
  #endif

  if(oilCold && (adcValue < ADC_COLD))  
    oilCold = false; // Oil is considered cold until it reaches a given temperature

  if(adcValue > ADC_NO_CONN) return 255; // Sensor not connected
  if(oilCold) return 0; // Oil is cold

  if(adcValue <= oilADC[0])
    oilLast = oilDegrees[0]; // Return highest value
  else
  {
    while(adcValue > oilADC[i]) i++;

    if(adcValue == oilADC[i])
      oilLast = oilDegrees[i];
    else
      oilLast = oilDegrees[i] + (oilDegrees[i-1] - oilDegrees[i]) * (oilADC[i] - adcValue) / (oilADC[i] - oilADC[i-1]);
  }

  static uint16_t oilSmooth = oilLast * OILTEMP_FILTER;
  if(oilSmooth / OILTEMP_FILTER < oilLast)
    oilSmooth++;
  else if(oilSmooth / OILTEMP_FILTER > oilLast)
    oilSmooth--;

  return oilSmooth / OILTEMP_FILTER;
}