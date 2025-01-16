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

extern volatile uint8_t S2_press;
extern volatile uint8_t S3_press;
extern volatile uint8_t S4_press;

extern volatile uint8_t sekunda;
extern volatile uint8_t sekunda_OK;

extern volatile uint8_t display_message;
extern volatile uint32_t message_timer;

void SysTick_Handler(void);
void displayMessageWithTimeout(char* line1, char* line2, uint8_t timeout_seconds);
void PORTA_IRQHandler(void);
void handleUsersAdministrate(void);
void handleRfidAccess(void);

#endif /* MAIN_H */
