#ifndef USERS_H
#define USERS_H

#include <string.h>

#include "MKL05Z4.h"
#include "rooms.h"

#define MAX_USERS_PER_ROOM 10
#define UID_LENGTH 16

extern char assignedUsers[MAX_ROOMS][MAX_USERS_PER_ROOM][UID_LENGTH];

int8_t isUserInRoom(char *room, char *uid);
int8_t addUserToRoom(char *room, char *uid);
int8_t removeUserFromRoom(char *room, char *uid);
int8_t toggleUserInRoom(char *room, char *uid);

#endif // USERS_H
