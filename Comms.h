#ifndef COMMS_H
#define COMMS_H

#include "Arduino.h"

static uint8_t buffer[SPEEDUINO_DATA_LEN];
bool requestData(uint16_t timeout = 20);

bool getBit(uint16_t address, uint8_t bit);
uint8_t getByte(uint16_t address);
uint16_t getWord(uint16_t address);

#endif //COMMS_H