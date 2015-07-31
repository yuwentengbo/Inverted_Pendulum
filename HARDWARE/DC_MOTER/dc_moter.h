#ifndef __DC_MOTER_H
#define __DC_MOTER_H
#include "sys.h"

#define MA PAout(1)
#define MB PAout(2)

void dc_moter_init();
void dc_moter_setspeed(int speed);

#endif