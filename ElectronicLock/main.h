#ifndef MAIN_H
#define MAIN_H

#include "MKL05Z4.h"
#include "lcd1602.h"
#include "users.h"
#include "rooms.h"
#include "rfid.h"
#include "klaw.h"

#define UID_LENGTH 16

extern volatile uint8_t S2_press;
extern volatile uint8_t S3_press;
extern volatile uint8_t S4_press;

// Funkcje

void PORTA_IRQHandler(void);
void handleUsersAdministrate(void);
void handleRfidAccess(void);

#endif /* MAIN_H */
