#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <stdlib.h>
#include <lib/assert.h>
#include <ili9341.h>

#include "frame_type.h"
#include "frame_rect.h"

#define ASSERT(x)   assert_(x)

typedef struct frame frame;
struct frame
{
    frame_type mFrameType;      // Type of the frame.

    const char *mpTitle;        // Title.

    uint8_t mOrder;             // Order defines drawing seq & event reaction.

    frame_rect mRegion;         // Region of the whole frame, in blocks of 8x8.
    
    color_t mPaper, mInk;       // Color scheme of this frame.

    int (*mpfEventProc)         // Ptr to touch processing
    (frame *pF, int x, int y);  // function.
};



#endif
