#ifndef ROOMS_H
#define ROOMS_H

#include <stdlib.h>
#include <stdio.h>
#include "MKL05Z4.h"

#define MAX_ROOMS 10 ///< Maximum number of rooms.

extern char default_room[4]; ///< Default room number.
extern char current_room[4]; ///< Current room number being operated on.

/**
 * @brief Switches to the previous room if possible.
 */
void switchRoomDown(void);

/**
 * @brief Switches to the next room if possible.
 */
void switchRoomUp(void);

#endif // ROOMS_H
