#ifndef _FRACT_VIEW_H
#define _FRACT_VIEW_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

class FractView : public BView {
   public:
      FractView(BRect);
      virtual ~FractView();
      virtual void MessageReceived(BMessage*);
   private:
       BBox *StatusBar;
  
};
#endif