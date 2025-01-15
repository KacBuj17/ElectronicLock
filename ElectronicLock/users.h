#ifndef USERS_H
#define USERS_H

#include <stdint.h>
#include "MKL05Z4.h"
#include "lcd1602.h"
#include "rooms.h"

#define MAX_ROOMS 10
#define MAX_USERS_PER_ROOM 10
#define UID_LENGTH 16

extern char assignedUsers[MAX_ROOMS][MAX_USERS_PER_ROOM][UID_LENGTH];

int isUserInRoom(char *room, char *uid);

void toggleUserInRoom(char *room, char *uid);

#endif // USERS_H
