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
#include <PopUpMenu.h>
#include <String.h>

#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>

#include "Globals.h"
#include "FractDrawer.h"


/**************************************************************************
*   setupcolor 
*       
*   @param 
*   @return void
**************************************************************************/
void FractDrawer::setupColor(){
   int j,k,l;
   float tmpRed[5000];
   float tmpGreen[5000];
   float tmpBlue[5000];
   
   for (j = 0;j < ColorDepth;j += Jump){
      tmpRed[j]  = (float)(rand() % ColorDepth) / (float)ColorDepth;
      tmpGreen[j]= (float)(rand() % ColorDepth) / (float)ColorDepth;
      tmpBlue[j] = (float)(rand() % ColorDepth) / (float)ColorDepth;
   }
   for (k = 0;k < ColorDepth;k += Jump){
      for (l = k + 1;(l < (k + Jump)) && (l < ColorDepth);l++){
         tmpRed[l]  = tmpRed[k] + ((float(l-k) * (tmpRed[k + Jump] - tmpRed[k]) / float(Jump-1)));
         tmpGreen[l]= tmpGreen[k] + ((float(l-k) * (tmpGreen[k + Jump] - tmpGreen[k]) / float(Jump-1)));
         tmpBlue[l] = tmpBlue[k] + ((float(l-k) * (tmpBlue[k + Jump] - tmpBlue[k]) / float(Jump-1)));
      }
   }
   tmpRed[1000 % modnum] = 0.0;
   tmpGreen[1000 %modnum]= 0.0;
   tmpBlue[1000 %modnum] = 0.0;
   
   for (j = 0;j < ColorDepth;j++){
      Red[j]  = (int)(250 * tmpRed[j]);
      Grenn[j] = (int)(250 * tmpGreen[j]);
      Blue[j] = (int)(250 * tmpBlue[j]);
   }
}

/**************************************************************************
*   pixPlot 
*       plot a pixel on our screen at the x,y position and thats all
*   @param int x is the x value
*   @param int y is our y value
*   @param int c is the color we want it in.
*   @return void
**************************************************************************/
void FractDrawer::pixPlot(int x, int y, int c){
   c = (c) % modnum; 
  canvis->Lock();
  drawer->SetHighColor(Red[c],Green[c],Blue[c]);
  drawer->StrokeLine(BPoint(x,y),BPoint(x,y));
  canvis->Unlock();
}

/*******************************************************
*
*******************************************************/
FractDrawer::FractDrawer(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS|B_PULSE_NEEDED){
   SetViewColor(B_TRANSPARENT_32_BIT);
   windW = Bounds().IntegerWidth();
   windH = Bounds().IntegerHeight();
   
   canvis = new BBitmap(Bounds(),B_RGB32,true);
   drawer = new BView(Bounds(),"",B_FOLLOW_NONE,B_WILL_DRAW);
   canvis->Lock();
   canvis->AddChild(drawer);
   canvis->Unlock();
   
   X1 = -2;
   X2 =  2;
   Y1 = -2;
   Y2 =  2;
   Depth = 1000;

   ColorDepth = 3000;
   modnum = 1000;
   Jump = 25;
   
   zoom = 2;
   srand(system_time());
   setupColor();
   
   
   Menu = new BPopUpMenu("Image Menu");
   MakeMenu(Menu);
   LoadBookmarks();
   
   ReCalc = true;
}


/*******************************************************
*
*******************************************************/
FractDrawer::~FractDrawer(){
}

/*******************************************************
*
*******************************************************/
void FractDrawer::FrameResized(float W,float H){
   windW = (int)W;
   windH = (int)H;
   
   //ReCalc = true;
}

/*******************************************************
*
*******************************************************/
void FractDrawer::Pulse(){
   if(ReCalc){
      resume_thread(render_thread = spawn_thread(Render_Hook, "Render", B_NORMAL_PRIORITY, this));
      ReCalc = false;
   }
   Invalidate();

}

