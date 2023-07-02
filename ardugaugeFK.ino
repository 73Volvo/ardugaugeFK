#include "Arduino.h"
#include "speeduino_data.h"
#include "Pages.h"
#include "Comms.h"
#include "ardugaugeFK.h"

static uint8_t pageNum = 0;

// storage variable for each graph, this is set up to hold four different graphs with the [4] increase that number for more graphs
int16_t graphData[4][108]; // places to store data for the graphs. Could probably use a smaller number for vertical screens
uint8_t graphIndex[4]; // what place are we currently writing for the graph?

// for custom screen
bool customScreen = 0; // flag - is Custom Screen selected?
uint8_t customScreenSelect = 0; // which custom stat is selected?

// screen variables
uint8_t SCREEN_ROTATION = 0; // 0 = 0° normal horizontal, 1 = 90° normal vertical, 2 = 180° flipped horizontal, 3 = 270° flipped vertical

// constant for button
const int8_t ButtonPinR = 2;  // the number of the pushbutton pin
const int8_t ButtonPinL = 1;  // the number of the pushbutton pin

//global variables
int16_t clt; // coolant temp
int16_t iat; // intake air temp
uint8_t bat; // battery voltage
uint8_t baro; // barometric pressure
uint8_t afr; // o2 Sensor Air Fuel Ratio Reading
uint8_t afrT; // AFR target
uint8_t adv; // Advance Degrees
uint16_t MAP; // Manifold Air Pressire
uint16_t pw; // Pulse Width
uint16_t rpm; // RPM
uint16_t tpsdot; // Trottle Position Sensor DOT
uint8_t tae; // AE Table Correction
uint8_t maxAFR = 100;
uint8_t minAFR = 199;
uint8_t ve; // current VE
uint8_t new_ve; // suggested VE

//variables to check engine status
static uint8_t engineStatus = ENGINE_OFF;
static uint8_t engineIdling = 0;
static bool shiftLight = false;

static bool ecuConnected = false;

uint32_t interval = 100; // 10 frames per second

void setup()
{
  pinMode(ButtonPinR, INPUT_PULLUP);
  pinMode(ButtonPinL, INPUT_PULLUP);
  pinMode(0, INPUT);
  Serial.begin(9600); // usb connection on Arduino Nano
  Serial1.begin(115200); // serial connection to Speeduino (D0 and D1)
  initDisplay();
  splashStart();
  endPage();
}

