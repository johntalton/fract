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
#include "Fract.h"

// Application's signature
const char *APP_SIGNATURE = "application/x-vnd.Abstract-Fract";

/*******************************************************
*   Main.. What else do you want from me.
*******************************************************/
int main(int argc, char* argv[]){
   Fract *app = new Fract();
   
   app->Run();
   delete app;
   return B_NO_ERROR;
}