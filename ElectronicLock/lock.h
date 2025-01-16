#ifndef LOCK_H
#define LOCK_H

#include "MKL05Z4.h"

#define lockPin 5

void lockInit(void);
void doorOpen(void);
void doorClose(void);

#endif // LOCK_H
