/* Quick and dirty WoW Memory Reader */
/* Written by Rijn */

/* Do what you will with it */

#include "wowread.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name and window title into a global variables  */
char szClassName[] = "WindowsApp";
char szWindowTitle[] = "WoWRead v0.13";

int offset[] = {0x11E8, /* Current HP */
                0x1200, /* Max HP */
                0x11EC, /* Current MP */
                0x1204, /* Max MP */
                0x16C8, /* Current XP */
                0x1E64, /* Rest XP */
                0x16CC, /* Max XP */
                0x0C48}; /* ID of NPC/PC under mouse */

HWND hwndStats; /* Edit box where stats are written to */
WOW_WINDOW win; /* Contains our window information */
WOW_STATS player; /* Where we will put player data */

int readProcess = 0;

void OpenWoW()
{
    char buff[256];

	if((win.tid = GetWindowThreadProcessId(win.hwnd, &win.pid)) == NULL)
	{
		sprintf(buff, "Cannot get process!");
		SetWindowText(hwndStats, buff);
		return;
	}

	if((win.process = OpenProcess(PROCESS_ALL_ACCESS, false, win.pid)) == NULL)
	{
		sprintf(buff, "Cannot open process!");
		SetWindowText(hwndStats, buff);
		return;
	}

	sprintf(buff, "Waiting until in game...");
	SetWindowText(hwndStats, buff);
}

void CloseWoW()
{
    if(readProcess)
	{
		if((win.hwnd = FindWindow("GxWindowClassD3d", "World of Warcraft")) != NULL)
			CloseHandle(win.process);
	}
}

void UpdateStatBox()
{
	char buff[256];
	int baseTemp;

	ReadProcessMemory(win.process, (void*)STATIC_ADDRESS, (void*)&baseTemp, 4, NULL);
	
	/* Since the address we are reading from doesn't always contain the address
	   we want, lets filter out the bad ones */
	if(((baseTemp & 0x8FF8) == 0x8008) || ((baseTemp & 0x8FF8) == 0x8))
	{
        player.base = baseTemp;
    }
		
    if(player.base != 0)
    {
        for(int x = 0; x < (sizeof(offset) / sizeof(offset[0])); x++)
        {
            /* If we get some crazy values it means our address isn't good anymore. */
            if(!(ReadProcessMemory(win.process, (void*)(player.base + offset[x]), (void*)&player.stat[x], 4, NULL)))
            {
                player.base = 0;
                SetWindowText(hwndStats, "Waiting until in game...");
                return;
            }
        }
    
        /* Yea, I know... */
        sprintf(buff, "Base Address: 0x%X\r\n\r\nHP: %d / %d\r\nMP: %d / %d\r\nXP: %d / %d \(+%d rest\)\r\n\r\nMouse Over NPC/PC: 0x%X",
                player.base, player.stat[0], player.stat[1], player.stat[2],
                player.stat[3], player.stat[4], player.stat[5], player.stat[6], player.stat[7]);
        SetWindowText(hwndStats, buff);
    } else
    {
        SetWindowText(hwndStats, "Waiting until in game...");
    }
}

/* ************* */
/* UI code below */
/* ************* */

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           szWindowTitle,       /* Title Text */
           WS_CAPTION | WS_SYSMENU,
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           300,                 /* The programs width */
           250,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            hwndStats = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY,
			0, 0, 300, 250, hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
			player.base = 0;
			SetTimer(hwnd, UPDATE_TIMER, 500, (TIMERPROC)NULL);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, UPDATE_TIMER);
            CloseWoW();
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_TIMER:
            if((UINT)wParam == UPDATE_TIMER)
            {
                if((win.hwnd = FindWindow("GxWindowClassD3d", "World of Warcraft")) != NULL)
                {
                    if(!readProcess) OpenWoW();
                    readProcess = 1;
                    UpdateStatBox();
                } else
                {
                    readProcess = 0;
                    SetWindowText(hwndStats, "WoW is not running...");
                }
            }
		  break;
		break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
