#ifndef _OUTLAW_H
#define _OUTLAW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "FractWindow.h"

extern const char *APP_SIGNATURE;
class Fract : public BApplication {
   public:
      Fract();
      ~Fract();
      virtual void AboutRequested();
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:
      FractWindow *theWin;
};
#endif