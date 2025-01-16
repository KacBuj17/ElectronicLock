#ifndef LOCK_H
#define LOCK_H

#include "MKL05Z4.h"

#define lockPin 5 ///< GPIO pin controlling the lock mechanism.

/**
 * @brief Initializes the lock control pin as an output.
 */
void LockInit(void);

/**
 * @brief Opens the door by setting the lock pin to high.
 */
void doorOpen(void);

/**
 * @brief Closes the door by setting the lock pin to low.
 */
void doorClose(void);

#endif // LOCK_H
