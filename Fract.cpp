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
#include <StorageKit.h>
#include <Alert.h>
#include <Application.h>

#include <stdio.h>

#include "Fract.h"
#include "FractWindow.h"
#include "TPreferences.h"

/*******************************************************
*
*******************************************************/
Fract::Fract() : BApplication(APP_SIGNATURE){
   BRect wind_pos;

   BRect defaultSize(50,50,450,450);

   TPreferences prefs("Fract_prefs");
   if (prefs.InitCheck() != B_OK) {
      // New User!
   }

   if(prefs.FindRect("window_pos", &wind_pos) != B_OK){
      wind_pos = defaultSize;
   }
   
   if(!wind_pos.Intersects(BScreen().Frame())){
      (new BAlert(NULL,"The window was somehow off the screen. We reset it position for you","Thanks"))->Go();
      theWin = new FractWindow(defaultSize);
   }else{
      // this is the normal start up.
      theWin = new FractWindow(wind_pos);
   }
}

/*******************************************************
*
*******************************************************/
Fract::~Fract(){
}

/*******************************************************
*   Our lovely about box with hidden box
*******************************************************/
void Fract::AboutRequested(){
   (new BAlert("About Fract","Fract ©1999\n\nTheAbstractCompany\nynop@acm.org\n\nVersion: 1.0 beta 1.x","Thats Nice"))->Go();
}

/*******************************************************
*   Ah .. do nothing .. just defalut pass off.
*******************************************************/
void Fract::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BApplication::MessageReceived(msg);
      break; 
   }  
}

/*******************************************************
*   Make shure all preffs are saved and kill it all
*******************************************************/
bool Fract::QuitRequested(){
   if(theWin->QuitRequested()){
      return true; // Ok .. fine .. leave then
   }
   return false;
}






