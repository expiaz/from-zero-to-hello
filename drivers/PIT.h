#ifndef PIT_H
#define PIT_H

#include "../kernel/ISR.h"
#include "./screen.h"
#include "../kernel/common.h"

void PIT_init(u32 frequency);

#endif