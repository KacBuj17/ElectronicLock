#ifndef ROOMS_H
#define ROOMS_H

#include <stdint.h>
#include "MKL05Z4.h"

#define MAX_ROOMS 10

extern char current_room[4];

void switchRoomDown(void);

void switchRoomUp(void);

#endif // ROOMS_H
