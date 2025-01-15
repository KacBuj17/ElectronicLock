#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

volatile uint8_t S2_press = 0;
volatile uint8_t S3_press = 0;
volatile uint8_t S4_press = 0;

static uchar status;
static uchar str[MAX_LEN];
static uchar cardID[5];

void PORTA_IRQHandler(void)
{
    uint32_t buf;
    buf = PORTA->ISFR & (S2_MASK | S3_MASK | S4_MASK);

    switch (buf)
    {
    case S2_MASK:
        DELAY(100)
        if (!(PTA->PDIR & S2_MASK)) // Minimalizacja drga� zestyk�w
        {
            DELAY(100)
            if (!(PTA->PDIR & S2_MASK)) // Minimalizacja drga� zestyk�w (c.d.)
            {
                if (!S2_press)
                {
                    S2_press = 1;
                }
            }
        }
        break;
    case S3_MASK:
        DELAY(100)
        if (!(PTA->PDIR & S3_MASK)) // Minimalizacja drga� zestyk�w
        {
            DELAY(100)
            if (!(PTA->PDIR & S3_MASK)) // Minimalizacja drga� zestyk�w (c.d.)
            {
                if (!S3_press)
                {
                    S3_press = 1;
                }
            }
        }
        break;
    case S4_MASK:
        if (!S4_press)
        {
            S4_press = 1;
        }
        break;
    default:
        break;
    }
    PORTA->ISFR |= S2_MASK | S3_MASK | S4_MASK; // Kasowanie wszystkich bit�w ISF
    NVIC_ClearPendingIRQ(PORTA_IRQn);
}

void handleS4Press()
{
    char uid_str[UID_LENGTH];
    uint32_t timeout = 5000;
    uint32_t start = SysTick->VAL;

    LCD1602_ClearAll();
    LCD1602_SetCursor(0, 0);
    LCD1602_Print("Scan RFID for:");
    LCD1602_SetCursor(0, 1);
    LCD1602_Print(current_room);

    while ((SysTick->VAL - start) < timeout)
    {
        status = MFRC522_Request(PICC_REQIDL, str);
        if (status == MI_OK)
        {
            status = MFRC522_Anticoll(cardID);
            if (status == MI_OK)
            {
                sprintf(uid_str, "0x%x0x%x0x%x0x%x", cardID[0], cardID[1], cardID[2], cardID[3]);
                toggleUserInRoom(current_room, uid_str);
                break;
            }
        }
    }

    LCD1602_ClearAll();
    S4_press = 0;
}

int main(void)
{
    MFRC522_Init();
    Klaw_Init();
    Klaw_S2_4_Int();
    LCD1602_Init();
    LCD1602_Backlight(TRUE);
    LCD1602_ClearAll();

    LCD1602_SetCursor(0, 0);
    LCD1602_Print("Electronic Lock");

    LCD1602_SetCursor(0, 1);
    LCD1602_Print("Push S1 To Start");

    while (PTA->PDIR & S1_MASK);
    LCD1602_ClearAll();

    char uid_str[16];

    while (1)
    {
        LCD1602_ClearAll();
        LCD1602_SetCursor(0, 0);
        LCD1602_Print("Current Room: ");
        LCD1602_SetCursor(13, 0);
        LCD1602_Print(current_room);

        status = MFRC522_Request(PICC_REQIDL, str);
        if(status == MI_OK)
        {
            status = MFRC522_Anticoll(cardID);
            if(status == MI_OK)
            {
                sprintf(uid_str,"0x%x0x%x0x%x0x%x",cardID[0],cardID[1],cardID[2],cardID[3]);
                if(isUserInRoom(current_room, uid_str))
                {
                    LCD1602_SetCursor(0, 1);
                    LCD1602_Print("ACCESS GRANTED");
                }
                else
                {
                    LCD1602_SetCursor(0, 1);
                    LCD1602_Print("ACCESS DENIED");
                }
                DELAY(8000)
            }
        }

        if (S2_press)
				{
						switchRoomDown();
						S2_press = 0;
				}
				if (S3_press)
				{
						switchRoomUp();
						S3_press = 0;
				}
        if (S4_press)
        {
            handleS4Press();
        }
        DELAY(1000)
    }

    return 0;
}
