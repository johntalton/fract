#ifndef _OL_WINDOW_H
#define _OL_WINDOW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "FractView.h"


class FractWindow : public BWindow {
   public:
      FractWindow(BRect);
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
      virtual void FrameResized(float,float);
      virtual void WindowActivated(bool active);
   private:
      BMenuBar *menubar;
      FractView *View;

};
#endif