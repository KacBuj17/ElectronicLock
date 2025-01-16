#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MKL05Z4.h"
#include "lcd1602.h"
#include "users.h"
#include "rooms.h"
#include "rfid.h"
#include "klaw.h"
#include "lock.h"

#define DEBOUNCE_DELAY_MS 100
#define DOOR_OPEN_S 2
#define DETECTION_TIMEOUT 5

#define ACCESS_GRANTED 0
#define ACCESS_DENIED 1
#define RFID_ERROR 2
#define CARD_NOT_DETECTED 3

void SysTick_Handler(void);
void displayMessageWithTimeout(char* line1, char* line2, uint8_t timeout_seconds);
void displayMessage(char* line1, char* line2);
void PORTA_IRQHandler(void);
void handleUsersAdministrate(void);
uint8_t handleReadRfidUID(void);
void handleRfidAccess(void);
void handleButtonsPressed(void);

#endif /* MAIN_H */
