// Mode Switch Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "ModeSwitchDemo.h"
#include "ModeSwitchAPI.h"

#include <ddraw.h>
#include <dinput.h>


// Global Variables:
LPDIRECTDRAW g_lpDD;  // the DirectDraw object

LPDIRECTDRAWSURFACE g_lpPrimary;  // primary buffer
LPDIRECTDRAWSURFACE g_lpBack;     // back buffer

LPDIRECTDRAWCLIPPER g_lpClipper;  // the windowed mode clipper

bool g_bRunGame;   // pause or continue the game engine
bool g_bExclusive; // current mode -- windowed or fullscreen


// Foward declarations of functions included in this code module:
// link lib from ddraw.dll
typedef _Check_return_ HRESULT (WINAPI *fpDirectDrawCreate)(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter);

void CreateDirectDraw();
void DestroyDirectDraw();

void CreateSurfaces(bool bExclusive, int nWidth, int nHeight, int nBPP);
void DestroySurfaces();

void CreateMainWindow();
void DestroyMainWindow();

RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient);

void AdjustMainWindow(bool bExclusive, int nScreenWidth, int nScreenHeight);

void SwitchMode(bool bExclusive, int nWidth, int nHeight, int nBPP);

//
//  FUNCTION: CreateDirectDraw()
//
//  PURPOSE: Creates the DirectDraw object with a default driver.
//
//  COMMENTS:
//
//    Call this function once, when the app starts up.
//
void CreateDirectDraw()
{
	HRESULT hr = 0;
    fpDirectDrawCreate DirectDrawCreate;

    HINSTANCE hinstLib = LoadLibrary(TEXT("ddraw.dll"));
    if (hinstLib == NULL) {
        throw "Failed to load ddraw.dll";
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
    DirectDrawCreate = (fpDirectDrawCreate)GetProcAddress(hinstLib, "DirectDrawCreate");
#pragma GCC diagnostic pop
    if (DirectDrawCreate == NULL) {
        throw "Failed to GetProcAddress DirectDrawCreate";
    }

	// create the DirectDraw object
	hr = DirectDrawCreate(NULL, &g_lpDD, NULL);

	if( FAILED( hr ) )
		throw "Failed to create DirectDraw";
}

//
//  FUNCTION: DestroyDirectDraw()
//
//  PURPOSE: Destroys the DirectDraw object.
//
//  COMMENTS:
//
//    Call this function once, when the app shuts down.
//
void DestroyDirectDraw()
{
	if( g_lpDD )
	{
		// restore the previous display mode
		if( g_bExclusive )
			g_lpDD->RestoreDisplayMode();

		// release the DirectDraw object
		g_lpDD->Release();
		g_lpDD = NULL;
	}
}

//
//  FUNCTION: CreateSurfaces()
//
//  PURPOSE: Creates the necessary objects to use with a given mode.
//
//  COMMENTS:
//
//    This function is used only by the SwitchMode function.
//
void CreateSurfaces(bool bExclusive, int nWidth, int nHeight, int nBPP)
{
	HRESULT hr = 0;

	if( bExclusive )
	{
		// exclusive code

		// set the cooperative level to fullscreen mode
		hr = g_lpDD->SetCooperativeLevel(hMainWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);

		if( FAILED( hr ) )
			throw "Failed to set the cooperative level to fullscreen/exclusive";

		// set the display mode
		hr = g_lpDD->SetDisplayMode(640, 480, 16);

		if( FAILED( hr ) )
			throw "Failed to set the display mode";

		// create the buffering system
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof( ddsd ));

		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
		ddsd.dwBackBufferCount = 1;

		// create the primary surface
		hr = g_lpDD->CreateSurface(&ddsd, &g_lpPrimary, NULL);

		if( FAILED( hr ) )
			throw "Failed to create the primary buffer";

		ZeroMemory(&ddsd, sizeof( ddsd ));

		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

		hr = g_lpPrimary->GetAttachedSurface(&ddsd.ddsCaps, &g_lpBack);

		if( FAILED( hr ) )
			throw "Failed to create the back buffer";

		// save the mode
		g_bExclusive = bExclusive;
	}
	else
	{
		// windowed code

		// set the cooperative level to windowed mode
		hr = g_lpDD->SetCooperativeLevel(hMainWnd, DDSCL_NORMAL);

		if( FAILED( hr ) )
			throw "Failed to set the cooperative level to normal";

		// create the buffering system
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd, sizeof( ddsd ));

		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		// create the primary buffer
		hr = g_lpDD->CreateSurface(&ddsd, &g_lpPrimary, NULL);

		if( FAILED( hr ) )
			throw "Failed to create the primary buffer";

		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = nWidth;
		ddsd.dwHeight = nHeight;

		// create the back buffer
		hr = g_lpDD->CreateSurface(&ddsd, &g_lpBack, NULL);

		if( FAILED( hr ) )
			throw "Failed to create the back buffer";

		// create the clipper
		hr = g_lpDD->CreateClipper(0, &g_lpClipper, NULL);

		if( FAILED( hr ) )
			throw "Failed to create the clipper";

		hr = g_lpClipper->SetHWnd(0, hMainWnd);

		if( FAILED( hr ) )
			throw "Failed to set the clipper's HWND";

		hr = g_lpPrimary->SetClipper(g_lpClipper);

		if( FAILED( hr ) )
			throw "Failed to set the primary surface's clipper";

		// save the mode
		g_bExclusive = bExclusive;
	}
}

