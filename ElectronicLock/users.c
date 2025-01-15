#include "users.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char assignedUsers[MAX_ROOMS][MAX_USERS_PER_ROOM][UID_LENGTH] = {{{0}}}; // Baza danych RFID

// Funkcja sprawdzaj¹ca, czy u¿ytkownik znajduje siê w bazie dla danego pokoju
int isUserInRoom(char *room, char *uid)
{
    int roomIndex = atoi(room) - 300; // Zak³adaj¹c pokoje 300, 301, 302
    if (roomIndex < 0 || roomIndex >= MAX_ROOMS)
    {
        return -1; // B³êdny numer pokoju
    }

    // Przeszukaj bazê danych dla danego pokoju
    for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
    {
        if (strcmp(assignedUsers[roomIndex][i], uid) == 0) // Znaleziono u¿ytkownika
        {
            return 1; // U¿ytkownik ju¿ w pokoju
        }
    }

    return 0; // U¿ytkownik nie znajduje siê w pokoju
}

// Funkcja dodaj¹ca lub usuwaj¹ca u¿ytkownika z pokoju
void toggleUserInRoom(char *room, char *uid)
{
    int roomIndex = atoi(room) - 300; // Zak³adaj¹c pokoje 300, 301, 302
    if (roomIndex < 0 || roomIndex >= MAX_ROOMS)
    {
        LCD1602_SetCursor(0, 1);
        LCD1602_Print("Invalid room!");
        DELAY(1000)
        return;
    }

    // Sprawdzenie, czy u¿ytkownik ju¿ znajduje siê w pokoju
    if (isUserInRoom(room, uid) == 1)
    {
        // Usuñ u¿ytkownika z pokoju
        for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
        {
            if (strcmp(assignedUsers[roomIndex][i], uid) == 0)
            {
                memset(assignedUsers[roomIndex][i], 0, UID_LENGTH);
                LCD1602_SetCursor(0, 1);
                LCD1602_Print("User removed!");
                DELAY(1000)
                return;
            }
        }
    }
    else
    {
        // Jeœli u¿ytkownik nie istnieje, dodaj go do bazy
        for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
        {
            if (assignedUsers[roomIndex][i][0] == '\0') // Znaleziono wolne miejsce
            {
                strcpy(assignedUsers[roomIndex][i], uid);
                LCD1602_SetCursor(0, 1);
                LCD1602_Print("User added!");
                DELAY(1000)
                return;
            }
        }

        // Jeœli brak wolnego miejsca w bazie
        LCD1602_SetCursor(0, 1);
        LCD1602_Print("Room full!");
        DELAY(1000)
    }
}
