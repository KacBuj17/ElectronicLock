#ifndef USERS_H
#define USERS_H

#include <string.h>

#include "MKL05Z4.h"
#include "rooms.h"

#define MAX_USERS_PER_ROOM 10
#define UID_LENGTH 16

/**
 * @brief Sprawdza, czy u�ytkownik o podanym UID znajduje si� w bazie danych dla konkretnego pokoju.
 * @param room Nazwa pokoju.
 * @param uid UID u�ytkownika.
 * @return 1 je�li u�ytkownik jest w bazie danych, 0 je�li nie, -1 w przypadku b��du.
 */
int8_t isUserInRoom(char *room, char *uid);

/**
 * @brief Dodaje u�ytkownika do pokoju, je�li jest wolne miejsce.
 * @param room Nazwa pokoju.
 * @param uid UID u�ytkownika.
 * @return 1 je�li u�ytkownik zosta� dodany, 0 je�li brak miejsca, -1 w przypadku b��du.
 */
int8_t addUserToRoom(char *room, char *uid);

/**
 * @brief Usuwa u�ytkownika o podanym UID z pokoju.
 * @param room Nazwa pokoju.
 * @param uid UID u�ytkownika.
 * @return 1 je�li u�ytkownik zosta� usuni�ty, 0 je�li nie znaleziono u�ytkownika, -1 w przypadku b��du.
 */
int8_t removeUserFromRoom(char *room, char *uid);

#endif // USERS_H
