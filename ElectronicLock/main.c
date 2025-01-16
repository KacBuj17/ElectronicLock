#include "main.h"

#define DEBOUNCE_DELAY_MS 100
#define DOOR_OPEN_S 2

volatile uint8_t S2_press = 0;
volatile uint8_t S3_press = 0;
volatile uint8_t S4_press = 0;

static uchar status;
static uchar str[UID_LENGTH];
static uchar cardID[4];
static char uid_str[UID_LENGTH];

volatile uint8_t sekunda = 0;
volatile uint8_t sekunda_OK = 0;

volatile uint8_t display_message = 0;
volatile uint32_t message_timer = 0;

volatile uint8_t door_opened = 0;
volatile uint32_t door_timer = 0;

void SysTick_Handler(void)
{ 
    sekunda++;
    if (sekunda == 10)
    {
        sekunda = 0;
        sekunda_OK = 1;

        if (display_message && message_timer > 0)
        {
            message_timer--;
            if (message_timer == 0)
            {
                display_message = 0;
            }
        }
				
				if (door_opened && door_timer > 0)
        {
            door_timer--;
            if (door_timer == 0)
            {
                door_opened = 0;
							  doorClose();
            }
        }
    }
}

void displayMessageWithTimeout(char* line1, char* line2, uint8_t timeout_seconds)
{
		if (line1)
    {
				LCD1602_SetCursor(0, 0);
				LCD1602_Print(line1);
    }
    if (line2)
    {
        LCD1602_SetCursor(0, 1);
        LCD1602_Print(line2);
    }

    display_message = 1;
    message_timer = timeout_seconds;
}

void PORTA_IRQHandler(void)
{
    uint32_t buf;
    buf = PORTA->ISFR & (S2_MASK | S3_MASK | S4_MASK);

    switch (buf)
    {
    case S2_MASK:
        DELAY(DEBOUNCE_DELAY_MS)
        if (!(PTA->PDIR & S2_MASK))
        {
            DELAY(DEBOUNCE_DELAY_MS)
            if (!(PTA->PDIR & S2_MASK))
            {
                if (!S2_press)
                {
                    S2_press = 1;
                }
            }
        }
        break;
    case S3_MASK:
        DELAY(DEBOUNCE_DELAY_MS)
        if (!(PTA->PDIR & S3_MASK))
        {
            DELAY(DEBOUNCE_DELAY_MS)
            if (!(PTA->PDIR & S3_MASK))
            {
                if (!S3_press)
                {
                    S3_press = 1;
                }
            }
        }
        break;
    case S4_MASK:
				DELAY(DEBOUNCE_DELAY_MS)
        if (!(PTA->PDIR & S4_MASK))
        {
            DELAY(DEBOUNCE_DELAY_MS)
            if (!(PTA->PDIR & S4_MASK))
            {
                if (!S4_press)
								{
										S4_press = 1;
								}
            }
        }
        break;
    default:
        break;
    }
    PORTA->ISFR |= S2_MASK | S3_MASK | S4_MASK;
    NVIC_ClearPendingIRQ(PORTA_IRQn);
}

void handleUsersAdministrate()
{
    LCD1602_ClearAll();
    LCD1602_SetCursor(0, 0);
    LCD1602_Print("Waiting for card");

    while (1)
    {
        status = MFRC522_Request(PICC_REQIDL, str);
        if (status == MI_OK)
        {
            status = MFRC522_Anticoll(cardID);
            if (status == MI_OK)
            {
								LCD1602_ClearAll();
                sprintf(uid_str, "0x%x0x%x0x%x0x%x", cardID[0], cardID[1], cardID[2], cardID[3]);
							
								if(isUserInRoom(current_room, uid_str))
								{
									removeUserFromRoom(current_room, uid_str);
									displayMessageWithTimeout("User Removed", "Succesfuly", 2);
								}
								else
								{
									addUserToRoom(current_room, uid_str);
									displayMessageWithTimeout("User Added", "Succesfuly", 2);
								}
                break;
            }
        }
    }
}

void handleRfidAccess()
{
		status = MFRC522_Request(PICC_REQIDL, str);
		if(status == MI_OK)
		{
				status = MFRC522_Anticoll(cardID);
				if(status == MI_OK)
				{
						sprintf(uid_str,"0x%x0x%x0x%x0x%x",cardID[0],cardID[1],cardID[2],cardID[3]);
						if(isUserInRoom(current_room, uid_str))
						{
								displayMessageWithTimeout("" , "ACCESS GRANTED", 2);
								doorOpen();
								door_opened = 1;
								door_timer = DOOR_OPEN_S;
						}
						else
						{
								displayMessageWithTimeout("" , "ACCESS DENIED ", 2);
						}
				}
				else
				{
						displayMessageWithTimeout("", "RFID ERROR   ", 2);
				}
		}
}

int main(void)
{
    MFRC522_Init();
    Klaw_Init();
    Klaw_S2_4_Int();
		lockInit();
    LCD1602_Init();
    LCD1602_Backlight(TRUE);
    LCD1602_ClearAll();

    LCD1602_SetCursor(0, 0);
    LCD1602_Print("Electronic Lock");
    LCD1602_SetCursor(0, 1);
    LCD1602_Print("Push S1 To Start");
	
		SysTick_Config(SystemCoreClock/10);

    while (PTA->PDIR & S1_MASK);
    LCD1602_ClearAll();
		char buffer[32];

    while (1)
    {
        if (!display_message)
        {
            LCD1602_ClearAll();
            sprintf(buffer, "Current Room:%s", current_room);
						LCD1602_SetCursor(0, 0);
						LCD1602_Print(buffer);
        }
				
				handleRfidAccess();
			
				if (S2_press) 
				{
						display_message = 0;
						switchRoomDown();
						S2_press = 0;
				}
				if (S3_press) 
				{
						display_message = 0;
						switchRoomUp();
						S3_press = 0;
				}
				if (S4_press) 
				{
						display_message = 0;
						handleUsersAdministrate();
						S4_press = 0;
				}
		}
}
