typedef enum 
{
    kEventUndef  =  -1,         // Perhaps shouldn't be used.

    kEventPing          =  0,   // Whether it is alive.
    kEventDraw          =  1,   // To draw.
    kEventClickInside   =  2,   // Click inside BBox.
    kEventClickOutside  =  3,   // ...outside.
    kEventTick          =  4,   // Time has passed.
    kEventClose         =  5,   // To close immediately.
    
} frame_event;
