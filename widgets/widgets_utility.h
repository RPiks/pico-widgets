#include <frame.h>
#include <ui_context.h>

#define _countof(array) (sizeof(array) / sizeof(array[0]))

int FitTitlesHoriz(const int *pstrlens, int strcount, int fit_pix);
void DrawTextBBox(screen_control_t *pscr, const frame_rect *prct, int8_t shft);
void SetRegionColor(screen_control_t *pscr, const frame_rect *prct,
                    color_t paper, color_t ink);
