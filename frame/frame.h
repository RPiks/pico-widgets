///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  frame.h - The `frame` definition header.
//
//
//  DESCRIPTION
//
//      Defines a basic atom of the pico-widgets UI.
//
//  PLATFORM
//      Hardware: Raspberry Pi Pico.
//      Software: https://github.com/RPiks/pico-touchscr-sdk
//
//  REVISION HISTORY
// 
//      Rev 0.5   25 Feb 2023
//  Initial release.
//
//  LICENCE
//      MIT License (http://www.opensource.org/licenses/mit-license.php)
//
//  Copyright (c) 2023 by Roman Piksaykin
//  
//  Permission is hereby granted, free of charge,to any person obtaining a copy
//  of this software and associated documentation files (the Software), to deal
//  in the Software without restriction,including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
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
