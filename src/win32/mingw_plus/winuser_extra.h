/* 
 * winuser_extra.h
 *
 * Description: The following is missing from mingw 1.1
 */

#ifndef _WINUSER_EXTRA_H_
#define _WINUSER_EXTRA_H_

/* From MSDN Lib - Mingw doesn't have */
#define FLASHW_STOP         0
#define FLASHW_CAPTION      0x00000001
#define FLASHW_TRAY         0x00000002
#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
#define FLASHW_TIMER        0x00000004
#define FLASHW_TIMERNOFG    0x0000000C


/* From MSDN Lib - Mingw doesn't have */
typedef struct {
    UINT  cbSize;
    HWND  hwnd;
    DWORD dwFlags;
    UINT  uCount;
    DWORD dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

#endif /* _WINUSER_EXTRA_H_ */
