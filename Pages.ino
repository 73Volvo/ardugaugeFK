#include "Arduino.h"
#include "Pages.h"
#include "Comms.h"
#include <Adafruit_SH110X.h> //for 2.42" 1309 screen - needed to tweak the SH1106G file because it had a 2 pixel offset
//#include <Adafruit_SSD1306.h> // for .96" 1306 screen
#include "Numbers24pt7b.h"
#include "FreeMono9pt7b.h"
#include "splash.h"

// Settings for OLED Screen
Adafruit_SH1106G OLED(128, 64, &Wire, -1); // for 2.42 1309 screen
//Adafruit_SSD1306 OLED(128, 64, &Wire, -1); // for .96" 130
int SCREEN_WIDTH = 128; // 128 for horizontal, 64 for vertical
int SCREEN_HEIGHT = 64; // 64 for horizontal, 128 for vertical
int INVERSE = 2; // for 2.42" screen
int WHITE = 1; // for 2.42" screen
int BLACK = 0; // for 2.42" screen

// for circle gauges
#define DEG_TO_RAD(deg) ((deg) * 3.14159 / 180)

#define CUSTOM_SCREEN_PAGE_COUNT 75

void initDisplay()
{
  uint8_t i;
  
  OLED.begin(0x3c, true); // for 2.42" display
  //OLED.begin(SSD1306_SWITCHCAPVCC, 60); // for .96" display
  OLED.setFont();
  OLED.setTextColor(INVERSE);
  OLED.clearDisplay();
  OLED.setTextWrap(0);
  OLED.setRotation(SCREEN_ROTATION);
}

void splashStart(){
  OLED.clearDisplay();
  int16_t centerX = (SCREEN_WIDTH - 64) / 2;
  int16_t centerY = (SCREEN_HEIGHT - 64) / 2;
  int16_t width = 64;
  int16_t height = 64;

  for (uint8_t numColumns = 2; numColumns <= 64; numColumns *= 2) {
    drawPartialBitmap(centerX, centerY, logoDesign64, width, height, numColumns);
    OLED.display();
    delay(50);
    }
  delay(1000);
}

void startPage(bool shiftLightOn)
{
  OLED.clearDisplay();
  //Drawing a INVERSE rectangle over the whole display will invert all graphics
  if(shiftLightOn)
    OLED.fillRect(0, 0, 128, 64, INVERSE);
}

void endPage()
{
  OLED.display();
}

float sineWave(){
  // example to make a quick sine wave signal
  float counter;
  counter = millis()/1000.0;
  return sin(counter);
}

void tachSweep(){
  int rpmMax = 8000;
  int cltMax = 230;
  static int rpmMod = rpm;
  static int cltMod = clt;
  static int steps = 0;
  static int sweepFlag = 0;
  
  if ( sweepFlag == 0) {
      rpm = easing(rpmMod, rpmMax, 0.3, 12, steps);
      clt = easing(cltMod, cltMax, 0.5, 12, steps);
      rpmMod = rpm;
      cltMod = clt;
      steps++;
  }
  if (steps >= 12 && sweepFlag == 0) {
      steps = 0;
      sweepFlag = 1;
  }
  if ( sweepFlag == 1) {
      rpm = easing(rpmMod, rpm, 0.5, 18, steps);
      clt = easing(cltMod, clt, 0.8, 18, steps);
      rpmMod = rpm;
      cltMod = clt;
      steps++;
  }
  if (steps >= 18 && sweepFlag == 1) {
      sweepFlag = 2;
  }
}

