#ifndef MAIN_H
#define MAIN_H

#include "users.h"
#include "rooms.h"
#include "rfid.h"
#include "lcd1602.h"
#include "klaw.h"
#include "MKL05Z4.h"

#define UID_LENGTH 16
#define MAX_LEN 16

extern volatile uint8_t S2_press;
extern volatile uint8_t S3_press;
extern volatile uint8_t S4_press;

// Funkcje

void PORTA_IRQHandler(void);
void handleS4Press(void);
void handleRoomSwitch(void);

#endif /* MAIN_H */
