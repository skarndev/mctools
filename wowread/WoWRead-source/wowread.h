#ifndef __WOWREAD_H__
#define __WOWREAD_H__

#include <windows.h>
#include <stdio.h>
#include <string.h>

#define UPDATE_TIMER    100
#define STATIC_ADDRESS  0x12C3CC /* Well, it's not reallly static. But we can pretend. */

typedef struct wow_window
{
    HWND hwnd;
    HANDLE process;
    DWORD tid;
    DWORD pid;

} WOW_WINDOW;

/* Add more stuff later */
typedef struct wow_stats
{
    int base;
    int stat[10];
} WOW_STATS;

#endif