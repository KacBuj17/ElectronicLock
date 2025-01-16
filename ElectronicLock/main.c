#include "main.h"

static uint8_t S2_press = 0;
static uint8_t S3_press = 0;
static uint8_t S4_press = 0;

static uint8_t seccond = 0;

static uint8_t display_message = 0;
static uint32_t message_timer = 0;

static uint8_t card_detection = 0;
static uint32_t card_detection_timer = 0;

static uint8_t door_opened = 0;
static uint32_t door_timer = 0;

static uchar cardID[4];
static uchar str[UID_LENGTH];
static char uid_str[UID_LENGTH];

void SysTick_Handler(void)
{ 
    seccond++;
    if (seccond == 10)
    {
        seccond = 0;

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
				
				if (card_detection && card_detection_timer > 0)
        {
            card_detection_timer--;
            if (card_detection_timer == 0)
            {
                card_detection = 0;
            }
        }
    }
}

void displayMessageWithTimeout(char* line1, char* line2, uint8_t timeout_seconds)
{
		if(line1)
		{
				LCD1602_SetCursor(0, 0);
				LCD1602_Print(line1);
		}
		if(line2)
		{
				LCD1602_SetCursor(0, 1);
				LCD1602_Print(line2);
		}

    display_message = 1;
    message_timer = timeout_seconds;
}

void displayMessage(char* line1, char* line2)
{
		if(line1)
		{
				LCD1602_SetCursor(0, 0);
				LCD1602_Print(line1);
		}
		if(line2)
		{
				LCD1602_SetCursor(0, 1);
				LCD1602_Print(line2);
		}
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
		displayMessage("Waiting for card", "");
	
		card_detection = 1;
		card_detection_timer = DETECTION_TIMEOUT;
	
    while (card_detection)
    {
        uchar status = MFRC522_Request(PICC_REQIDL, str);
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

uchar handleReadRfidUID()
{
		uchar status = MFRC522_Request(PICC_REQIDL, str);
		if(status == MI_OK)
		{
				status = MFRC522_Anticoll(cardID);
				if(status == MI_OK)
				{
						sprintf(uid_str,"0x%x0x%x0x%x0x%x",cardID[0],cardID[1],cardID[2],cardID[3]);
						return isUserInRoom(current_room, uid_str) ? ACCESS_GRANTED : ACCESS_DENIED;
				}
				return RFID_ERROR;
		}
		return CARD_NOT_DETECTED;
}

void handleRfidAccess()
{
		uchar auth_status = handleReadRfidUID();
		if(auth_status == 0)
		{
				displayMessageWithTimeout("", "ACCESS GRANTED", DOOR_OPEN_S);
				doorOpen();
				door_opened = 1;
				door_timer = DOOR_OPEN_S;
		}
		else if(auth_status == 1)
		{
				displayMessageWithTimeout("", "ACCESS DENIED ", DOOR_OPEN_S);
		}
		else if(auth_status == 2)
		{
				displayMessageWithTimeout("", "RFID ERROR   ", DOOR_OPEN_S);
		}
}

void handleButtonsPressed()
{
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

int main(void)
{
    MFRC522_Init();
	
    Klaw_Init();
    Klaw_S2_4_Int();
	
		LockInit();
	
    LCD1602_Init();
    LCD1602_Backlight(TRUE);
		LCD1602_ClearAll();
	
		SysTick_Config(SystemCoreClock/10);
	
		char buffer[16];
	
		displayMessage("Electronic Lock", "Push S1 To Start");

    while (PTA->PDIR & S1_MASK);
	
    while (1)
    {
        if (!display_message)
        {
						LCD1602_ClearAll();
						sprintf(buffer, "Current Room:%s", current_room);
						displayMessage(buffer, "");
        }
				
				handleRfidAccess();
				handleButtonsPressed();
		}
}
