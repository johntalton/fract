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
#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <Path.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "Globals.h"
#include "FractWindow.h"
#include "FractView.h"
#include "TPreferences.h"

/*******************************************************
*   Our wonderful BWindow, ya its kewl like that.
*   we dont do much here but set up the menubar and 
*   let the view take over.  We also nead some message
*   redirection and handling
*******************************************************/
FractWindow::FractWindow(BRect frame) : BWindow(frame,"YNOP's Fractal",B_TITLED_WINDOW,B_ASYNCHRONOUS_CONTROLS){//|B_NOT_ZOOMABLE|B_NOT_RESIZABLE
   BRect r;
   BMenu *menu;
   BMenuItem *item;
   
   r = Bounds();
   // Creat a standard menubar
   menubar = new BMenuBar(r, "MenuBar");
   // Standard File menu
   menu = new BMenu("File");
  // menu->AddItem(new BSeparatorItem());
   menu->AddItem(item=new BMenuItem("About Fract", new BMessage(B_ABOUT_REQUESTED), 'A'));
   item->SetTarget(be_app);
   menu->AddItem(new BSeparatorItem());
   menu->AddItem(item=new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
   // add File menu now
   menubar->AddItem(menu);
   
   // Attach the menu bar to he window
   AddChild(menubar);
   
   // Do a little claculating to take menubar int account
   r = Bounds();
   r.bottom = r.bottom - menubar->Bounds().bottom;

   View = new FractView(r);
   View->MoveBy(0, menubar->Bounds().Height() + 1);

   AddChild(View);
   View->MakeFocus(true);
   Show();
   

}

/*******************************************************
*  
*******************************************************/
void FractWindow::FrameResized(float,float){
}

/*******************************************************
*  
*******************************************************/
void FractWindow::WindowActivated(bool active){
}

/*******************************************************
*   More nothingness. pass menu msg down to View.
*   like new game and pause and stuff like that.
*******************************************************/
void FractWindow::MessageReceived(BMessage* msg){
   switch(msg->what){
   default:
      BWindow::MessageReceived(msg);
   }
}

/*******************************************************
*   Someone asked us nicely to quit. I gess we should
*   so clean up. save our setings (position of win)
*   and tell the main be_app to shut it all down .. bye
*******************************************************/
bool FractWindow::QuitRequested(){
   TPreferences prefs("Fract_prefs");
   if (prefs.InitCheck() != B_OK) {
   }
   prefs.SetRect("window_pos", Frame());

   be_app->Lock();
   be_app->Quit();
   be_app->Unlock();
   return true;
}
