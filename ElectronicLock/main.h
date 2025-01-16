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

/**
 * @brief Obs³uguje przerwanie SysTick, zarz¹dza timerami i ich stanami.
 */
void SysTick_Handler(void);

/**
 * @brief Wyœwietla komunikat na LCD z czasem wyœwietlania.
 * @param line1 Pierwsza linia komunikatu.
 * @param line2 Druga linia komunikatu.
 * @param timeout_seconds Czas wyœwietlania w sekundach.
 */
void displayMessageWithTimeout(char* line1, char* line2, uint8_t timeout_seconds);

/**
 * @brief Wyœwietla komunikat na LCD bez limitu czasu.
 * @param line1 Pierwsza linia komunikatu.
 * @param line2 Druga linia komunikatu.
 */
void displayMessage(char* line1, char* line2);

/**
 * @brief Obs³uguje przerwanie dla przycisków S2, S3, S4, uwzglêdniaj¹c debouncing.
 */
void PORTA_IRQHandler(void);

/**
 * @brief Obs³uguje proces administracji u¿ytkownikami poprzez RFID.
 */
void handleUsersAdministrate(void);

/**
 * @brief Odczytuje UID karty RFID.
 * @return Status operacji (np. ACCESS_GRANTED, ACCESS_DENIED, RFID_ERROR, CARD_NOT_DETECTED).
 */
uint8_t handleReadRfidUID(void);

/**
 * @brief Obs³uguje dostêp u¿ytkownika na podstawie karty RFID.
 */
void handleRfidAccess(void);

/**
 * @brief Obs³uguje logikê dzia³ania po naciœniêciu przycisków S2, S3, S4.
 */
void handleButtonsPressed(void);

#endif /* MAIN_H */
