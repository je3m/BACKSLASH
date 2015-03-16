#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#pragma comment(lib, "winmm.lib")
//#define	DEBUG	0

#define CLASSNAME	"winkey"
#define WINDOWTITLE	"svchost"

char	windir[MAX_PATH + 1];
HHOOK	kbdhook;	/* Keyboard hook handle */
bool	running;	/* Used in main loop */

/**
* \brief Called by Windows automagically every time a key is pressed (regardless
* of who has focus)
*/
__declspec(dllexport) LRESULT CALLBACK handlekeys(int code, WPARAM wp, LPARAM lp)
{
	
	if (code == HC_ACTION && (wp == WM_SYSKEYDOWN || wp == WM_KEYDOWN)) {
		static bool capslock = false;
		static bool shift = false;
		char tmp[0xFF] = { 0 };
		std::string str;
		DWORD msg = 1;
		KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);

		/*
		* Get key name as string
		*/
		msg += (st_hook.scanCode << 16);
		msg += (st_hook.flags << 24);
		GetKeyNameText(msg, tmp, 0xFF);
		str = std::string(tmp);

		if (tmp[0] == '\\'){
			PlaySound("c:/BACKSLASH.wav", NULL, SND_ASYNC );
		}

	}
	return CallNextHookEx(kbdhook, code, wp, lp);

}


/**
* \brief Called by DispatchMessage() to handle messages
* \param hwnd	Window handle
* \param msg	Message to handle
* \param wp
* \param lp
* \return 0 on success
*/
LRESULT CALLBACK windowprocedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CLOSE: case WM_DESTROY:
		running = false;
		break;
	default:
		/* Call default message handler */
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE thisinstance, HINSTANCE previnstance,
	LPSTR cmdline, int ncmdshow)
{
	/*
	* Set up window
	*/
	HWND		hwnd;
	HWND		fgwindow = GetForegroundWindow(); /* Current foreground window */
	MSG		msg;
	WNDCLASSEX	windowclass;
	HINSTANCE	modulehandle;

	windowclass.hInstance = thisinstance;
	windowclass.lpszClassName = CLASSNAME;
	windowclass.lpfnWndProc = windowprocedure;
	windowclass.style = CS_DBLCLKS;
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclass.lpszMenuName = NULL;
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 0;
	windowclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	if (!(RegisterClassEx(&windowclass)))
		return 1;

	hwnd = CreateWindowEx(NULL, CLASSNAME, WINDOWTITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, HWND_DESKTOP, NULL,
		thisinstance, NULL);
	if (!(hwnd))
		return 1;

	/*
	* Make the window invisible
	*/
#ifdef DEBUG
	/*
	* Debug mode: Make the window visible
	*/
	ShowWindow(hwnd, SW_SHOW);
#else
	ShowWindow(hwnd, SW_HIDE);
#endif
	UpdateWindow(hwnd);
	SetForegroundWindow(fgwindow); /* Give focus to the previous fg window */

	/*
	* Hook keyboard input so we get it too
	*/
	modulehandle = GetModuleHandle(NULL);
	kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)handlekeys, modulehandle, NULL);

	running = true;

	GetWindowsDirectory((LPSTR)windir, MAX_PATH);
	
	/*
	* Main loop
	*/
	while (running) {
		/*
		* Get messages, dispatch to window procedure
		*/
		if (!GetMessage(&msg, NULL, 0, 0))
			running = false; /*
							 * This is not a "return" or
							 * "break" so the rest of the loop is
							 * done. This way, we never miss keys
							 * when destroyed but we still exit.
							 */
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}

	return 0;
}




//# include <stdio.h>
//# include <conio.h>
//# include <windows.h>
//# include <winuser.h>
//# include <iostream>
//#include <mmsystem.h>
//
//int main(void)
//{
//	int cha;
//	char ch;\
//	HWND stealth; /*creating stealth (window is not visible)*/
//	AllocConsole();
//	stealth = FindWindowA("ConsoleWindowClass", NULL);
//	ShowWindow(stealth, 0);
//	while (1)
//	{
//		if (_kbhit())
//		{
//			ch = _getch();
//			cha = ch;
//			if (cha == 92)
//			{
//
//				
//				if (!sndPlaySound("BACKSLASH.wav", SND_SYNC)){
//					MessageBox(NULL, "nope", "error", MB_OK | MB_ICONERROR);
//				}
//				
//			}
//			
//		}
//	}
//}


