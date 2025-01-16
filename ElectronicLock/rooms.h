#ifndef ROOMS_H
#define ROOMS_H

#include <stdlib.h>
#include <stdio.h>
#include "MKL05Z4.h"

#define MAX_ROOMS 10

extern char default_room[4];
extern char current_room[4];

/**
 * @brief Prze³¹cza siê na poprzedni pokój, jeœli to mo¿liwe.
 */
void switchRoomDown(void);

/**
 * @brief Prze³¹cza siê na nastêpny pokój, jeœli to mo¿liwe.
 */
void switchRoomUp(void);

#endif // ROOMS_H
