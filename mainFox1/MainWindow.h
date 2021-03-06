
#ifndef __MainWindow_H__
#define __MainWindow_H__

#include "fx.h"

//-------------------------------------------------------
// Class: MainWindow
// Description: Central window for the application
//-------------------------------------------------------
class MainWindow : public FXMainWindow
{
   FXDECLARE(MainWindow)

public:
   MainWindow(FXApp*);
   virtual ~MainWindow();

   // initialize
   void create();

   // define additional message IDs for this window
   enum {
      ID_CANVAS = FXMainWindow::ID_LAST,
      ID_SPIN,
      ID_SPINFAST,
      ID_STOP,
      ID_TIMEOUT,
      ID_CHORE,
      ID_MULTISAMPLE_OFF,
      ID_MULTISAMPLE_2X,
      ID_MULTISAMPLE_4X,
      ID_SPEED,
      ID_OPENGL
   };

   // message handlers
   long onMouseDown(FXObject*, FXSelector, void*);
   long onMouseUp(FXObject*, FXSelector, void*);
   long onMouseMove(FXObject*, FXSelector, void*);
   long onExpose(FXObject*, FXSelector, void*);
   long onConfigure(FXObject*, FXSelector, void*);
   long onCmdSpin(FXObject*, FXSelector, void*);
   long onUpdSpin(FXObject*, FXSelector, void*);
   long onCmdStop(FXObject*, FXSelector, void*);
   long onUpdStop(FXObject*, FXSelector, void*);
   long onTimeout(FXObject*, FXSelector, void*);
   long onChore(FXObject*, FXSelector, void*);
   long onCmdSpinFast(FXObject*, FXSelector, void*);
   long onUpdSpinFast(FXObject*, FXSelector, void*);
   long onCmdMultiSample(FXObject*, FXSelector, void*);
   long onUpdMultiSample(FXObject*, FXSelector, void*);
   long onUpdSpeed(FXObject*, FXSelector, void*);
   long onCmdOpenGL(FXObject*, FXSelector, void*);

protected:
   MainWindow()    {}

private:
   void drawScene();

   FXGLCanvas* glcanvas;         // GL canvas for drawing
   FXRealSpinner* speedcontrol;  // spinner control for speed
   FXGLVisual* glvisual;         // OpenGL visual

   FXdouble rts;
   FXTime lasttime;
   bool spinning;                // spinning flag
   double angle;                 // Rotation angle of box
   FXDataTarget dt_rts;
};

#endif
