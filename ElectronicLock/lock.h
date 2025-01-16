#ifndef LOCK_H
#define LOCK_H

#include "MKL05Z4.h"

#define lockPin 5

/**
 * @brief Inicjalizuje pin steruj�cy zamkiem jako wyj�cie.
 */
void LockInit(void);

/**
 * @brief Otwiera zamek, ustawiaj�c odpowiedni pin na stan wysoki.
 */
void doorOpen(void);

/**
 * @brief Zamyka zamek, ustawiaj�c odpowiedni pin na stan niski.
 */
void doorClose(void);

#endif // LOCK_H
