#include <iostream>
#include "array"
#include <cstdio>
#include <memory>
#include <stdexcept>

/* TinyWM is written by Nick Welch <mack@incise.org>, 2005.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

/* much of tinywm's purpose is to serve as a very basic example of how to do X
 * stuff and/or understand window managers, so i wanted to put comments in the
 * code explaining things, but i really hate wading through code that is
 * over-commented -- and for that matter, tinywm is supposed to be as concise
 * as possible, so having lots of comments just wasn't really fitting for it.
 * i want tinywm.c to be something you can just look at and go "wow, that's
 * it?  cool!"  so what i did was just copy it over to annotated.c and comment
 * the hell out of it.  ahh, but now i have to make every code change twice!
 * oh well.  i could always use some sort of script to process the comments out
 * of this and write it to tinywm.c ... nah.
 */

/* most X stuff will be included with Xlib.h, but a few things require other
 * headers, like Xmd.h, keysym.h, etc.
 */
#include <X11/Xlib.h>
#include <cstring>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

long RGB(int r, int g, int b) {
    return b + (g << 8) + (r << 16);
}

std::string exec(const char *cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    return result;
}

int main() {

    Display *dpy;
    Window root;
    Window bar;
    XWindowAttributes attr;

    GC graphicContext;

    /* we use this to save the pointer's state at the beginning of the
     * move/resize.
     */
    XButtonEvent start;

    XEvent ev;


    if (!(dpy = XOpenDisplay(0x0))) return 1;

    auto black = BlackPixel(dpy, root);
    auto white = WhitePixel(dpy, root);

    root = DefaultRootWindow(dpy);
    bar = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 1920, 100, 1, white,
                              black);

    XClearWindow(dpy, bar);
    XMapRaised(dpy, bar);


    graphicContext = XCreateGC(dpy, bar, 0, 0);


    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root,
             True, GrabModeAsync, GrabModeAsync);

    XGrabButton(dpy, 1, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync,
                GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync,
                GrabModeAsync, None, None);

    std::cout << "started" << std::endl;

    int d = 0;

    while (1) {
        XNextEvent(dpy, &ev);


        if (ev.type == KeyPress && ev.xkey.subwindow != None) {

            XKillClient(dpy, ev.xkey.subwindow);

            std::cout << "kill window" << std::endl;
        } else if (ev.type == ButtonPress && ev.xbutton.subwindow != None) {
            XGrabPointer(dpy, ev.xbutton.subwindow, True,
                         PointerMotionMask | ButtonReleaseMask, GrabModeAsync,
                         GrabModeAsync, None, None, CurrentTime);


            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        } else if (ev.type == MotionNotify) {
            if (ev.xmotion.window != bar) {


                int xdiff, ydiff;

                while (XCheckTypedEvent(dpy, MotionNotify, &ev));

                xdiff = ev.xbutton.x_root - start.x_root;
                ydiff = ev.xbutton.y_root - start.y_root;

                XMoveResizeWindow(dpy, ev.xmotion.window,
                                  attr.x + (start.button == 1 ? xdiff : 0),
                                  attr.y + (start.button == 1 ? ydiff : 0),
                                  MAX(1, attr.width + (start.button == 3 ? xdiff : 0)),
                                  MAX(1, attr.height + (start.button == 3 ? ydiff : 0)));

            }
        } else if (ev.type == ButtonRelease)
            XUngrabPointer(dpy, CurrentTime);


        XSetForeground(dpy, graphicContext, RGB(0, 0, 255));

        std::string text = "test " + d;
        XDrawString(dpy, bar, graphicContext, 40, 40, text.c_str(), text.length());
    }


}
