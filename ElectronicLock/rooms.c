#include "rooms.h"
#include <stdlib.h>
#include <stdio.h>

char current_room[4] = "300";

void switchRoomDown()
{
    int roomIndex = atoi(current_room) - 300;
    if (roomIndex > 0)
    {
        roomIndex--;
        sprintf(current_room, "%d", roomIndex + 300);
    }
}

void switchRoomUp()
{
    int roomIndex = atoi(current_room) - 300;
    if (roomIndex < MAX_ROOMS - 1)
    {
        roomIndex++;
        sprintf(current_room, "%d", roomIndex + 300);
    }
}
