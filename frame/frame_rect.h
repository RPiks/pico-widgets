#ifndef FRAME_RECT_H
#define FRAME_RECT_H

#include <stdint.h>

typedef struct
{
    uint16_t mTlx, mTly;
    uint16_t mWidth, mHeight;
} frame_rect;

#endif
