
#ifndef __MainWindow_H__
#define __MainWindow_H__

#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Roller.H>

class GLWindow;
class FltkDisplay;

//-----------------------------------------------------
// Class: MainWindow
//
// Description: Defines our main window
//-----------------------------------------------------
class MainWindow :  public Fl_Window
{
public:

   MainWindow(int x, int y, int w, int h, const char* l = nullptr);
   MainWindow(int x, int y);
   virtual ~MainWindow();

   // set up our GUI
   void setupGui(FltkDisplay* myDis);
   // our exit function by the 'X' key or Ctrl + 'c'
   virtual void quitApp();
   // menu item callbacks
   virtual void menuCB(const char* x);
   // roller callbacks
   virtual void setYRotation(const double x);
   virtual void setXRotation(const double x);
   virtual void setZRotation(const double x);

private:

   static const int MAX_GL_WINS = 2;
   static const int MAX_MENU_ITEMS = 2;
   GLWindow* glWins[MAX_GL_WINS];  // our GL window(s)
   Fl_Menu_Bar* menuBar;           // our menu bar
   Fl_Roller* vRoll;               // vertical roller
   Fl_Roller* hRoll;               // horizontal roller
   Fl_Roller* zRoll;               // roller around the Z-axis
   int numWins;                    // number of GL windows we have.
};

#endif
