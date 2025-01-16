#include "lock.h"

void LockInit()
{
		PORTA->PCR[lockPin] = PORT_PCR_MUX(1);
    PTA->PDDR |= (1 << lockPin);
    PTA->PCOR = (1 << lockPin);
}

void doorOpen()
{
		PTA->PSOR = (1 << lockPin);
}

void doorClose()
{
		PTA->PCOR = (1 << lockPin);
}
