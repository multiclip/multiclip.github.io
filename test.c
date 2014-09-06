#include <X11/Xlib.h>
#include <X11/keysym.h>

// The key code to be sent.
// A full list of available codes can be found in /usr/include/X11/keysymdef.h
#define KEYCODE XK_Return

// Function to create a keyboard event
XKeyEvent createKeyEvent(Display *display, Window &win,
                           Window &winRoot, bool press,
                           int keycode, int modifiers)
{
   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.time        = CurrentTime;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycode);
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;
}

main()
{
// Obtain the X11 display.
   Display *display = XOpenDisplay(0);
   if(display == NULL)
      return -1;

// Get the root window for the current display.
   Window winRoot = XDefaultRootWindow(display);

// Find the window which has the current keyboard focus.
   Window winFocus;
   int    revert;
   XGetInputFocus(display, &winFocus, &revert);

// Send a fake key press event to the window.
   XKeyEvent event = createKeyEvent(display, winFocus, winRoot, true, KEYCODE, 0);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

// Send a fake key release event to the window.
   event = createKeyEvent(display, winFocus, winRoot, false, KEYCODE, 0);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

// Done.
   XCloseDisplay(display);
   return 0;
}
