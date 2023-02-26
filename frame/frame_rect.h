#ifndef FRAME_RECT_H
#define FRAME_RECT_H

#include <stdint.h>

/// @brief Struct which defines a rectangle in text mode.
typedef struct
{
    uint8_t mTlx, mTly;
    uint8_t mWidth, mHeight;
} frame_rect;

#endif