//
//  FUNCTION: DestroySurfaces()
//
//  PURPOSE: Destroys the necessary objects to use with a given mode.
//
//  COMMENTS:
//
//    This function is used only by the SwitchMode function.
//
void DestroySurfaces()
{
	if( g_bExclusive )
	{
		// exclusive code
		if( g_lpPrimary )
		{
			// release the flipping chain
			g_lpPrimary->Release();
			g_lpPrimary = NULL;
		}
	}
	else
	{
		// window code
		if( g_lpBack )
		{
			// release the back buffer
			g_lpBack->Release();
			g_lpBack = NULL;
		}

		if( g_lpPrimary )
		{
			// release the clipper (indirectly)
			g_lpPrimary->SetClipper(NULL);
			g_lpClipper->Release();
			g_lpClipper = NULL;

			// release the primary buffer
			g_lpPrimary->Release();
			g_lpPrimary = NULL;
		}
	}
}

//
//  FUNCTION: CreateMainWindow()
//
//  PURPOSE: Creates the main window.
//
//  COMMENTS:
//
//    This function is used only by the SwitchMode function.
//
void CreateMainWindow()
{
	// Create and display the main window
	hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

	if (!hMainWnd)
	  throw "Failed to create the main window";

//	ShowWindow(hMainWnd, nCmdShow);
//	UpdateWindow(hMainWnd);
}

//
//  FUNCTION: DestroyMainWindow()
//
//  PURPOSE: Destroys the main window, unhooking DirectX.
//
//  COMMENTS:
//
//    This function is used only by the SwitchMode function.
//
void DestroyMainWindow()
{
	if( IsWindow(hMainWnd) )
	{
		DestroyWindow(hMainWnd);
		hMainWnd = NULL;
	}
}

//
//  FUNCTION: CalculateWindowRect()
//
//  PURPOSE: Calculates the window rect, given a window and desired client size.
//
//  COMMENTS:
//
//    Use this function whenever you want to create or resize a window with a
//	  desired client area.
//
RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient)
{
    // declare a RECT to hold the results of our calculations
    RECT rcDesiredWindowRect;
    ZeroMemory(&rcDesiredWindowRect, sizeof( rcDesiredWindowRect ));

	// get the current window rect and its client rect
	RECT rcCurrentWindowRect;
	RECT rcCurrentClientRect;

	GetWindowRect(hWindow, &rcCurrentWindowRect);
	GetClientRect(hWindow, &rcCurrentClientRect);

	// get the difference between the current and desired client areas
	SIZE szClientDifference;

	szClientDifference.cx = (rcCurrentClientRect.right  - szDesiredClient.cx);
	szClientDifference.cy = (rcCurrentClientRect.bottom - szDesiredClient.cy);

	// get the desired window rect by adding the client area difference to the
	// current window rect
	rcDesiredWindowRect.left   = rcCurrentWindowRect.left;
	rcDesiredWindowRect.top    = rcCurrentWindowRect.top;
	rcDesiredWindowRect.right  = (rcCurrentWindowRect.right  - szClientDifference.cx);
	rcDesiredWindowRect.bottom = (rcCurrentWindowRect.bottom - szClientDifference.cy);

	return rcDesiredWindowRect;
}

//
//  FUNCTION: AdjustMainWindow()
//
//  PURPOSE: Adjusts the main window size, position, and style, given the display mode.
//
//  COMMENTS:
//
//    This function is used by the SwitchMode function.
//
void AdjustMainWindow(bool bExclusive, int nScreenWidth, int nScreenHeight)
{
	static HMENU hOldMenu = NULL;

	// hide the window while we're working...
	ShowWindow(hMainWnd, SW_HIDE);

	if( bExclusive )
	{
		// exclusive mode code
		if( !g_bExclusive )
		{
			// change the style of the window
			SetWindowLong(hMainWnd, GWL_STYLE, WS_POPUP);

			// remove the menu and save the old menu
			hOldMenu = GetMenu(hMainWnd);
			SetMenu(hMainWnd, NULL);
		}
	}
	else
	{
		// windowed mode code

		if( g_bExclusive )
		{
			// change the style of the window
			SetWindowLong(hMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			// Remove the WS_EX_TOPMOST style
			//LONG nOldExStyle = GetWindowLong(hMainWnd, GWL_EXSTYLE);
			//nOldExStyle &= WS_EX_TOPMOST;
			//nOldExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE;
			//SetWindowLong(hMainWnd, GWL_EXSTYLE, nOldExStyle);

			// attach the old menu
			if( hOldMenu )
				SetMenu(hMainWnd, hOldMenu);
		}

		// calculate the new window rect
		SIZE szDesiredClient;

		szDesiredClient.cx = nScreenWidth;
		szDesiredClient.cy = nScreenHeight;

		RECT rcNewWindowRect = CalculateWindowRect(hMainWnd, szDesiredClient);

		// resize the window according to the new rect
		MoveWindow(hMainWnd, rcNewWindowRect.left, rcNewWindowRect.top, 
			(rcNewWindowRect.right - rcNewWindowRect.left),
			(rcNewWindowRect.bottom - rcNewWindowRect.top), true);
	}

	// unhide the window now that we're done with it
	ShowWindow(hMainWnd, SW_SHOW);
}

//
//  FUNCTION: SwitchMode()
//
//  PURPOSE: Handles all mode-changing. This is the only API function.
//
//  COMMENTS:
//
//    This function is used to change modes any time during running.
//
void SwitchMode(bool bExclusive, int nWidth, int nHeight, int nBPP)
{
	// destroy any existing surfaces and clippers
	DestroySurfaces();
	DestroyDirectDraw();

	// destroy the main window and create it again
	DestroyMainWindow();
	CreateMainWindow();

	// make any changes to the window
	AdjustMainWindow(bExclusive, nWidth, nHeight);

	// create new surfaces and change the cooperative level and display mode
	CreateDirectDraw();
	CreateSurfaces(bExclusive, nWidth, nHeight, nBPP);
}
