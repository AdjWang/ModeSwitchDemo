# Mode Switch Demo

Reaaaaaaaally nice code by null_pointer:

- [Moving from Exclusive Mode to Windowed Mode in DirectX Part I](https://archive.gamedev.net/archive/reference/articles/article960.html)
- [Moving from Exclusive Mode to Windowed Mode in DirectX Part II](https://archive.gamedev.net/archive/reference/articles/article1034.html)

> Only wish the world fills with those genial programmers and graceful codes.

This is a demo of old ddraw API, supports changing display mode between fullscreen and windowed by `F4`.

Now let me borrow it as the test suite...

## Usage

1. Run `make -j$(nproc) all`
2. Run `bin/demo.exe`

Default build in debug mode. It can be set to release mode in the Makefile.

## Original README.txt

```
README.TXT

 - AUTHOR -

  nick:      null_pointer
  e-mail:    ratt96963@aol.com
  web site:  http://www.freeyellow.com/members8/nullpointer
  site name: Sabre Multimedia


 - FILE LIST -

  Mode Switch API.h	include file for Mode Switch API functions
  Mode Switch API.cpp	implementation file for Mode Switch API functions
  Mode Switch Demo.h	program-specific include file
  Mode Switch Demo.cpp	program-specific implementation file
  Mode Switch Demo.rc	program-specific resource symbol definition file
  resource.h		program-specific resource symbol include file
  small.ico		program-specific icon
  stdafx.h		standard project headers
  stdafx.cpp		includes stdafx.h
  ModeSwitch Demo.exe	Compiled program for Part 2


 - HOW TO USE THE PROGRAM -

  Press F4 to change between exclusive mode and windowed mode, at 640x480x16.
  Press ESC to exit the program, or choose exit from the File menu in
  windowed mode, or hit the close button on the window in windowed mode.

  Mode Switch Demo.exe plots 10,000 random pixels per frame, so you can see
  that the clipper, blitting, and stretched blitting all work as planned.

  This program requires 16-bit color at resolutions of 640x480 and higher.


 - HOW TO USE THE SOURCE -

  You can add in the Mode Switch API.h and Mode Switch Demo.cpp files to your
  own project, but you'll have to change the names of certain global variables
  which Mode Switch API.cpp is dependent upon to match the names of your own
  global variables of the same types. They are: hMainWnd, hInst, szTitle, and
  szWindowClass.

  Use the SwitchMode() function to set up the program window and DirectDraw. Use
  the SwitchMode() function anytime afterwards to change the mode and/or window
  size. To clean up, call DestroySurfaces() and DestroyDirectDraw() once each at
  the end of your program in that order.


 - SUGGESTIONS ON MODIFICATIONS - 

  You could probably provide menu items and/or hotkeys to change modes and
  window sizes. Also, you could change the window style to prevent maximizing
  and resizing of the window, as that would hinder performance slightly. (best
  to leave them on, though)


Thanks for your time!


 - null_pointer
```
