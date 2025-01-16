#include "users.h"

char assignedUsers[MAX_ROOMS][MAX_USERS_PER_ROOM][UID_LENGTH] = {{{0}}};

int8_t isUserInRoom(char *room, char *uid)
{
		int roomIndex = atoi(room) - atoi(default_room);
	
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

int8_t addUserToRoom(char *room, char *uid)
{
		int roomIndex = atoi(room) - atoi(default_room);
	
		if (roomIndex < 0 || roomIndex >= MAX_ROOMS)
		{
				return -1;
		}
		
		for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
		{
				if (assignedUsers[roomIndex][i][0] == '\0')
				{
						strcpy(assignedUsers[roomIndex][i], uid);
						return 1;
				}
		}
		return 0;
}

int8_t removeUserFromRoom(char *room, char *uid)
{
		int roomIndex = atoi(room) - atoi(default_room);
	
		if (roomIndex < 0 || roomIndex >= MAX_ROOMS)
		{
				return -1;
		}
			
		for (int i = 0; i < MAX_USERS_PER_ROOM; i++)
		{
				if (strcmp(assignedUsers[roomIndex][i], uid) == 0)
				{
						memset(assignedUsers[roomIndex][i], 0, UID_LENGTH);
						return 1;
				}
		}
		return 0;
}
