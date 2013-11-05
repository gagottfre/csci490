#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <iostream>

int main()
{
	Display *display;
	unsigned int keycode;
	display = XOpenDisplay(NULL);

	//keycode = XKeysymToKeycode(display, XK_t);
	//XTestFakeKeyEvent(display, keycode, true, 0);

	for(int i = 0; i < 100; i++)
	{	
		XTestFakeRelativeMotionEvent(display, 1, 1, 100);
	}
	for(int i = 0; i < 100; i++)
	{	
		XTestFakeRelativeMotionEvent(display, 1, -1, 100);
	}
	for(int i = 0; i < 100; i++)
	{	
		XTestFakeRelativeMotionEvent(display, -1, -1, 100);
	}
	for(int i = 0; i < 100; i++)
	{	
		XTestFakeRelativeMotionEvent(display, -1, 1, 100);
	}

	XFlush(display);
}
