
#if !defined(AFX_MODESWITCHAPI_H__F604EE40_03B1_11D4_AB54_0050BAB009EE__INCLUDED_)
#define AFX_MODESWITCHAPI_H__F604EE40_03B1_11D4_AB54_0050BAB009EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include <ddraw.h>


// Global Variables:
extern LPDIRECTDRAW g_lpDD;  // the DirectDraw object

extern LPDIRECTDRAWSURFACE g_lpPrimary;  // primary buffer
extern LPDIRECTDRAWSURFACE g_lpBack;     // back buffer

extern LPDIRECTDRAWCLIPPER g_lpClipper;  // the windowed mode clipper

extern bool g_bRunGame;   // pause or continue the game engine
extern bool g_bExclusive; // current mode -- windowed or fullscreen


// Foward declarations of functions included in this code module:
void CreateDirectDraw();
void DestroyDirectDraw();

void CreateSurfaces(bool bExclusive, int nWidth, int nHeight, int nBPP);
void DestroySurfaces();

void CreateMainWindow();
void DestroyMainWindow();

RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient);

void AdjustMainWindow(bool bExclusive, int nScreenWidth, int nScreenHeight);

void SwitchMode(bool bExclusive, int nWidth, int nHeight, int nBPP);


#endif // !defined(AFX_MODESWITCHAPI_H__F604EE40_03B1_11D4_AB54_0050BAB009EE__INCLUDED_)
