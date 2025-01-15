#include "users.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char assignedUsers[MAX_ROOMS][MAX_USERS_PER_ROOM][UID_LENGTH] = {{{0}}};

int isUserInRoom(char *room, char *uid)
{
    int roomIndex = atoi(room) - 300;
    if (roomIndex < 0 || roomIndex >= MAX_ROOMS)
    {
        return -1;
    }

    for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
    {
        if (strcmp(assignedUsers[roomIndex][i], uid) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void toggleUserInRoom(char *room, char *uid)
{
		LCD1602_ClearAll();
    int roomIndex = atoi(room) - 300;
    if (roomIndex < 0 || roomIndex >= MAX_ROOMS)
    {
        LCD1602_SetCursor(0, 0);
        LCD1602_Print("Invalid room!");
        DELAY(8000)
        return;
    }

    if (isUserInRoom(room, uid) == 1)
    {
        for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
        {
            if (strcmp(assignedUsers[roomIndex][i], uid) == 0)
            {
                memset(assignedUsers[roomIndex][i], 0, UID_LENGTH);
                LCD1602_SetCursor(0, 0);
                LCD1602_Print("User removed");
								LCD1602_SetCursor(0, 1);
                LCD1602_Print("succesfuly!");
								DELAY(8000)
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
        {
            if (assignedUsers[roomIndex][i][0] == '\0')
            {
                strcpy(assignedUsers[roomIndex][i], uid);
                LCD1602_SetCursor(0, 0);
                LCD1602_Print("User added");
								LCD1602_SetCursor(0, 1);
                LCD1602_Print("succesfuly!");
								DELAY(8000)
                return;
            }
        }

        LCD1602_SetCursor(0, 1);
        LCD1602_Print("Room full!");
				DELAY(8000)
    }
}