void loop()
{

  // variables for governing consistent framerate
  static uint32_t previousMillis;
  static uint32_t nextTime;
  if (millis() - nextTime >= interval) // only run loop if it's been at least 1/10 of a second since the last loop
  {
    // update next time
    nextTime += interval;
    float currentFrameRate = 1000.0 / (millis() - previousMillis);
    //Serial.print("Current Frame Rate: ");
    //Serial.println(currentFrameRate);
    previousMillis = millis();

    // This just makes a variable called "exampleSignal" that's a sine wave from -1 to 1 used to test out the graphs
    float exampleSignal;
    exampleSignal = sineWave();

    // this grabs a list of stats from the Speeduino and assigns them to the variables above
    // if you're in "TEST" mode (defined in ardugauge.h) it assigns some pre-set stats
  grabStats();

    // Record stats for the graphs
    recordGraph(exampleSignal, 0, 1); // settings are:  (variable to record, graph number, graph speed from 1-10 higher is slower)
    recordGraph(afr/10.0, 1, 1);
    recordGraph(afr/10.0, 2, 3);
    recordGraph(afrT/10.0, 3, 1);

    // sweep the tach, why not?
    tachSweep();

    // Button operation
    buttonCheck();
    
    checkEngineStatus(); // checks if the engine is off, idling, or running, and also checks if shift light should be lit

    startPage(shiftLight); // if the shiftlight is on, draws an inverse rectangle over the while screen

    ecuConnected = requestData(50); // is the Speeduino connected?

    #ifdef TEST  
      ecuConnected = true; // if we're in testing mode, let's pretend a Speediuino is connected.
    #endif

    if(!ecuConnected) { // if Speeduino isn't connected, print the "no connection" page
        pageNoECU();
    }

    else // if the Speeduino is connected, show the pages of gauges!
    {  
      if (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2){
      switch(pageNum)   // this part decides what page is displayed in the horizontal view
      {
        case 0:
          // two circle gauges
          // ( x position, y position, diameter, start angle, end angle, gradiations, stat value, start range, end range, redline, numbers switch )
          drawCircleGauge( 64, 64, 62 , 170, 10, 9, rpm, 0, 8000, 6500, 1);
          drawCircleGauge( 64, 64, 40 , 170, 10, 5, clt, 125, 230, 212, 0);
          break;
        case 1:
          // display the current values as stats on the screen using the "twoStats" function
          // settings for "threeStats" ( which row - can be a decimal, name1, variable1, max1, min1, name2, variable2, min2, max2, name3, variale3, min3, max3)
          // If the name is set to "0" the stat in that space will not display
          twoStats(1, "WATER", clt, 0, 32, 212, "AIR", iat, 0, 32, 110);
          twoStats(2, "MAP", MAP, 0, 0, 100, "AFR" , afr, 1 , 0, 20);
          twoStats(3, "ADV", adv, 0, 0, 60, "AFR T", afrT, 1, 10, 20);
          twoStats(4, "WATER", clt, 0, 32, 212, "AIR", iat, 0, 32, 110);
          break;
        case 2:
          // display the current values as stats on the screen using the "threeStats" function
          // settings for "threeStats" ( which row - can be a decimal, name1, variable1, min1, max1, name2, variable2, min2, max2, name3, variale3, min3, max3)
          // If the name is set to "0" the stat in that space will not display
          threeStats(1, "WATER", clt, 0, 32, 212, "AIR", iat, 0, 32, 110, "BAT", bat, 1, 10, 15);
          threeStats(2, "MAP", MAP, 0, 0, 100, "AFR" , afr, 1 , 0, 20, "BARO", baro, 0, 10, 110);
          threeStats(3, "ADV", adv, 0, 0, 60, "AFR T", afrT, 1, 10, 20, "PW", pw, 1, 0, 20);
          break;
        case 3:
          threeStats(1, "WATER", clt, 0, 32, 212, "AIR", iat, 0, 32, 110, "AFRT", afrT, 1, 10, 20);
          bottomRow_AFR();
          break;
        case 4:
          twoStats(1, "WATER", clt, 0, 32, 212, "AIR", iat, 0, 32, 110);
          bottomRow("RPM", rpm , 0);
          break;
        case 5:
          // Tuning Page
          twoStats(1, "RPM", rpm, 0, 600, 6500, "MAP", MAP, 0, 0, 100);
          twoStats(2.5, "T AFR", afrT, 1, 10, 20, "AFR" , afr, 1 , 10, 20);
          twoStats(4, "CALC", new_ve, 0, 0, 256, "VE", ve, 0, 0, 256);
          break;
        case 6:
          //AE Page
          twoStats(1, "RPM", rpm, 0, 600, 6500, "%/S", tpsdot, 0, 0, 100);
          twoStats(2.5, "AE%", tae, 0, 0, 100, "TGT" , afrT, 1 , 10, 20);
          twoStats(4, "MAX", maxAFR, 1, 1, 20, "MIN", minAFR, 1, 1, 20);
          break;
        case 7:
          // three circle gauges, showing different placements and sweeps
          drawCircleGauge( 0, 32, 31 , -70, 70, 6, 20+(exampleSignal*10), 0, 50, 35, 0);
          drawCircleGauge( 64, 32, 25 , 250, -70, 12, 20+(exampleSignal*10), 0, 50, 45, 0);
          drawCircleGauge( 128, 32, 31 , 250, 110, 6, 20+(exampleSignal*10), 0, 50, 35, 0);
          break;
        case 8:
          // displayGraph function displays the graphs
          // settings: (vertical height default 0, graph number, min, max, name1, variable1, decimal1, max1, min1, name2, variable2, decimal2, min2, max2 )
          displayGraph(0, 1, 10, 20, "AFR" , afr, 1, 10, 20, "afrT", afrT, 1, 10, 20);
          displayGraph2(0, 3, 10, 20); // second dotted line graph
          break;
        case 9:
          // fast AFR graph, speed is adjusted above when the AFR signal is capured with the "recordGraph" function
          displayGraph(0, 1, 10, 20, "AFR" , afr, 1, 10,20, "SINE", exampleSignal*10.0, 1, -1, 1);
          displayGraph2(0, 0, -1, 1); // second dotted line graph  - sine graph
          break;
        case 10:
          // slow AFR
          displayGraph(0, 2, 10, 20, "AFR" , afr, 1, 10,20, "0", 0, 1, -1, 1); // slow AFR graph
          break;
        case 11:
          displayGraph(0, 3, 10, 20, "afrT" , afrT, 1, 10, 20, "0", 0, 0, 0, 0);
          break;
        case 12:
          // simple customizable display page
          customGauge(customScreenSelect);
        default:
          break;
        }
      }

      if (SCREEN_ROTATION == 1 || SCREEN_ROTATION == 3){
        switch(pageNum)   // this part decides what page is displayed in the vertical view
        {
        case 0:
          // two circle gauges
          // ( x position, y position, diameter, start angle, end angle, gradiations, stat value, start range, end range, redline, numbers switch )
          drawCircleGauge( 64, 64, 62 , 260, 100, 9, rpm, 0, 8000, 6500, 1);
          drawCircleGauge( 64, 64, 40 , 260, 100, 5, clt, 125, 230, 212, 0);
          break;
        case 1:
          oneStat(1, "WATER", clt, 0, 32, 212);
          oneStat(1.5, "AIR", iat, 0, 32, 110);
          oneStat(2, "MAP", MAP, 0, 0, 100);
          oneStat(2.5, "AFR", afr, 1, 0, 20);
          oneStat(3, "ADV", adv, 0, 0, 60);
          oneStat(3.5, "TAFR", afrT, 1, 10, 20);
          oneStat(4, "WATER", clt, 0, 32, 212);
          oneStat(4.5, "AIR", iat, 0, 32, 110);
          break;
        case 2:
          // vertical page example - WILL NOT DISPLAY PROPERLY WITH STANDARD SCREEN ROTATION!
          // can fit eight stats or a graph and four stats
          displayGraph(0, 1, 10, 20, "AFR" , afr, 1, 10, 20, "0", 0, 0, 0, 0);
          displayGraph2(0, 3, 10, 20); // second dotted line graph
          oneStat(3, "RPM", rpm, 0, 600, 6500);
          oneStat(3.5, "TAFR", afrT, 1, 10, 20);
          oneStat(4, "CALC", new_ve, 0, 0, 256);
          oneStat(4.5, "MAX", afr, 1, 1, 20);
          break;
        case 3:
          displayGraph(0, 0, -1, 1, "SINE" , exampleSignal*10.0, 1, -1, 1, "0", 0, 0, 0, 0);
          displayGraph(1, 0, -1, 1, "SINE" , exampleSignal*10.0, 1, -1, 1, "0", 0, 0, 0, 0);
          break;
        case 4:
          oneStat(2, "WATER", clt, 0, 32, 212);
          oneStat(2.75, "AIR", iat, 0, 32, 110);
          oneStat(3.5, "RPM", rpm, 0, 0, 6500);
          break;
        case 5:
          // Tuning Page
          oneStat(1, "RPM", rpm, 0, 600, 6500);
          oneStat(1.7, "MAP", MAP, 0, 0, 100);
          oneStat(2.4, "TAFR", afrT, 1, 10, 20);
          oneStat(3.1, "AFR", afr, 1, 10, 20);
          oneStat(3.8, "CALC", new_ve, 0, 0, 256);
          oneStat(4.5, "VE", ve, 0, 0, 256);
          break;
        case 6:
          // AE Page
          oneStat(1, "RPM", rpm, 0, 600, 6500);
          oneStat(1.7, "%S", tpsdot, 0, 0, 100);
          oneStat(2.4, "AE%", tae, 0, 0, 100);
          oneStat(3.1, "TGT", afrT, 1, 10, 20);
          oneStat(3.8, "MAX", maxAFR, 1, 1, 20);
          oneStat(4.5, "MIN", minAFR, 1, 1, 20);
          break;
        case 7:
          // three circle gauges, showing different placements and sweeps
          drawCircleGauge( 32, 36 , 31 , 220, -40, 12, 20+(exampleSignal*10), 0, 50, 45, 0);
          drawCircleGauge( 32, 100, 31 , 220, -40, 12, 20+(exampleSignal*10), 0, 50, 45, 0);
          break;
        case 8:
          // displayGraph function displays the graphs
          // settings: (vertical height default 0, graph number, min, max, name1, variable1, decimal1, max1, min1, name2, variable2, decimal2, min2, max2 )
          displayGraph(0, 0, -1, 1, "SINE" , exampleSignal*10.0, 1, -1, 1, "0", 0, 0, 0, 0);
          oneStat(3, "RPM", rpm, 0, 600, 6500);
          oneStat(3.5, "TAFR", afrT, 1, 10, 20);
          oneStat(4, "CALC", new_ve, 0, 0, 256);
          oneStat(4.5, "MAX", afr, 1, 1, 20);
          break;
        case 9:
          // fast AFR graph, speed is adjusted above when the AFR signal is capured with the "recordGraph" function
          displayGraph(0, 1, 10, 20, "AFR" , afr, 1, 10,20, "0", 0, 0, 0, 0);
          // slow AFR
          displayGraph(1, 2, 10, 20, "AFR" , afr, 1, 10,20, "0", 0, 0, 0, 0);
          break;
        case 10:
          oneStat(1, "RPM", rpm, 0, 600, 6500);
          oneStat(1.5, "TAFR", afrT, 1, 10, 20);
          oneStat(2, "CALC", new_ve, 0, 0, 256);
          oneStat(2.5, "MAX", afr, 1, 1, 20);
          oneStat(3, "RPM", rpm, 0, 600, 6500);
          oneStat(3.5, "TAFR", afrT, 1, 10, 20);
          oneStat(4, "CALC", new_ve, 0, 0, 256);
          oneStat(4.5, "MAX", afr, 1, 1, 20);
          break;
        case 11:
          displayGraph(0, 3, 10, 20, "afrT" , afrT, 1, 10, 20, "0", 0, 0, 0, 0);
          oneStat(3, "RPM", rpm, 0, 600, 6500);
          oneStat(3.5, "TAFR", afrT, 1, 10, 20);
          oneStat(4, "CALC", new_ve, 0, 0, 256);
          oneStat(4.5, "MAX", afr, 1, 1, 20);
          break;
        case 12:
          // simple customizable display page
          customGauge(customScreenSelect);
        default:
          break;
        }
      }
    }

    endPage(); // writes the latest changes to the screen
  }

}
