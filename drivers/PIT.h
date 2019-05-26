#ifndef PIT_H
#define PIT_H

#include "../kernel/isr.h"
#include "screen.h"
#include "../kernel/common.h"

void init_PIT(u32 frequency);

#endif