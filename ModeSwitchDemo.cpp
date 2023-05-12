// Mode Switch Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "ModeSwitchDemo.h"
#include "ModeSwitchAPI.h"

// Global Variables:
HINSTANCE hInst;						// current instance
HWND hMainWnd;							// current window
TCHAR szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	// The window class name

// Foward declarations of functions included in this code module:
void				GameLoop();
void				ProcessInput();
void				ProcessLogic();
void				ProcessGraphics();
void				ProcessSound();

void				DrawPixels(LPDIRECTDRAWSURFACE lpSurface);

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
BOOL				ExitInstance();

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//
//   FUNCTION: GameLoop()
//
//   PURPOSE: Runs the game.
//
//   COMMENTS:
//
//        Handles input, logic, graphics, and sound.
//
void GameLoop()
{
	// Process input
	ProcessInput();

	// Process logic
	ProcessLogic();

	// Process graphics
	ProcessGraphics();

	// Process sound
	ProcessSound();
}

//
//   FUNCTION: ProcessInput()
//
//   PURPOSE: Gathers input and stores it in a global structure.
//
//   COMMENTS:
//
//        Handles input.
//
void ProcessInput()
{
	// (found in WndProc under WM_COMMAND, for simplicity)
}

//
//   FUNCTION: ProcessLogic()
//
//   PURPOSE: Processes the current input and sets the proper game data.
//
//   COMMENTS:
//
//        Handles the game movement/AI/logic.
//
void ProcessLogic()
{

}

//
//   FUNCTION: ProcessGraphics()
//
//   PURPOSE: Processes the game data and outputs it to the screen(s).
//
//   COMMENTS:
//
//        Handles graphics.
//
void ProcessGraphics()
{
	// clear the back buffer before drawing
	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx, sizeof( ddbltfx ));

	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = RGB(0, 0, 0);

	g_lpBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);

	// draw pixels to the back buffer -- notice this doesn't change
	// between exclusive mode and windowed mode!
	DrawPixels(g_lpBack);

	// flip or blit it to the screen
	if( g_bExclusive )
	{
		// exclusive code
		g_lpPrimary->Flip(NULL, DDFLIP_WAIT);
	}
	else
	{
		// windowed code

		// calculate the client rect in screen coordinates
		RECT rect;
		ZeroMemory(&rect, sizeof( rect ));

		// get the client area
		GetClientRect(hMainWnd, &rect);

		// copy the rect's data into two points
		POINT p1;
		POINT p2;

		p1.x = rect.left;
		p1.y = rect.top;
		p2.x = rect.right;
		p2.y = rect.bottom;

		// convert it to screen coordinates (like DirectDraw uses)
		ClientToScreen(hMainWnd, &p1);
		ClientToScreen(hMainWnd, &p2);

		// copy the two points' data back into the rect
		rect.left   = p1.x;
		rect.top    = p1.y;
		rect.right  = p2.x;
		rect.bottom = p2.y;

		// blit the back buffer to our window's position
		g_lpPrimary->Blt(&rect, g_lpBack, NULL, DDBLT_WAIT, NULL);
	}
}

//
//   FUNCTION: ProcessSound()
//
//   PURPOSE: Processes the game data and outputs it to the speakers(s).
//
//   COMMENTS:
//
//        Handles sound.
//
void ProcessSound()
{
	// (no sound code, for simplicity)
}

