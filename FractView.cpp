/*******************************************************
*   Fract©
*
*   This is a port of my fractal that I originaly did
*   in OpenGL (GLUT) for my Compter Science Graphics 
*   Class. 
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    November 14 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>

#include <stdlib.h>

#include "Globals.h"
#include "FractView.h"
#include "FractDrawer.h"


/*******************************************************
*
*******************************************************/
FractView::FractView(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW){
   SetViewColor(216,216,216);

   BRect b = Bounds();
   
   AddChild(new FractDrawer(BRect(b.left,b.top,b.right,b.bottom-15)));

   b = Bounds();
   StatusBar = new BBox(BRect(b.left-3,b.bottom-14,b.right+3,b.bottom+3),"StatusBar",B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM); 
   AddChild(StatusBar);
}

/*******************************************************
*
*******************************************************/
FractView::~FractView(){
}


/*******************************************************
*
*******************************************************/
void FractView::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BView::MessageReceived(msg);
   }
}
