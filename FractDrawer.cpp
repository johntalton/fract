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
#include "FractDrawer.h"



//Al's kick ass Matrix from a single dynamic array. Thanks Al.
int SZ;
int FractDrawer::SetSub( const int a, const int b) {
  // lvalue
  return Set[ a * SZ + b];
}

void FractDrawer::SetSub( const int a, const int b, const int c){
  // rvalue
  Set[ a * SZ + b] = c;
}
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
      Green[j]= (int)(250 * tmpGreen[j]);
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

  SetHighColor(Red[c],Green[c],Blue[c]);
  StrokeLine(BPoint(x,y),BPoint(x,y));
}




/*******************************************************
*
*******************************************************/
FractDrawer::FractDrawer(BRect frame):BView(frame, "", B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_FRAME_EVENTS){
   SetViewColor(B_TRANSPARENT_32_BIT);
   windW = (int)Bounds().Width();
   windH = (int)Bounds().Height();
   X1 = -2;
   X2 =  2;
   Y1 = -2;
   Y2 =  2;
   Depth = 1000;

   ColorDepth = 2000;
   modnum = 1000;
   Jump = 25;
   
   if(windW > windH){
      SZ = windW + 1;
   }else {
      SZ = windH + 1;
   }
   Set = new int[SZ * SZ];

   zoom = 2;
   srand(system_time());
   setupColor();
   
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
   delete []Set;
   if(windW > windH){
      SZ = windW + 1;
   }else {
      SZ = windH + 1;
   }
   Set = new int[SZ * SZ];
   ReCalc = true;

}

/*******************************************************
*
*******************************************************/
void FractDrawer::MouseDown(BPoint p){
   double x2minusx1 = X2 - X1;
   double y2minusy1 = Y2 - Y1;
   int zoomXtwo = 2 * zoom;
   double newx2,newx1,newy2,newy1;
     
   newx2 = (p.x/(double)windW) * x2minusx1 + X1 + x2minusx1/(double)(zoomXtwo);
   newx1 = (p.x/(double)windW) * x2minusx1 + X1 - x2minusx1/(double)(zoomXtwo);
   newy2 = Y2 - (p.y/(double)windH) * y2minusy1 + y2minusy1/(double)(zoomXtwo);
   newy1 = Y2 - (p.y/(double)windH) * y2minusy1 - y2minusy1/(double)(zoomXtwo);
         
   X2 = newx2;
   X1 = newx1;
   Y2 = newy2;
   Y1 = newy1;
         
         
   //FillRect(BRect(X1,Y1,X2,Y2));
   ReCalc = true;
   Invalidate();
}

/*******************************************************
*
*******************************************************/
void FractDrawer::Draw(BRect f){
   long double Cx;
   long double Cy;
   long double x;
   long double y;
   long double xsqr;
   long double ysqr;
   int32 count;
  // int HalfwindW = (windW/2);
 //  int HalfwindH = (windH/2);
   int Wmin1 = windW-1;
   int Hmin1 = windH-1;

   int i,j;
  // int ctwo = 2, cfour = 4;

 //  glutSetCursor(GLUT_CURSOR_WAIT);
   if(ReCalc){
      for(i = 0; i <= windW+1; i++){
         Cx = X1 + ((i*(X2-X1))/Wmin1);
         for(j = 0; j <= windH+1; j++){
            Cy = Y1 + ((j * (Y2-Y1))/Hmin1);
            count = 0; x = 0; y = 0; xsqr = 0; ysqr = 0; 
            //while(((xsqr+ysqr) < 4) && (count < Depth)){
            while( (  (x < 2)&&(x > -2)&&(y < 2)&&(y > -2)  ) && (count < Depth)){
               count++;
               y = 2 * x * y + Cy;           
               x = xsqr - ysqr + Cx;            
               xsqr = x*x;
               ysqr = y*y; 
            }
            pixPlot(i,j,count);
            SetSub(i,j,count);
          }
      }
      ReCalc = 0;
   }else{
      for(i = 0; i < windW+1;i++){
         for(j = 0; j < windH+1; j++){
            pixPlot(i,j,SetSub(i,j));
         }
      }
   }
//   glutSetCursor(GLUT_CURSOR_CROSSHAIR);




}

/*******************************************************
*
*******************************************************/
void FractDrawer::MessageReceived(BMessage *msg){
   switch(msg->what){
   default:
      BView::MessageReceived(msg);
   }
}
