#include "widgets_utility.h"

/// @brief Draws a bounding box around a text rectangle.
/// @param pscr The control structure.
/// @param prct Text-mode rectangle in symbol coords.
/// @param shft A shift in pixels (<0 means inside one).
void DrawBBox(screen_control_t *pscr, const frame_rect *prct, int8_t shft)
{
    ASSERT(pscr);
    ASSERT(prct);
    ASSERT(prct->mTlx - shft >= 0);
    ASSERT(prct->mTly - shft >= 0);

    const int tlx = prct->mTlx - shft;
    const int tly = prct->mTly - shft;
    const int w = prct->mWidth + (shft<<1) - 1;
    const int h = prct->mHeight+ (shft<<1) - 1;

    TftPutLine(pscr, tlx, tly, tlx+w, tly);
    TftPutLine(pscr, tlx+w, tly, tlx+w, tly+h);
    TftPutLine(pscr, tlx, tly+h, tlx+w, tly+h);
    TftPutLine(pscr, tlx, tly, tlx, tly+h);
}

/// @brief Utility of fitting text titles horizontally.
/// @param pstrlens Ptr to array of strings width in pix.
/// @param strcount count of pstrlen's elements.
/// @param fit_pix Width of pixel line to fit in.
/// @return A gap calculated which should be added btw words.
int FitTitlesHoriz(const int *pstrlens, int strcount, int fit_pix)
{
    ASSERT(pstrlens);
    ASSERT(strcount);
    ASSERT(fit_pix > 8);

    int acc_w = 0;
    for(int i = 0; i < strcount; ++i)
    {
        acc_w += pstrlens[i];
    }

    return (fit_pix - acc_w) / strcount / 2;
}

/// @brief Sets color attrs of text mode rectangle.
/// @param pscr The screen control structure.
/// @param prct The text mode rectangle.
/// @param paper Paper(background) color.
/// @param ink Ink(foreground) color.
void SetRegionColor(screen_control_t *pscr, const frame_rect *prct,
                    color_t paper, color_t ink)
{
    for(int j = prct->mTly; j < prct->mTly + prct->mHeight; ++j)
        for(int i = prct->mTlx; i < prct->mTlx + prct->mWidth; ++i)
        {
            TftPutColorAttr(pscr, i, j, paper, ink);
        }
}

void AssignRect(frame_rect *pdst, uint16_t x, uint16_t y, 
                uint16_t w, uint16_t h)
{
    ASSERT(pdst);

    pdst->mTlx = x;
    pdst->mTly = y;
    pdst->mWidth = w;
    pdst->mHeight= h;
}

int SelectTargetRect(const frame_rect *prects, int count, int x, int y)
{
    for(int i = 0; i < count; ++i)
    {
        if(IsInsideRect(prects + i, x, y))
        {
            return i;
        }
    }
    return -1;
}

/// @brief Clear a widget using its bounding box.
/// @param pwidget A widget.
/// @param pscr The screen control structure.
void ClearWidgetBBox(frame *pwidget, screen_control_t *pscr)
{
    ASSERT(pwidget);

    const int xl = pwidget->mRegion.mTlx >> 3;
    const int xr = (pwidget->mRegion.mTlx + pwidget->mRegion.mWidth) >> 3;
    const int yl = pwidget->mRegion.mTly >> 3;
    const int yr = (pwidget->mRegion.mTly + pwidget->mRegion.mHeight) >> 3;
    for(int j = yl; j <= yr; ++j)
        for(int i = xl; i <= xr; ++i)
        {
            TftClearRect8(pscr, i, j);
            uint8_t *pbox = pscr->mpColorBuffer + i + TEXT_WIDTH * j;
            *pbox |= 1 << 6;        // Set for update.
        }
}