//
//   FUNCTION: DrawPixels()
//
//   PURPOSE: Plots 10,000 random pixels to the specified surface.
//
//   COMMENTS:
//
//        This function is only used by the ProcessGraphics function.
//
void DrawPixels(LPDIRECTDRAWSURFACE lpSurface)
{
	HRESULT hr = 0;

	// Get the bit-depth of the surface, so we draw in different
	// bit-depths correctly
	DDPIXELFORMAT ddpf;
	ZeroMemory(&ddpf, sizeof( ddpf ));

	ddpf.dwSize = sizeof( ddpf );

	lpSurface->GetPixelFormat(&ddpf);

	// Get the surface's description and lock it.
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof( ddsd ));

	ddsd.dwSize = sizeof( ddsd );

	hr = lpSurface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	{
		if( FAILED( hr ) )
		{
			lpSurface->Unlock(NULL);
			throw "DrawPixels: Failed to lock the specified surface.";
		}

		// Plot the pixels
		int x = 0;
		int y = 0;
		unsigned char nColor = 0;

		const int nBytesPerPixel = ddpf.dwRGBBitCount / 8;
		const int nPitch = (int) (ddsd.lPitch / nBytesPerPixel);
		unsigned char* pVideoMemory = (unsigned char*) ddsd.lpSurface;

		for( int i=0; i < 10000; i++ )
		{
			x = (rand() % (ddsd.dwWidth  * nBytesPerPixel));
			y = (rand() % (ddsd.dwHeight * nBytesPerPixel));

			nColor = (rand() % 256);

			pVideoMemory[x + y * nPitch] = nColor;
		}
	}
	hr = lpSurface->Unlock(NULL);

	if( FAILED( hr ) )
		throw "DrawPixels: Failed to unlock the specified surface";
}

//
//   FUNCTION: WinMain()
//
//   PURPOSE: The virtual entry point for the app.
//
//   COMMENTS:
//
//        Handles startup, running, and shutdown.
//
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	// HACCEL hAccelTable;

	try {
		// Initialize global strings
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(hInstance, IDC_MODESWITCHDEMO, szWindowClass, MAX_LOADSTRING);
		MyRegisterClass(hInstance);

		// Perform application initialization:
		if (!InitInstance (hInstance, nCmdShow)) 
			throw "Failed to initialize the app";

		// hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MODESWITCHDEMO);
		LoadAccelerators(hInstance, (LPCTSTR)IDC_MODESWITCHDEMO);

		// Main message loop:
		for( ;; )
		{
			if( PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
			{
				// retrieve a message
				GetMessage(&msg, NULL, NULL, NULL);

				if( msg.message == WM_QUIT )
					break; // only way out of the for( ;; ) loop

				// dispatch the message to our WndProc
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if( g_bRunGame )
				{
					// game code here

					GameLoop();
				}
			}
		}

		// Perform application cleanup:
		if (!ExitInstance ())
			throw "Failed to cleanup after the app";
	}

	catch( char* pErrorText ) {
		MessageBox(hMainWnd, pErrorText, szTitle, MB_OK);
		return FALSE;
	};

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MODESWITCHDEMO);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_MODESWITCHDEMO;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	// Initialize COM
	CoInitialize(NULL);

	// Create DirectDraw, setup the window, and set the display mode
	SwitchMode(false, 640, 480, 16);

	return TRUE;
}

//
//   FUNCTION: ExitInstance()
//
//   PURPOSE: Cleans up the app.
//
//   COMMENTS:
//
//        This function destroys DirectDraw and its objects.
//
BOOL ExitInstance()
{
	// Destroy DirectDraw and its objects
	DestroySurfaces();
	DestroyDirectDraw();

	// Unintialize COM
	CoUninitialize();

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;//, wmEvent;
	PAINTSTRUCT ps;
	// HDC hdc;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			// wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hMainWnd);
				   PostQuitMessage(0);
//				   CloseWindow(hMainWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_KEYUP:
			switch( wParam )
			{
			case VK_ESCAPE: // end the program
				PostQuitMessage(0);
				break;
			case VK_F4:  // switch modes
				if( g_bExclusive )
				{
					// switch to windowed mode
					SwitchMode(false, 640, 480, 16);
				}
				else
				{
					// switch to fullscreen mode
					SwitchMode(true, 640, 480, 16);
				}
				break;
			}
			break;
		case WM_PAINT:
			// hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			// EndPaint(hWnd, &ps);
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_ACTIVATE:
			if( LOWORD( wParam ) == WA_INACTIVE )
			{
				// the user is now working with another app
				g_bRunGame = false;
			}
			else
			{
				// the user has now switched back to our app
				g_bRunGame = true;
			}
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