void buttonCheck() {
  static uint32_t RbtnPressed = 0;
  static uint32_t RbtnStartTime = 0;
  static uint32_t LbtnPressed = 0;
  static uint32_t LbtnStartTime = 0;

  if (!digitalRead(ButtonPinL)) { // Button is pressed, both buttons do the same thing
    if (LbtnPressed == 0) {
      LbtnPressed = millis(); // start timer
      LbtnStartTime = millis(); // record start time for the button press
    }
  } else {
    if (LbtnPressed) {
      uint32_t LbtnDuration = millis() - LbtnStartTime; // calculate the duration of the button press

      // Check for long press (1 second or more)
      if (LbtnDuration >= 1000) {
        if (LbtnDuration >= 2000) {
          // 2 - second action
          Serial.println("2-second left button press");
          int SWAP = SCREEN_WIDTH;
          SCREEN_WIDTH = SCREEN_HEIGHT;
          SCREEN_HEIGHT = SWAP;
          Serial.print("W: ");
          Serial.println(SCREEN_WIDTH);
          Serial.print("H: ");
          Serial.println(SCREEN_HEIGHT);
            
            if (SCREEN_ROTATION == 0) {
            SCREEN_ROTATION = 3;
            }
            else SCREEN_ROTATION--;
          OLED.setRotation(SCREEN_ROTATION);
        } else {
          // 1 - second action
          Serial.println("1-second left button press");
          if (customScreen == 1) {
            customScreen = 0;
            pageNum = 0;
          } else {
            pageNum = LAST_PAGE;
            customScreen = 1;
          }
        }
      } else {
        // Short press
            Serial.println("short left button press");
            if (customScreen == 1) {
              if (customScreenSelect == 0) {
              customScreenSelect = 75-1;
              }
              else customScreenSelect--; // constrains value between 0 and 75

            } else {
              if (pageNum == 0) {
              pageNum = LAST_PAGE-1;
              }
              else pageNum--; // constrains value between 0 and LAST_PAGE

            }
      }
    }
    LbtnPressed = 0;
    Serial.print("customScrteenSelect: ");
    Serial.println(customScreenSelect);
  }

                

  if (!digitalRead(ButtonPinR)) { // Button is pressed, both buttons do the same thing
    if (RbtnPressed == 0) {
      RbtnPressed = millis(); // start timer
      RbtnStartTime = millis(); // record start time for the button press
    }
  } else {
    if (RbtnPressed) {
      uint32_t RbtnDuration = millis() - RbtnStartTime; // calculate the duration of the button press

      // Check for long press (1 second or more)
      if (RbtnDuration >= 1000) {
        if (RbtnDuration >= 2000) {
          // 2 - second action
          Serial.println("2-second right button press");
          int SWAP = SCREEN_WIDTH;
          SCREEN_WIDTH = SCREEN_HEIGHT;
          SCREEN_HEIGHT = SWAP;
          Serial.print("W: ");
          Serial.println(SCREEN_WIDTH);
          Serial.print("H: ");
          Serial.println(SCREEN_HEIGHT);
          SCREEN_ROTATION = (SCREEN_ROTATION + 1) % 4;
          OLED.setRotation(SCREEN_ROTATION);
        } else {
          // 1 - second action
          Serial.println("1-second right button press");
          if (customScreen == 1) {
            customScreen = 0;
            pageNum = 0;
          } else {
            pageNum = LAST_PAGE;
            customScreen = 1;
          }
        }
      } else {
        // Short press
            Serial.println("short right button press");
            if (customScreen == 1) {
              customScreenSelect = (customScreenSelect + 1) % ( 75 ); // constrains value between 0 and 75
            } else {
              pageNum = (pageNum + 1) % LAST_PAGE; // constrains value between 0 and LAST_PAGE
            }
      }
    }
    RbtnPressed = 0;
  }
}

// Display one stats per row
void oneStat(float row, String title1, float stat1, int decimal1, int min1, int max1)
{
  char valString[8];
  int16_t x1, y1;
  uint16_t w, h;
  int LROffset = 3;
  int height = ( SCREEN_HEIGHT*( (row-1)/4.0) );// + row;
  int celHeight = SCREEN_HEIGHT*0.25;
  int celWidth = SCREEN_WIDTH*0.33;
  int decimalMod = 1;

  // Print 1
  OLED.setTextSize(0);
  OLED.getTextBounds(title1,0,0,&x1, &y1, &w, &h);
  OLED.setCursor( SCREEN_WIDTH-w , height );
  OLED.print(title1);
  OLED.setFont(&FreeMono9pt7b);
  OLED.setCursor(1-LROffset, height+12);
  formatValue(valString, stat1, decimal1);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  OLED.print(valString);
  OLED.setFont();
  if (decimal1 == 0) decimalMod = 1;
  else decimalMod = 10;
  if(stat1 < min1*decimalMod) OLED.fillRoundRect( 0 , (celHeight*(row-1))+1, w + 2, h + 2 , 2, INVERSE);
  if(stat1 > max1*decimalMod) OLED.fillRoundRect( 0 , (celHeight*(row-1))+1, w + 2, h + 2 , 2, INVERSE);
}

