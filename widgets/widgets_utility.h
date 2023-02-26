#include <frame.h>
#include <ui_context.h>
#include <ui_protos.h>

#define _countof(array) (sizeof(array) / sizeof(array[0]))

int FitTitlesHoriz(const int *pstrlens, int strcount, int fit_pix);
void DrawBBox(screen_control_t *pscr, const frame_rect *prct, int8_t shft);
void SetRegionColor(screen_control_t *pscr, const frame_rect *prct,
                    color_t paper, color_t ink);
void AssignRect(frame_rect *pdst, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
int SelectTargetRect(const frame_rect *prects, int count, int x, int y);
void ClearWidgetBBox(frame *pwidget, screen_control_t *pscr);