/*******************************************************
*
*******************************************************/
void FractDrawer::MouseDown(BPoint p){
   BMessage *currentMsg = Window()->CurrentMessage();
   if (currentMsg->what == B_MOUSE_DOWN) {
      uint32 buttons = 0;
      currentMsg->FindInt32("buttons", (int32 *)&buttons);
      uint32 modifiers = 0;
      currentMsg->FindInt32("modifiers", (int32 *)&modifiers);

      if(buttons & B_PRIMARY_MOUSE_BUTTON){
         double x2minusx1 = X2 - X1;
         double y2minusy1 = Y2 - Y1;
         int zoomXtwo = 2 * zoom;
         double newx2,newx1,newy2,newy1;
     
         newx2 = ((p.x)/(double)windW) * x2minusx1 + X1 + x2minusx1/(double)(zoomXtwo);
         newx1 = ((p.x)/(double)windW) * x2minusx1 + X1 - x2minusx1/(double)(zoomXtwo);
         newy2 = Y2 - ((windH - p.y)/(double)windH) * y2minusy1 + y2minusy1/(double)(zoomXtwo);
         newy1 = Y2 - ((windH - p.y)/(double)windH) * y2minusy1 - y2minusy1/(double)(zoomXtwo);
         
         X2 = newx2;
         X1 = newx1;
         Y2 = newy2;
         Y1 = newy1;
         
         ReCalc = true;
         //Invalidate();
      
      }else if(buttons & B_SECONDARY_MOUSE_BUTTON){
         
         BMenuItem *selected = Menu->Go(ConvertToScreen(p));
         if(selected){
            //(selected->Target())->PostMessage(selected->Message()->what);
            if(selected->Message()){
               //Looper()->PostMessage(selected->Message()->what);
               MessageReceived(selected->Message());
            }
         }
      }
   }
}

/*******************************************************
*   Makes that nice right click menu that we all love
*   this should consist of the Major things the user 
*   wants to do .. quick and easy.
*******************************************************/
void FractDrawer::MakeMenu(BMenu* menu){
  // BMenuItem* item;
   BMenu *submenu;
/*   menuItem = new BMenuItem("About ", new BMessage(B_ABOUT_REQUESTED));
   menu->AddItem(menuItem);
   menuItem = new BMenuItem("Help...", new BMessage(msg_help));
   menu->AddItem(menuItem);
   menu->AddSeparatorItem();
*/

   bmarkmenu = new BMenu("BookMarks");
   bmarkmenu->AddItem(new BMenuItem("Add Bookmark",NULL));
   
   menu->AddItem(bmarkmenu);
   submenu = new BMenu("Zoom factor");
   submenu->AddItem(new BMenuItem("1X",NULL));
   submenu->AddItem(new BMenuItem("2X",NULL));
   submenu->AddItem(new BMenuItem("4X",NULL));
   menu->AddItem(submenu);

   submenu = new BMenu("Depth");
   submenu->AddItem(new BMenuItem("64",NULL));
   submenu->AddItem(new BMenuItem("256",NULL));
   submenu->AddItem(new BMenuItem("1,000",NULL));
   submenu->AddItem(new BMenuItem("2,000",NULL));
   submenu->AddItem(new BMenuItem("5,000",NULL));
   submenu->AddItem(new BMenuItem("10,000",NULL));
   menu->AddItem(submenu);

   submenu = new BMenu("Color Depth");
   submenu->AddItem(new BMenuItem("Re Color",new BMessage(RECOLOR),'R'));
   submenu->AddItem(new BMenuItem("64 Colors",NULL));
   submenu->AddItem(new BMenuItem("256 Colors",NULL));
   submenu->AddItem(new BMenuItem("1,000 Colors",NULL));
   menu->AddItem(submenu);

   menu->AddItem(new BMenuItem("Toggle FullScreen",NULL,'F'));
  
   menu->AddItem(new BMenuItem("Increment Jump",new BMessage(INCJUMP)));
   menu->AddItem(new BMenuItem("Decrement Jump",new BMessage(DECJUMP)));
   
   menu->AddItem(new BMenuItem("Save As",NULL));

   submenu = new BMenu("Set as Background");
   submenu->AddItem(new BMenuItem("Centered",NULL));
   submenu->AddItem(new BMenuItem("Scaled",NULL));
   submenu->AddItem(new BMenuItem("Tiled",NULL));
   menu->AddItem(submenu);
   menu->AddItem(new BSeparatorItem());

   menu->AddItem(new BMenuItem("Quit",new BMessage(B_QUIT_REQUESTED)));
   ///menu->AddItem(submenu);
}

float sign(float x){
   if(x > 0){
      return 1.0;
   }else if(x < 0){
      return -1.0;
   }else{
      return 0.0;
   }
}

