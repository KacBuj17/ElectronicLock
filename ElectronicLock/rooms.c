#include "rooms.h"

char default_room[4] = "300";
char current_room[4] = "300";

void switchRoomDown()
{
    int roomIndex = atoi(current_room) - atoi(default_room);
    if (roomIndex > 0)
    {
        roomIndex--;
        sprintf(current_room, "%d", roomIndex + atoi(default_room));
    }
}

void switchRoomUp()
{
    int roomIndex = atoi(current_room) - atoi(default_room);
    if (roomIndex < MAX_ROOMS - 1)
    {
        roomIndex++;
        sprintf(current_room, "%d", roomIndex + atoi(default_room));
    }
}
