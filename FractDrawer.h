#ifndef _FRACT_DRAWER_H
#define _FRACT_DRAWER_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <String.h>

class FractDrawer : public BView {
   public:
      FractDrawer(BRect);
      virtual ~FractDrawer();
      virtual void FrameResized(float,float);
      virtual void MouseDown(BPoint);
      virtual void Draw(BRect);
      virtual void MessageReceived(BMessage*);
      virtual void Pulse();
      static int32 Render_Hook(void* obj){
			return ((FractDrawer*)obj)->Render();
      }
   private:
      int SetSub(const int,const int);
      void SetSub(const int,const int,const int);
      void setupColor();
      void pixPlot(int,int,int);
      void MakeMenu(BMenu*);
      int32 Render();
      void LoadBookmarks();
      BString ReadLine(BFile *);
      
      BMenu *bmarkmenu;
      BPopUpMenu *Menu;
      thread_id render_thread;
      
      BBitmap *canvis;
      BView *drawer;
         
      long double X1;
      long double X2;
      long double Y1;
      long double Y2;
      int64 Depth;
      int windW;
      int windH;
  
      int ColorDepth;
      int modnum;
      int Jump;

      int Red[5000];
      int Green[5000];
      int Blue[5000];
      
      //int *Set;
      
      int zoom;
  
      bool ReCalc;
  
};
#endif