// Display two stats per row
void twoStats(float row, String title1, float stat1, int decimal1, int min1, int max1, String title2, float stat2, int decimal2, int min2, int max2)
{
  char valString[8];
  int16_t x1, y1;
  uint16_t w, h;
  int LROffset = 3;
  int height = ( SCREEN_HEIGHT*( (row-1)/4.0) );// + row;
  int celHeight = SCREEN_HEIGHT*0.25;
  int celWidth = SCREEN_WIDTH*0.33;
  int decimalMod = 1;

  // Print 1
  if (title1 != "0"){
  OLED.setTextSize(0);
  OLED.getTextBounds(title1,0,0,&x1, &y1, &w, &h);
  OLED.setCursor( (SCREEN_WIDTH/2)-(w*.75) , height );
  OLED.print(title1);
  OLED.setFont(&FreeMono9pt7b);
  OLED.setCursor(1-LROffset, height+12);
  formatValue(valString, stat1, decimal1);
  OLED.print(valString);
  OLED.setFont();
  if (decimal1 == 0) decimalMod = 1;
  else decimalMod = 10;
  if(stat1 < min1*decimalMod) OLED.fillRoundRect( 0 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  if(stat1 > max1*decimalMod) OLED.fillRoundRect( 0 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  }

  // Print 2
  if (title2 != "0"){
  OLED.setTextSize(0);  
  OLED.getTextBounds(title2,0,0,&x1, &y1, &w, &h);
  OLED.setCursor( (SCREEN_WIDTH/2)-(w*.25), height+8 );
  OLED.print(title2);
  OLED.setFont(&FreeMono9pt7b);
  formatValue(valString, stat2, decimal2);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  OLED.setCursor(SCREEN_WIDTH-w-LROffset, height+12);
  OLED.print(valString);
  OLED.setFont();
  if (decimal2 == 0) decimalMod = 1;
  else decimalMod = 10;
  if(stat2 < min2*decimalMod) OLED.fillRoundRect( (celWidth*2)+2 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  if(stat2 > max2*decimalMod) OLED.fillRoundRect( (celWidth*2)+2 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  }
}


// Display three stats per row
void threeStats(float row, String title1, float stat1, int decimal1, int min1, int max1, String title2, float stat2, int decimal2, int min2, int max2, String title3, float stat3, int decimal3, int min3, int max3)
{
  char valString[8];
  int16_t x1, y1;
  uint16_t w, h;
  int LROffset = 3;
  int height = ( SCREEN_HEIGHT*( (row-1)/3.0) ) + row;
  int celHeight = SCREEN_HEIGHT*0.33;
  int celWidth = SCREEN_WIDTH*0.33;
  int decimalMod = 1;

  // Print 1
  if (title1 != "0"){
  OLED.setTextSize(0);
  OLED.getTextBounds(title1,0,0,&x1, &y1, &w, &h);
  OLED.setCursor( 1 , height );
  OLED.print(title1);
  OLED.setFont(&FreeMono9pt7b);
  OLED.setCursor(1-LROffset, height+18);
  formatValue(valString, stat1, decimal1);
  OLED.print(valString);
  OLED.setFont();
  if (decimal1 == 0) decimalMod = 1;
  else decimalMod = 10;
  if(stat1 < min1*decimalMod) OLED.fillRoundRect( 0 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  if(stat1 > max1*decimalMod) OLED.fillRoundRect( 0 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  }

  // Print 2
  if (title2 != "0"){
  OLED.setTextSize(0);
  OLED.getTextBounds(title2,0,0,&x1, &y1, &w, &h);
  OLED.setCursor( ((SCREEN_WIDTH/2)-(w/2)) , height );
  OLED.print(title2);
  OLED.setFont(&FreeMono9pt7b);
  formatValue(valString, stat2, decimal2);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  OLED.setCursor((SCREEN_WIDTH/2)-(w/2)-LROffset, height+18);
  OLED.print(valString);
  OLED.setFont();
  if (decimal2 == 0) decimalMod = 1;
  else decimalMod = 10;
  if(stat2 < min2*decimalMod) OLED.fillRoundRect( celWidth+1 , (celHeight*(row-1))+1, celWidth, celHeight , 2 , INVERSE);
  if(stat2 > max2*decimalMod) OLED.fillRoundRect( celWidth+1 , (celHeight*(row-1))+1, celWidth, celHeight , 2  , INVERSE);
  int middle = w/2;
  }

  // Print 3
  if (title3 != "0"){
  OLED.setTextSize(0);  
  OLED.getTextBounds(title3,0,0,&x1, &y1, &w, &h);
  OLED.setCursor( SCREEN_WIDTH-w, height );
  OLED.print(title3);
  OLED.setFont(&FreeMono9pt7b);
  formatValue(valString, stat3, decimal3);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  OLED.setCursor(SCREEN_WIDTH-w-LROffset, height+18);
  OLED.print(valString);
  OLED.setFont();
  if (decimal3 == 0) decimalMod = 1;
  else decimalMod = 10;
  if(stat3 < min3*decimalMod) OLED.fillRoundRect( (celWidth*2)+2 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  if(stat3 > max3*decimalMod) OLED.fillRoundRect( (celWidth*2)+2 , (celHeight*(row-1))+1, celWidth, celHeight, 2, INVERSE);
  }
}

// Display AFR on the bottom row
void bottomRow_AFR()
{
  char valString[8];
  int8_t i;

  OLED.setCursor(16, 56);
  OLED.setTextSize(0);
  OLED.print(F("AFR"));
  OLED.setCursor(32, 56);
  OLED.setFont(&Numbers24pt7b);
  formatValue(valString, afr, 1);
  OLED.print(valString);
  OLED.setFont();

  // EGO Correction indicators
  OLED.drawFastVLine(1, 48 - EGO_AUTHORITY * EGO_TICK_PIXELS, 2 * EGO_AUTHORITY * EGO_TICK_PIXELS + 1, INVERSE);
  for(i = -EGO_AUTHORITY; i <= EGO_AUTHORITY; i++)
    OLED.drawFastHLine(2, 48 + (i * EGO_TICK_PIXELS), 3, INVERSE);
  i = (getByte(SPEEDUINO_EGOCORR_BYTE) - 100) * EGO_TICK_PIXELS;
  OLED.fillTriangle(8, 48 - i, 12, 44 - i, 12, 52 - i, INVERSE);

  // AE/DE indicator
  if(getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_TPSAE_BIT) || getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_MAPAE_BIT))
    OLED.fillTriangle(20, 46, 24, 38, 28, 46, INVERSE);
  if(getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_TPSDE_BIT) || getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_MAPDE_BIT))
    OLED.fillTriangle(20, 50, 24, 58, 28, 50, INVERSE);
}

// Display Big Number on the bottom row
void bottomRow(String title, int variable, int decimal)
{
  char valString[8];
  int16_t x1, y1;
  uint16_t w, h;

  OLED.setCursor(1, 56);
  OLED.setTextSize(0);
  OLED.print(title);
  OLED.setFont(&Numbers24pt7b);
  formatValue(valString, variable, decimal);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  OLED.setCursor(SCREEN_WIDTH-w-5, 62);
  OLED.print(valString);
  OLED.setFont();

}

// grabStats function grabs the values from Speeduino and assigns them to variables used by the gauge
// it's also a place to calculate custom stats
void grabStats()
{
  static uint16_t maxTPSDOT = 0;
  static uint8_t maxTAE = 100;
  static uint32_t lastTPSDOT = millis();
  static uint16_t taeRPM;
  static uint8_t oilTemp = getOilTemp(); // for analog temperature sensor

  // variables for standard stats from Speeduino
  clt = (int16_t) ( (getByte(SPEEDUINO_CLT_BYTE) + SPEEDUINO_TEMPERATURE_OFFSET ) * SPEEDUINO_TEMPERATURE_MULTIPLIER ) + C_TO_F;
  iat = (int16_t) ( (getByte(SPEEDUINO_IAT_BYTE) + SPEEDUINO_TEMPERATURE_OFFSET ) * SPEEDUINO_TEMPERATURE_MULTIPLIER ) + C_TO_F;
  bat = getByte(SPEEDUINO_VOLTAGE_BYTE);
  baro = getByte(SPEEDUINO_BARO_BYTE);
  afr = getByte(SPEEDUINO_AFR_BYTE);
  afrT = getByte(SPEEDUINO_AFRTARGET_BYTE);
  adv = getByte(SPEEDUINO_ADVANCE);
  MAP = getWord(SPEEDUINO_MAP_WORD);
  pw = getWord(SPEEDUINO_PW1_WORD) / 100;
  rpm = getWord(SPEEDUINO_RPM_WORD);
  tpsdot = getByte(SPEEDUINO_TPSDOT_BYTE) * 10;
  tae = getByte(SPEEDUINO_AECORR_BYTE);
  ve = getByte(SPEEDUINO_CURRENTVE_BYTE) * 100 / getByte(SPEEDUINO_GAMMAE_BYTE); // current VE
  new_ve = ve * afr / afrT; // suggested VE
  
  if(tpsdot > maxTPSDOT)
  {
	// AE event
    maxTPSDOT = tpsdot;
	  taeRPM = rpm;
	  maxTAE = tae;
	  if(afr > maxAFR)
		  maxAFR = afr;
    if(afr < minAFR)
		  minAFR = afr;
	
    lastTPSDOT = millis();
  }
  else if(millis() > lastTPSDOT + AE_MONITOR_MS)
  {
    // AE Monitoring ended
	  maxTPSDOT = 0;
	  maxAFR = 100;
    minAFR = 199;
	
    lastTPSDOT = millis();
  }
  else if(maxTPSDOT > 0)
  {
    // AE occurred, and we are monitoring AFR and TAE
    if(afr > maxAFR)
      maxAFR = afr;
    if(afr < minAFR)
		  minAFR = afr;
    if(tae > maxTAE)
      maxTAE = tae;

    // Display the values that we saved then we had the tpsDOT event, instead of "live" values
    rpm = taeRPM;
    tae = maxTAE;
  }

    // TEST - if "TEST" is un-commented in ardugauge.h you can use these variables as stand-ins to test the fuctions while disconnected from the Speeduino
  #ifdef TEST
    ecuConnected = true;
    bitSet(buffer[SPEEDUINO_ENGINE_BITFIELD], SPEEDUINO_ENGINE_RUNNING_BIT);
    bitSet(buffer[SPEEDUINO_ENGINE_BITFIELD], SPEEDUINO_ENGINE_WARMUP_BIT);
    #define TEST_RPM 1200
    buffer[SPEEDUINO_RPM_WORD] = TEST_RPM % 256;
    buffer[SPEEDUINO_RPM_WORD+1] = TEST_RPM / 256;
    //shiftLight = true;
    #define TEST_MAP 50
    buffer[SPEEDUINO_MAP_WORD] = TEST_MAP % 256;
    buffer[SPEEDUINO_MAP_WORD+1] = TEST_MAP / 256;
    buffer[SPEEDUINO_CURRENTVE_BYTE] = 155;
    buffer[SPEEDUINO_BARO_BYTE] = 100;
    buffer[SPEEDUINO_CLT_BYTE] = 120;
    buffer[SPEEDUINO_IAT_BYTE] = 65;
    //oilTemp = 100;
    buffer[SPEEDUINO_WUECORR_BYTE] = 100;
    buffer[SPEEDUINO_AFRTARGET_BYTE] = 145;
    buffer[SPEEDUINO_AFR_BYTE] = 147;
    buffer[SPEEDUINO_VOLTAGE_BYTE] = 120;
    buffer[SPEEDUINO_EGOCORR_BYTE] = 100;
    buffer[SPEEDUINO_TPSDOT_BYTE] = 0;
    buffer[SPEEDUINO_AECORR_BYTE] = 100;
    buffer[SPEEDUINO_GAMMAE_BYTE] = 100;
    buffer[SPEEDUINO_ASECORR_BYTE] = 105;
  #endif
}

void recordGraph(float Var, int Num, int speedGraph)
{
  static int speedGraphArray[4][10]; // first [4] limits the number of graphs
  static int speedGraphCounter[4]; // first [4] limits the numner of graphs
  int speedGraphAverage = 0;

  speedGraphArray[Num][speedGraphCounter[Num]] = Var*100;
  speedGraphCounter[Num]++;

  if (speedGraphCounter[Num] == speedGraph) {
      for (int i = 0; i < speedGraph; i++){
        speedGraphAverage = speedGraphAverage + speedGraphArray[Num][i];
      }
      speedGraphAverage = speedGraphAverage / (speedGraph);
      graphData[Num][graphIndex[Num]] = speedGraphAverage;
      graphIndex[Num]++; // Next time we will write to the next data entry
      if(graphIndex[Num] > 107 || graphIndex[Num] < 0) graphIndex[Num] = 0;
      speedGraphCounter[Num] = 0;
  }
}

void displayGraph( float row, int Num, float minX, float maxX, String nameGraph1, float variableGraph1, int decimal1, int min1, int max1, String nameGraph2, float variableGraph2, int decimal2, int min2, int max2 )
{
  uint8_t i, x;
  float y, y2;
  float midX = (minX+maxX)/2; //figure out middle axis value
  int miX = minX; // axis label for min graph value 
  int maX = maxX; // axis label for max graph value
  int mdX = midX; // axis label for middle graph value
  float convertX = 50/(maxX-minX); // multiplier to scale graph to fit the full range (50 pixels)
  row = row * 64.0; // for vertical orientation, top or bottom of screen

  char valString[8];
  int16_t x1, y1;
  uint16_t w, h;
  int16_t xA, yA, xB, yB;

  int decimalMod = 1;

  // Print Variable 1 Upper Left
  if (nameGraph1 != "0"){
  OLED.setTextSize(0);  
  OLED.setCursor( 1, 2 + row );
  OLED.print(nameGraph1);
  formatValue(valString, variableGraph1, decimal1);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  if (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2)   OLED.setCursor((SCREEN_WIDTH/2)-w-6, 2 + row );
    else OLED.setCursor((SCREEN_WIDTH)-w, 2 + row );
  OLED.print(valString);
  OLED.setFont();
  if (decimal1 == 0) decimalMod = 1;
    else decimalMod = 10;
  if ((SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2) && (variableGraph1 < min1*decimalMod || variableGraph1 > max1*decimalMod )) OLED.fillRect( 0 , 0 + row , SCREEN_WIDTH/2, 12 , INVERSE);
  if ((SCREEN_ROTATION == 1 || SCREEN_ROTATION == 3) && (variableGraph1 < min1*decimalMod || variableGraph1 >= max1*decimalMod )) OLED.fillRect( 0 , 0 + row , SCREEN_WIDTH, 12 , INVERSE);
  }

  // Print Variable 2 Upper Right
  if (nameGraph2 != "0"){
  OLED.setTextSize(0);  
  OLED.setCursor( SCREEN_WIDTH/2, 2 + row );
  OLED.print(nameGraph2);
  formatValue(valString, variableGraph2, decimal2);
  OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
  OLED.setCursor(SCREEN_WIDTH-w, 2 + row );
  OLED.print(valString);
  OLED.setFont();
  if (decimal2 == 0) decimalMod = 1;
    else decimalMod = 10;
  if(variableGraph2 < min2*decimalMod) OLED.fillRect( SCREEN_WIDTH/2 , 0, SCREEN_WIDTH/2, 12, INVERSE);
  if(variableGraph2 > max2*decimalMod) OLED.fillRect( SCREEN_WIDTH/2 , 0, SCREEN_WIDTH/2, 12, INVERSE);
  }

  // Print Graph Axis Scale
  OLED.drawLine(14, 13 + row , 17, 13 + row , WHITE); // maxX
  OLED.setCursor(0, 13 + row );
  OLED.print(maX);
  OLED.drawLine(14, 38 + row , 17, 38 + row , WHITE); // midpoint
  OLED.setCursor(0, 35 + row );
  OLED.print(mdX);
  OLED.drawLine(14, 63 + row , 17, 63 + row , WHITE); // minX
  OLED.setCursor(0, 56 + row );
  OLED.print(miX);
  
  OLED.setTextColor(INVERSE); // Reset to regular text color

  i = ( graphIndex[Num] + (128 - SCREEN_WIDTH) ) % 108;         // Start from the oldest data
  for(x = 0; x < (SCREEN_WIDTH-20); x++)
  {
    if(i > 107) i = 0;                // Overflow back to zero

    y2 = y;                // y1 equals the previous pixel

    y = (((graphData[Num][i]/100.0)-minX) * convertX );   // convert int back to decimal value

    xA = x+20;
    yA = (63-y) + row;
    xB = x+19;
    yB = (63-y2) + row;

    if(x == 0){ //if this is the first point
      OLED.drawPixel(xA, yA , WHITE);
    }
    if(x > 0 && x < (SCREEN_WIDTH-20)) {
      cropLineToRange(xA, yA, xB, yB, 12+row, 63+row);
    }
    i++;

  }

}

void displayGraph2( float row, int Num, float minX, float maxX)
{
  uint8_t i, x;
  float y, y2;
  float convertX = 50/(maxX-minX); // multiplier to scale graph to fit the full range (50 pixels)
  row = row * 64.0; // for vertical orientation, top or bottom of screen
  int16_t xA, yA, xB, yB;
  
  OLED.setTextColor(INVERSE); // Reset to regular text color

  i = ( graphIndex[Num] + (128 - SCREEN_WIDTH) ) % 108;         // Start from the oldest data
  for(x = 0; x < (SCREEN_WIDTH-20); x++)
  {
    if(i > 107) i = 0;                // Overflow back to zero
    y2 = y;                // y1 equals the previous pixel
    y = (((graphData[Num][i]/100.0)-minX) * convertX );   // convert int back to decimal value

    xA = x+20;
    yA = (63-y) + row;
    xB = x+19;
    yB = (63-y2) + row;

      if ( ((i / 2)  % 2 == 0) && (x > 0 && x < (SCREEN_WIDTH-20)) ) { // draw a line two cycles on, two cycles off
          cropLineToRange(xA, yA, xB, yB, 12+row, 63+row);
      }
    i++;
  }
}

void drawCircleGauge(int GAUGE_CENTER_X, int GAUGE_CENTER_Y, int GAUGE_RADIUS, int GAUGE_START_ANGLE, int GAUGE_END_ANGLE,int NUM_GRADUATIONS, int sensorValue, int rangeMin, int rangeMax, int redlineValue, bool numbers) {

  // Calculate the angle range for the active area of the gauge
  float gaugeRange = GAUGE_END_ANGLE - GAUGE_START_ANGLE;
  
  // Determine the start and end angles based on clockwise or counterclockwise
  float startAngle, endAngle;
    startAngle = DEG_TO_RAD(GAUGE_START_ANGLE + gaugeRange);
    endAngle = DEG_TO_RAD(GAUGE_START_ANGLE);

  // Draw the active area of the gauge
  drawArc(GAUGE_CENTER_X, GAUGE_CENTER_Y, GAUGE_RADIUS, startAngle, endAngle, WHITE, 16);

  // Calculate the angle between each graduation
  float anglePerGraduation = gaugeRange / (float)(NUM_GRADUATIONS - 1);

  // Draw graduations and labels
  for (int i = 0; i < NUM_GRADUATIONS; i++) {
    float angle = DEG_TO_RAD(GAUGE_START_ANGLE + i * anglePerGraduation);
    
    // Determine the coordinates
    int x1, y1, x2, y2;
      x1 = GAUGE_CENTER_X + cos(angle) * GAUGE_RADIUS;
      y1 = GAUGE_CENTER_Y - sin(angle) * GAUGE_RADIUS;
      x2 = GAUGE_CENTER_X + cos(angle) * (GAUGE_RADIUS - 4);
      y2 = GAUGE_CENTER_Y - sin(angle) * (GAUGE_RADIUS - 4);

    OLED.drawLine(x1, y1, x2, y2, WHITE);

    if (numbers){
    // Calculate the position for the label
    int xOffset = -2;
    int yOffset = -2;
    float labelRadius = GAUGE_RADIUS - 8;  // Adjust the radius as needed
    int labelX = GAUGE_CENTER_X + cos(angle) * labelRadius + xOffset;
    int labelY = GAUGE_CENTER_Y - sin(angle) * labelRadius + yOffset;
    // Display the first digit of the degree label
    OLED.setCursor(labelX, labelY);
    int range = (rangeMax- rangeMin);
    int firstDigit = getFirstDigit(rangeMin + i * (range / (NUM_GRADUATIONS - 1)));
    OLED.print(firstDigit);
    }
  }

  // Calculate the angle range for the redline
  float redlineAngleStart = DEG_TO_RAD(GAUGE_START_ANGLE + (redlineValue - rangeMin) * gaugeRange / (rangeMax - rangeMin));
  float redlineAngleEnd = DEG_TO_RAD(GAUGE_START_ANGLE + gaugeRange);
  
  // Draw the redline indicator arc
  drawArc(GAUGE_CENTER_X, GAUGE_CENTER_Y, GAUGE_RADIUS - 4, redlineAngleStart, redlineAngleEnd, WHITE, 4); 

  // Calculate the position of the needle tip based on the sensor value
  sensorValue = constrain(sensorValue, rangeMin, rangeMax);
  float needleAngle = DEG_TO_RAD(GAUGE_START_ANGLE + (sensorValue - rangeMin) * gaugeRange / (rangeMax - rangeMin));

  // Determine the coordinates based on clockwise or counterclockwise
  int needleX, needleY, needleX2, needleY2;
    needleX = GAUGE_CENTER_X + cos(needleAngle) * GAUGE_RADIUS;
    needleY = GAUGE_CENTER_Y - sin(needleAngle) * GAUGE_RADIUS;
    needleX2 = GAUGE_CENTER_X + cos(needleAngle) * (GAUGE_RADIUS - 20);
    needleY2 = GAUGE_CENTER_Y - sin(needleAngle) * (GAUGE_RADIUS - 20);

  // Draw the needle
  OLED.drawLine(needleX, needleY, needleX2, needleY2, WHITE);
}

void customGauge(int customScreenSelect)
{
  int16_t x1, y1;
  uint16_t w, h;
  char valString[8];
  int currentStat;
  uint8_t statNum[] = { 0, 1, 9, 17, 18, 19, 20, 21, 22, 23, 24, 25, 
            26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 
            38, 39, 47, 48, 49, 50, 51, 52, 54, 55, 72, 73,
            74, 75, 76, 77, 78, 85, 89, 90, 91, 92, 93, 94,
            95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105,
            106, 110, 111, 112, 113, 114, 115, 116, 117, 118,
            119, 120, 124, 132, 133, 134 }; // 75 stats to cycle through on the custom screen
    currentStat = statNum[customScreenSelect];
    String name = speeduino[currentStat].name;
    int number = speeduino[currentStat].number;
    int subNumber = speeduino[currentStat].subNumber;
    int type = speeduino[currentStat].type;
    float multiplier = (speeduino[currentStat].multiplier)/10.00;
    int decimal = speeduino[currentStat].decimal;
    OLED.setTextSize(0);
    OLED.setCursor(0, 0);

    if (type != 0) {
        if (multiplier != 0) {
            if (type == 1) formatValue(valString, (getByte(number))*multiplier, decimal);
            if (type == 2) formatValue(valString, (getWord(number))*multiplier, decimal);
        }
        if (multiplier == 0) {
            if (type == 1) formatValue(valString, ( (getByte(number) + SPEEDUINO_TEMPERATURE_OFFSET ) * SPEEDUINO_TEMPERATURE_MULTIPLIER ) + C_TO_F, decimal);
            if (type == 2) formatValue(valString, ( (getWord(number) + SPEEDUINO_TEMPERATURE_OFFSET ) * SPEEDUINO_TEMPERATURE_MULTIPLIER ) + C_TO_F, decimal);
        }
      if (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2) {
        OLED.println("CUSTOM GAUGE: ");
        OLED.print(name);
        OLED.setFont(&Numbers24pt7b);
        OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
        OLED.setCursor(SCREEN_WIDTH-w-4, SCREEN_HEIGHT-10);
      }
      if (SCREEN_ROTATION == 1 || SCREEN_ROTATION == 3) {
        OLED.println("CUSTOM");
        OLED.println("GAUGE");
        OLED.println(" ");
        OLED.print(name);
        OLED.setFont(&FreeMono9pt7b);
        OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
        OLED.setCursor(SCREEN_WIDTH-w-4, SCREEN_HEIGHT-h);
      }
    OLED.print(valString);
    OLED.setFont();
    }
    else {
        for (int i = 0; i < 8; i++) {
          if (speeduino[currentStat].number == speeduino[currentStat + i].number) {
            uint8_t x, y;
            name = speeduino[currentStat + i].name;
            if ( (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2) && (i < 4) ) {
                y = i*(SCREEN_HEIGHT/5);
                x = 0;
                }
            if ( (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2) && (i > 3) ) {
                y = (i-4)*(SCREEN_HEIGHT/5);
                x = SCREEN_WIDTH/2;
                }
            if (SCREEN_ROTATION == 1 || SCREEN_ROTATION == 3) {
                y = i*(SCREEN_HEIGHT/9);
                x = 0;
                }
            OLED.setCursor(x, y); 
            OLED.print(name);
            OLED.print(" ");
            formatValue(valString, getBit(number, i), 0);
            OLED.print(valString);
            OLED.getTextBounds(valString,0,0,&x1, &y1, &w, &h);
            if (getBit(number, i) == 1) OLED.fillRect( x, y, 64, h, INVERSE);
            OLED.println(" ");
            }
        }
        OLED.setFont();
    }
    // draw navigation scroll at bottom of screen
    OLED.drawFastHLine(0, SCREEN_HEIGHT-1, SCREEN_WIDTH, INVERSE);
    OLED.drawFastVLine(0, SCREEN_HEIGHT-2, -5, INVERSE);
    OLED.drawFastVLine(SCREEN_WIDTH-1, SCREEN_HEIGHT-2, -5, INVERSE);
    OLED.drawFastVLine( ((customScreenSelect/75.0)*(SCREEN_WIDTH-2))+1, SCREEN_HEIGHT-2, -5, INVERSE);
}

void pageNoECU()
{
  int16_t x1, y1;
  uint16_t w, h;
  OLED.drawBitmap((SCREEN_WIDTH - 64) / 2, ((SCREEN_HEIGHT - 64) / 2), logoDesign64, 64, 64, 1);
  String noECU = "NO ECU";
  OLED.getTextBounds(noECU,0,0,&x1, &y1, &w, &h);
  if (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2) OLED.setCursor(SCREEN_WIDTH-w-4, SCREEN_HEIGHT-h-4);
  if (SCREEN_ROTATION == 1 || SCREEN_ROTATION == 3) OLED.setCursor((SCREEN_WIDTH-w)/2, SCREEN_HEIGHT-h-4);
  OLED.setTextSize(1);
  OLED.setTextColor(INVERSE);
  OLED.print("NO ECU");
}

int getFirstDigit(int number) {
  while (number >= 100) {
    number = number/10;
  }
  return number / 10;
}

void drawArc(int centerX, int centerY, int radius, float startAngle, float endAngle, uint16_t color, int numSegments) {

  // Calculate the angular distance between each segment
  float angleStep = (endAngle - startAngle) / numSegments;

  // Draw each segment of the arc
  for (int i = 0; i < numSegments; i++) {
    float angle = startAngle + i * angleStep;

    // Calculate the coordinates of the current segment
    int x1 = centerX + cos(angle) * radius;
    int y1 = centerY - sin(angle) * radius;
    int x2 = centerX + cos(angle + angleStep) * radius;
    int y2 = centerY - sin(angle + angleStep) * radius;

    // Draw a line segment between the coordinates
    OLED.drawLine(x1, y1, x2, y2, color);
  }
}

uint8_t formatValue(char *buf, int32_t value, uint8_t decimal)
{
  clearBuffer(buf);
  snprintf(buf, 22, "%d", value);
  uint8_t len = strlen(buf);

  if (decimal != 0)
  {
    uint8_t target = decimal + 1;
    uint8_t numLen = len - ((value < 0) ? 1 : 0);
    while (numLen < target)
    {
      for (uint8_t i = 0; i < numLen + 1; i++)
      // if negative, skip negative sign
      {
        buf[len - i + 1] = buf[len - i];
        buf[len - i] = '0';
      }
      buf[len + 1] = '\0';
      numLen++;
      len++;
    }
    // insert
    for (uint8_t i = 0; i < decimal + 1; i++)
    {
      buf[len - i + 1] = buf[len - i];
      buf[len - i] = '.';
    }
  }
  return strlen(buf);
}

void clearBuffer(char *buf, uint8_t bufLen)
{
  for (uint8_t i = 0; i < bufLen; i++)
  {
    buf[i] = '\0';
  }
}

void checkEngineStatus()
{
  // Check/update engine status
  if(getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_RUNNING_BIT))
  {
	  // Running
	  if(engineStatus == ENGINE_OFF)
	  {
		  if(getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_WARMUP_BIT))
			  engineStatus = ENGINE_WARMUP; // Off -> Warmup
		  else
			  engineStatus = ENGINE_RUN; // Off -> Run
	  }
	  else if(engineStatus == ENGINE_WARMUP)
	  {
		  if(!getBit(SPEEDUINO_ENGINE_BITFIELD, SPEEDUINO_ENGINE_WARMUP_BIT))
			  engineStatus = ENGINE_RUN; // Warmup -> Run
	  }
	  
	  // Check idle condition
	  if(engineIdling == 0 && getWord(SPEEDUINO_RPM_WORD) < IDLE_RPM_ENTER)
		  engineIdling = millis();
	  else if(getWord(SPEEDUINO_RPM_WORD) > IDLE_RPM_EXIT)
		  engineIdling = 0; 

    #ifndef TEST
      // Check if shift light must be lit
      if(!shiftLight && (getBit(SPEEDUINO_SPARK_BITFIELD, SPEEDUINO_SPARK_SOFTLIMIT_BIT) || getBit(SPEEDUINO_SPARK_BITFIELD, SPEEDUINO_SPARK_HARDLIMIT_BIT) || getWord(SPEEDUINO_RPM_WORD) > SHIFTLIGHT_ON_RPM))
        shiftLight = true;
      else if(shiftLight && (!getBit(SPEEDUINO_SPARK_BITFIELD, SPEEDUINO_SPARK_SOFTLIMIT_BIT) && !getBit(SPEEDUINO_SPARK_BITFIELD, SPEEDUINO_SPARK_HARDLIMIT_BIT) && getWord(SPEEDUINO_RPM_WORD) < SHIFTLIGHT_OFF_RPM))
        shiftLight = false;
    #endif
  }
  else
  {
	  // Off
	  engineStatus = ENGINE_OFF;
  }
}  

float easing(float currentValue, float targetValue, float easingFactor, int totalSteps, int currentStep) {
  if (currentStep <= 0) {
    return currentValue; // No change if currentStep is 0 or negative
  } else if (currentStep >= totalSteps) {
    return targetValue; // Reached target value if currentStep is greater than or equal to totalSteps
  } else {
    float t = (float)currentStep / totalSteps; // Normalized time (0.0 to 1.0)
    float tSquared = t * t;
    float tCubed = tSquared * t;
    float easeIn = 2 * tCubed - 3 * tSquared + 1; // Ease-in formula
    float easeOut = 1 - easeIn; // Ease-out formula

    // Calculate the eased value between currentValue and targetValue
    float easedValue = currentValue + (targetValue - currentValue) * (easeIn * (1 - easingFactor) + easeOut * easingFactor);

    return easedValue;
  }
}

void cropLineToRange(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t minY, int16_t maxY) {
  // Check if the line is completely outside the range
  if ((y1 <= minY && y2 <= minY) || (y1 >= maxY && y2 >= maxY)) {
    // Line is completely outside the range, no need to draw anything
    return;
  }

  // Clip the line to the specified range
  if (y1 < minY) {
    float slope = static_cast<float>(y2 - y1) / (x2 - x1);
    x1 += static_cast<int16_t>((minY - y1) / slope);
    y1 = minY;
  }
  if (y2 < minY) {
    float slope = static_cast<float>(y2 - y1) / (x2 - x1);
    x2 += static_cast<int16_t>((minY - y2) / slope);
    y2 = minY;
  }
  if (y1 > maxY) {
    float slope = static_cast<float>(y2 - y1) / (x2 - x1);
    x1 += static_cast<int16_t>((maxY - y1) / slope);
    y1 = maxY;
  }
  if (y2 > maxY) {
    float slope = static_cast<float>(y2 - y1) / (x2 - x1);
    x2 += static_cast<int16_t>((maxY - y2) / slope);
    y2 = maxY;
  }
  OLED.drawLine(x1, y1  , x2, y2 , WHITE); 
}

void drawPartialBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t width, int16_t height, uint8_t numColumns) {
  int16_t byteWidth = (width + 7) / 8;

  for (int16_t j = 0; j < height; j++) {
    for (int16_t i = 0; i < numColumns; i++) {
      if (x + i >= 0 && x + i < SCREEN_WIDTH && y + j >= 0 && y + j < SCREEN_HEIGHT) {
        uint8_t byte = pgm_read_byte(bitmap + j * byteWidth + i / 8);
        uint8_t bit = byte & (128 >> (i % 8));
        if (bit != 0) {
          OLED.drawPixel(x + i, y + j, WHITE);
        }
      }
    }
  }
}