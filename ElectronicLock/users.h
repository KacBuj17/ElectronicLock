#ifndef USERS_H
#define USERS_H

#include <string.h>

#include "MKL05Z4.h"
#include "rooms.h"

#define MAX_USERS_PER_ROOM 10
#define UID_LENGTH 16

/**
 * @brief Sprawdza, czy u¿ytkownik o podanym UID znajduje siê w bazie danych dla konkretnego pokoju.
 * @param room Nazwa pokoju.
 * @param uid UID u¿ytkownika.
 * @return 1 jeœli u¿ytkownik jest w bazie danych, 0 jeœli nie, -1 w przypadku b³êdu.
 */
int8_t isUserInRoom(char *room, char *uid);

/**
 * @brief Dodaje u¿ytkownika do pokoju, jeœli jest wolne miejsce.
 * @param room Nazwa pokoju.
 * @param uid UID u¿ytkownika.
 * @return 1 jeœli u¿ytkownik zosta³ dodany, 0 jeœli brak miejsca, -1 w przypadku b³êdu.
 */
int8_t addUserToRoom(char *room, char *uid);

/**
 * @brief Usuwa u¿ytkownika o podanym UID z pokoju.
 * @param room Nazwa pokoju.
 * @param uid UID u¿ytkownika.
 * @return 1 jeœli u¿ytkownik zosta³ usuniêty, 0 jeœli nie znaleziono u¿ytkownika, -1 w przypadku b³êdu.
 */
int8_t removeUserFromRoom(char *room, char *uid);

#endif // USERS_H
