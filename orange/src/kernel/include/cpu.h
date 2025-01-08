#ifndef CPU
#define CPU
#include "../../../comm/type.h"

typedef struct __attribute__((packed)) _segment_desc {
    uint16_t limit15_0;
    uint16_t base15_0;
    uint8_t base23_16;
    uint16_t attr;
    uint8_t base31_24;
} segment_desc;

#endif