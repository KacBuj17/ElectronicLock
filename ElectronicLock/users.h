#ifndef USERS_H
#define USERS_H

#include <string.h>

#include "MKL05Z4.h"
#include "rooms.h"

#define MAX_USERS_PER_ROOM 10 ///< Maximum number of users per room.
#define UID_LENGTH 16         ///< Length of the RFID UID string.

/**
 * @brief Checks if a user with the specified UID is in the database for specific room.
 * @param room Name of the room.
 * @param uid UID of the user.
 * @return 1 if the user is in the room, 0 otherwise, -1 if an error occurs.
 */
int8_t isUserInRoom(char *room, char *uid);

/**
 * @brief Adds a user to the room if there is space available.
 * @param room Name of the room.
 * @param uid UID of the user.
 * @return 1 if the user was added, 0 if the room is full, -1 if an error occurs.
 */
int8_t addUserToRoom(char *room, char *uid);

/**
 * @brief Removes a user with the specified UID from the room.
 * @param room Name of the room.
 * @param uid UID of the user.
 * @return 1 if the user was removed, 0 if the user was not found, -1 if an error occurs.
 */
int8_t removeUserFromRoom(char *room, char *uid);
#endif // USERS_H
