#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool quited = false;

void on_delete(Display *display, Window window) {
  XDestroyWindow(display, window);
  quited = true;
}

extern int main(int argc, char *argv[]) {
  Display *display = XOpenDisplay(NULL);
  if (NULL == display) {
    fprintf(stderr, "Failed to initialize display");
    return EXIT_FAILURE;
  }

  Window root = DefaultRootWindow(display);
  if (None == root) {
    fprintf(stderr, "No root window found");
    XCloseDisplay(display);
    return EXIT_FAILURE;
  }

  Window window =
      XCreateSimpleWindow(display, root, 0, 0, 800, 600, 0, 0, 0xffffffff);
  if (None == window) {
    fprintf(stderr, "Failed to create window");
    XCloseDisplay(display);
    return EXIT_FAILURE;
  }

  XMapWindow(display, window);

  Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &wm_delete_window, 1);

  // XColor color;
  // Colormap colormap;

  // // Get the default colormap
  // colormap = DefaultColormap(display, window);

  // // Allocate the color by name (e.g., "red")
  // if (!XAllocNamedColor(display, colormap, "red", &color, &color)) {
  //     fprintf(stderr, "Failed to allocate color\n");
  //     return EXIT_FAILURE;
  // }

  XSelectInput(display, window, ExposureMask | KeyPressMask);
  GC gc = XCreateGC(display, window, 0, NULL);
  if (gc == NULL) {
    fprintf(stderr, "Faied to create GC");
    XCloseDisplay(display);
    return EXIT_FAILURE;
  }

  int screen = DefaultScreen(display);
  int black = XBlackPixel(display, screen);
  int white = XWhitePixel(display, screen);

  XEvent event;
  while (!quited) {
    XNextEvent(display, &event);

    switch (event.type) {
    case ClientMessage: {
      if (event.xclient.data.l[0] == wm_delete_window) {
        on_delete(event.xclient.display, event.xclient.window);
      }
    } break;
    case Expose: {
      XSetForeground(display, gc, black);
      XFillRectangle(display, window, gc, 20, 20, 200, 200);
      XFlush(display);
    } break;
    }
  }

  XFreeGC(display, gc);
  // XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
