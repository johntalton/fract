/*******************************************************
*   Fract©
*
*   Niffty passwd cracker with threads :)
*
*   @author  TheAbstractCompany, YNOP(ynop@acm.org) 
*   @vertion beta
*   @date    November 5 1999
*******************************************************/
#include <AppKit.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <Alert.h>
#include <Application.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "Fract.h"
#include "FractWindow.h"

/*******************************************************
*
*******************************************************/
Fract::Fract() : BApplication(APP_SIGNATURE){
   BRect wind_pos;
   BPath path;
   int ref;
   BRect defaultSize(50,50,450,450);

   if(find_directory(B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
      path.Append("Fract_settings");
      ref = open(path.Path(),O_RDONLY);
      if(ref >= 0){
         read(ref, (char *)&wind_pos, sizeof(wind_pos));
         close(ref);
         if(!wind_pos.Intersects(BScreen().Frame())){
            theWin = new FractWindow(defaultSize);
         }else{
            theWin = new FractWindow(wind_pos);
         }
      }else{ // hmm that was not in there
         theWin = new FractWindow(defaultSize);
      }
   }else{ // gess there are no defaults ..first time?
      theWin = new FractWindow(defaultSize);
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