/*******************************************************
*
*******************************************************/
int32 FractDrawer::Render(){
   long double Cx;
   long double Cy;
   long double x;
   long double y;
   long double xsqr;
   long double ysqr;
   int64 count;
   int Wmin1 = windW-1;
   int Hmin1 = windH-1;

   int i,j;
  // int ctwo = 2, cfour = 4;



//---------------------------------------------
/*  float tempx; 
  long int count=0;
  long int scale=1;

 float  x=0;
  float y=0;

  float a = -20,//-3.14, 
        b = 20,//0.3, 
        c = -0.37;//0.3;

      a = 10;
      b = 1;
      c = 0;
      x = 0;
      y = 0;
      scale = 10;
      count = 0;

 while((count < 100000000)   ){//
      count++;
      pixPlot(scale*x,scale*y,(count/10000));
      tempx = x;
      x = y - sign(x) * sqrt(fabs(b * x - c));
      y = a - tempx;
   }*/
//---------------------------------------------


      for(i = 0; i <= windW+1; i++){
         Cx = X1 + ((i*(X2-X1))/Wmin1);
         for(j = 0; j <= windH+1; j++){
            Cy = Y1 + ((j * (Y2-Y1))/Hmin1);
            count = 0; x = 0; y = 0; xsqr = 0; ysqr = 0; 
            while(((xsqr+ysqr) < 4) && (count < Depth)){
            //while( (  (x < 2)&&(x > -2)&&(y < 2)&&(y > -2)  ) && (count < Depth)){
               count++;
               y = 2 * x * y + Cy;           
               x = xsqr - ysqr + Cx;            
               xsqr = x*x;
               ysqr = y*y; 
            }
            pixPlot(i,j,count);
          }
      }
      ReCalc = false;

   return B_OK;
}
 
/*******************************************************
*
*******************************************************/
void FractDrawer::Draw(BRect f){
   DrawBitmapAsync(canvis,canvis->Bounds(),Bounds());
   Sync();
}

/*******************************************************
*
*******************************************************/
void FractDrawer::LoadBookmarks(){
   fstream Bookfile;
   char markName[20];
   //BString markName;
   //BFile Bookfile("Bookmark.pst",B_READ_ONLY);
   long double Bmarkx1, Bmarky1, Bmarkx2, Bmarky2;
   BMessage *msg;
   int bsize = 0;
   
   char BFileName[13] = { "Bookmark.pst" };
   Bookfile.open(BFileName,ios::in);
   Bookfile >> bsize;
   //Bookfile.Read(&bsize,sizeof(int));
   BString t;
   t << (uint32)bsize;
   (new BAlert(NULL,t.String(),""))->Go();

   for(int i = 0; i < bsize; i++){
      Bookfile >> setprecision(100) >> Bmarkx1;
      Bookfile >> setprecision(100) >> Bmarky1;
      Bookfile >> setprecision(100) >> Bmarkx2;
      Bookfile >> setprecision(100) >> Bmarky2;
      //Bookfile.Read(&Bmarkx1,sizeof(long double));
      //Bookfile.Read(&Bmarky1,sizeof(long double));
      //Bookfile.Read(&Bmarkx2,sizeof(long double));
      //Bookfile.Read(&Bmarky2,sizeof(long double));
      //markName = ReadLine(&Bookfile);
      
      
      Bookfile >> markName; //NEADS TO HANDLE SPACES IN NAME
      //glutSetMenu(submenu1);
      //glutAddMenuEntry(markName,i);
      //glutSetMenu(menu);
      
      msg = new BMessage(OPEN_BMARK);
      msg->MakeEmpty();
      msg->AddData("x1",B_RAW_TYPE,&Bmarkx1,sizeof(long double));
      msg->AddData("x2",B_RAW_TYPE,&Bmarkx2,sizeof(long double));
      msg->AddData("y1",B_RAW_TYPE,&Bmarky1,sizeof(long double));
      msg->AddData("y2",B_RAW_TYPE,&Bmarky2,sizeof(long double));
      bmarkmenu->AddItem(new BMenuItem(markName,msg));
   }
   Bookfile.close();
}

/*******************************************************
*   Read one line of the file. 
*******************************************************/
BString FractDrawer::ReadLine(BFile *file){
   BString line;
   char a = '\\';
   line.SetTo("");
   while(file->Read((void*)&a,1) && (a != '\n')){
      line << a;
   } 
   //line << '\n';
   return line;
}

/*******************************************************
*
*******************************************************/
void FractDrawer::MessageReceived(BMessage *msg){
   switch(msg->what){
   case RECOLOR:
      setupColor();
      ReCalc = true;
      break;
   case INCJUMP:
      Jump += 10;
      setupColor();
      ReCalc = true;
      break;
   case DECJUMP:
      Jump -= 5;
      if(Jump < 1){Jump = 1;}
      setupColor();
      ReCalc = true;
      break;
   default:
      BView::MessageReceived(msg);
   }
}
