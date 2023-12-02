#pragma once

#include "_import.h"

// Each tick == 10ms
#define DT (1.0/100.0)

WASI_IMPORT(yield_tick, sched_yield, int32_t, void);
