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

#define DEBOUNCE_DELAY_MS 100 ///< Debounce delay for buttons in milliseconds.
#define DOOR_OPEN_S 2         ///< Time in seconds for the door to remain open.
#define DETECTION_TIMEOUT 5   ///< Timeout for card detection in seconds.

#define ACCESS_GRANTED 0      ///< Status indicating access is granted.
#define ACCESS_DENIED 1       ///< Status indicating access is denied.
#define RFID_ERROR 2          ///< Status indicating an RFID error occurred.
#define CARD_NOT_DETECTED 3   ///< Status indicating no card was detected.

/**
 * @brief SysTick interrupt handler for managing timers and state variables.
 */
void SysTick_Handler(void);

/**
 * @brief Displays a message on the LCD with a timeout.
 * @param line1 First line of the message.
 * @param line2 Second line of the message.
 * @param timeout_seconds Duration to display the message in seconds.
 */
void displayMessageWithTimeout(char* line1, char* line2, uint8_t timeout_seconds);

/**
 * @brief Displays a message on the LCD indefinitely.
 * @param line1 First line of the message.
 * @param line2 Second line of the message.
 */
void displayMessage(char* line1, char* line2);

/**
 * @brief Interrupt handler for buttons S2, S3, and S4 with debounce functionality.
 */
void PORTA_IRQHandler(void);

/**
 * @brief Handles the user administration process using RFID cards.
 */
void handleUsersAdministrate(void);

/**
 * @brief Reads the UID of an RFID card.
 * @return Status indicating the result of the operation (e.g., ACCESS_GRANTED, ACCESS_DENIED).
 */
uint8_t handleReadRfidUID(void);

/**
 * @brief Handles access control logic based on RFID card data.
 */
void handleRfidAccess(void);

/**
 * @brief Handles button press logic for S2, S3, and S4.
 */
void handleButtonsPressed(void);

#endif /* MAIN_H */
