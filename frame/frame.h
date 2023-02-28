#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>
#include <stdlib.h>
#include <lib/assert.h>
#include <ili9341.h>

#include "frame_type.h"
#include "frame_rect.h"
#include "frame_event.h"

#define ASSERT(x)   assert_(x)

typedef struct frame frame;
struct frame
{
    frame_type mFrameType;      // Type of the frame.

    const char *mpTitle;        // Title.

    frame_rect mRegion;         // Region of the whole frame, in blocks of 8x8.
    
    color_t mPaper, mInk;       // Color scheme of this frame.

    int (*mpfEventProc)         // Ptr to event processing
    (frame *pF, frame_event fE, // function.
    int x, int y, void *pcntx);

    void *mpPayLoad;            // Custom data/func associated with the frame.
};

#endif
