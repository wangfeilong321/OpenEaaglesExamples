

#include "MainWindow.h"
#include "SettingsDialog.h"

#include "fx.h"
#include "fx3d.h"

// timer setting (in nanoseconds)
const FXTime TIMER_INTERVAL = 100000000;

//--------------------------------------------------------------------------------
// message types in map:
//
// SEL_PAINT : must repaint window
// SEL_CONFIGURE : resize
// SEL_COMMAND : GUI command
// SEL_UPDATE : GUI update
// SEL_TIMEOUT : timeout occurred
// SEL_CHORE : background chore
//--------------------------------------------------------------------------------

// message map which assoicates messages objects received to specific member functions
FXDEFMAP(MainWindow) MainWindowMap[] = {
   //___Message_Type________ID______________________________Message_Handler_______
   FXMAPFUNC(SEL_PAINT,     MainWindow::ID_CANVAS,          MainWindow::onExpose),
   FXMAPFUNC(SEL_CONFIGURE, MainWindow::ID_CANVAS,          MainWindow::onConfigure),
   FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_SPIN,            MainWindow::onCmdSpin),
   FXMAPFUNC(SEL_UPDATE,    MainWindow::ID_SPIN,            MainWindow::onUpdSpin),
   FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_SPINFAST,        MainWindow::onCmdSpinFast),
   FXMAPFUNC(SEL_UPDATE,    MainWindow::ID_SPINFAST,        MainWindow::onUpdSpinFast),
   FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_STOP,            MainWindow::onCmdStop),
   FXMAPFUNC(SEL_UPDATE,    MainWindow::ID_STOP,            MainWindow::onUpdStop),
   FXMAPFUNC(SEL_UPDATE,    MainWindow::ID_SPEED,           MainWindow::onUpdSpeed),
   FXMAPFUNC(SEL_TIMEOUT,   MainWindow::ID_TIMEOUT,         MainWindow::onTimeout),
   FXMAPFUNC(SEL_CHORE,     MainWindow::ID_CHORE,           MainWindow::onChore),
   FXMAPFUNC(SEL_COMMAND,   MainWindow::ID_OPENGL,          MainWindow::onCmdOpenGL),
   FXMAPFUNCS(SEL_COMMAND,  MainWindow::ID_MULTISAMPLE_OFF, MainWindow::ID_MULTISAMPLE_4X, MainWindow::onCmdMultiSample),
   FXMAPFUNCS(SEL_UPDATE,   MainWindow::ID_MULTISAMPLE_OFF, MainWindow::ID_MULTISAMPLE_4X, MainWindow::onUpdMultiSample),
};

// macro generated code (class name, base class name, pointer to message map, # of entries in message map)
FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))

MainWindow::MainWindow(FXApp* a):FXMainWindow(a, "OpenGL Test Application", nullptr, nullptr, DECOR_ALL, 0, 0, 800, 600)
{
   // right vertical frame that will contain buttons
   FXVerticalFrame* buttonFrame = new FXVerticalFrame(this, LAYOUT_SIDE_RIGHT|LAYOUT_FILL_Y, 0,0,0,0, 2,2,3,3);

   // button to open GL capabilities dialog
   new FXButton(buttonFrame, tr("&OpenGL Info\tDisplay OpenGL Capabilities"), nullptr, this,
                ID_OPENGL,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0, 10,10,5,5);

   // buttons to select the mode of spinning - timers or chores
   new FXButton(buttonFrame, tr("Spin &Timer\tSpin using interval timers\nNote the app blocks until the interal has elapsed..."),
                nullptr, this, ID_SPIN, FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0,10,10,5,5);
   new FXButton(buttonFrame, tr("Spin &Chore\tSpin as fast as possible using chores\nNote even though the app is very responsive, it never blocks;\nthere is always something to do..."),
                nullptr, this, ID_SPINFAST, FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

   // button to stop spinning
   new FXButton(buttonFrame, tr("&Stop Spin\tStop this mad spinning, I'm getting dizzy"),
                nullptr, this, ID_STOP, FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT, 0,0,0,0, 10,10,5,5);

   // group box which will contain a spinner control
   FXGroupBox* groupbox1 = new FXGroupBox(buttonFrame, tr("Speed (rts)"), GROUPBOX_NORMAL|FRAME_GROOVE|LAYOUT_FILL_X);

   // spinner control which is used to set speed
   speedcontrol = new FXRealSpinner(groupbox1, 3, &dt_rts, FXDataTarget::ID_VALUE,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);
   speedcontrol->setRange(0.1, 3.0);
   speedcontrol->setIncrement(0.1);

   // group box which will contain radio buttons to set sampling
   FXGroupBox* groupbox2 = new FXGroupBox(buttonFrame, tr("Multi Sampling"), GROUPBOX_NORMAL|FRAME_GROOVE|LAYOUT_FILL_X);
   new FXRadioButton(groupbox2, "Off", this, ID_MULTISAMPLE_OFF);
   new FXRadioButton(groupbox2, "2x", this, ID_MULTISAMPLE_2X);
   new FXRadioButton(groupbox2, "4x", this, ID_MULTISAMPLE_4X);

   // button to exit application
   new FXButton(buttonFrame, tr("&Exit\tExit the application"), nullptr, getApp(),
                FXApp::ID_QUIT,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_BOTTOM|LAYOUT_LEFT, 0,0,0,0,10,10,5,5);

   // left vertical frame that will contain an opengl canvas
   FXVerticalFrame* glcanvasFrame = new FXVerticalFrame(this, LAYOUT_FILL_X|LAYOUT_FILL_Y, 0,0,0,0, 2,2,3,3);

   // subframe within drawing glcanvas
   FXComposite* glpanelFrame = new FXVerticalFrame(glcanvasFrame, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,
                                                   0,0,0,0, 0,0,0,0);

   // a visual that contains pixel information for opengl drawing
   glvisual = new FXGLVisual(getApp(), VISUAL_DOUBLE_BUFFER);

   // defines the area to be drawn by another object, i.e., opengl code
   glcanvas = new FXGLCanvas(glpanelFrame, glvisual, this, ID_CANVAS, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);

   // enables tooltips
   new FXToolTip(getApp());

   spinning = false;
   angle = 0.0;
   rts = 1.0;
   dt_rts.connect(rts);
}

MainWindow::~MainWindow()
{
   getApp()->removeTimeout(this, ID_TIMEOUT);
   getApp()->removeChore(this, ID_CHORE);
   delete glvisual;
}

void MainWindow::create()
{
   FXMainWindow::create();
   show(PLACEMENT_SCREEN);
}

// widget has been resized
long MainWindow::onConfigure(FXObject*, FXSelector, void*)
{
   if ( glcanvas->makeCurrent() ) {
      glViewport(0, 0, glcanvas->getWidth(), glcanvas->getHeight());
      glcanvas->makeNonCurrent();
   }
   return 1;
}

// widget needs repainting
long MainWindow::onExpose(FXObject*, FXSelector, void*)
{
   drawScene();
   return 1;
}

// timer expired, rotate the boxes, draw and reset timer
long MainWindow::onTimeout(FXObject*, FXSelector, void*)
{
   angle += 2.0;
   if ( angle > 360.0 ) angle -= 360.0;
   lasttime = FXThread::time();
   drawScene();
   getApp()->addTimeout(this, ID_TIMEOUT, TIMER_INTERVAL);
   return 1;
}

// chore message received, rotate boxes, redraw, add new chore
long MainWindow::onChore(FXObject*, FXSelector, void*)
{
   FXTime c = FXThread::time();
   FXTime d = c - lasttime;
   angle += (d / 1000000000.0) * (360.0 * rts);
   if ( angle > 360.0 ) angle-=360.0;
   lasttime = c;
   drawScene();
   getApp()->addChore(this, ID_CHORE);
   return 1;
}

// start spinning, add timer to animate
long MainWindow::onCmdSpin(FXObject*, FXSelector, void*)
{
   spinning = true;
   getApp()->addTimeout(this, ID_TIMEOUT, TIMER_INTERVAL);
   return 1;
}

// enable or disable the spin button
long MainWindow::onUpdSpin(FXObject* sender, FXSelector, void*)
{
   FXButton* button = static_cast<FXButton*>(sender);
   spinning ? button->disable() : button->enable();
   return 1;
}

// start spinning boxes, add chore to keep spinning
long MainWindow::onCmdSpinFast(FXObject*, FXSelector, void*)
{
  spinning = true;
  lasttime = FXThread::time();
  speedcontrol->enable();
  getApp()->addChore(this, ID_CHORE);
  return 1;
}

// enable or disable the spin button
long MainWindow::onUpdSpinFast(FXObject* sender, FXSelector,void*)
{
   FXButton* button = static_cast<FXButton*>(sender);
   spinning ? button->disable() : button->enable();
   return 1;
}

// if boxes are spinning, stop them
long MainWindow::onCmdStop(FXObject*, FXSelector, void*)
{
   getApp()->removeTimeout(this, ID_TIMEOUT);
   getApp()->removeChore(this, ID_CHORE);
   speedcontrol->disable();
   spinning = false;
   return 1;
}

// enable or disable the stop button
long MainWindow::onUpdStop(FXObject* sender, FXSelector,void*)
{
   FXButton* button = static_cast<FXButton*>(sender);
   spinning ? button->enable() : button->disable();
   return 1;
}

// enable or disable the stop button
long MainWindow::onUpdSpeed(FXObject* sender, FXSelector,void*)
{
   if (getApp()->hasTimeout(this, ID_TIMEOUT))
      sender->handle(this, FXSEL(SEL_COMMAND, ID_DISABLE), nullptr);
   else
      sender->handle(this, FXSEL(SEL_COMMAND, ID_ENABLE), nullptr);
   return 1;
}

// draws a simple box using the given corners
void drawBox(GLfloat xmin, GLfloat ymin, GLfloat zmin, GLfloat xmax, GLfloat ymax, GLfloat zmax)
{
   glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(0.0, 0.0, -1.0);
      glVertex3f(xmin, ymin, zmin);
      glVertex3f(xmin, ymax, zmin);
      glVertex3f(xmax, ymin, zmin);
      glVertex3f(xmax, ymax, zmin);
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(xmax, ymin, zmin);
      glVertex3f(xmax, ymax, zmin);
      glVertex3f(xmax, ymin, zmax);
      glVertex3f(xmax, ymax, zmax);
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(xmax, ymin, zmax);
      glVertex3f(xmax, ymax, zmax);
      glVertex3f(xmin, ymin, zmax);
      glVertex3f(xmin, ymax, zmax);
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(xmin, ymin, zmax);
      glVertex3f(xmin, ymax, zmax);
      glVertex3f(xmin, ymin, zmin);
      glVertex3f(xmin, ymax, zmin);
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(0.0,1.0, 0.0);
      glVertex3f(xmin, ymax, zmin);
      glVertex3f(xmin, ymax, zmax);
      glVertex3f(xmax, ymax, zmin);
      glVertex3f(xmax, ymax, zmax);
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(xmax, ymin, zmax);
      glVertex3f(xmax, ymin, zmin);
      glVertex3f(xmin, ymin, zmax);
      glVertex3f(xmin, ymin, zmin);
   glEnd();
}

// draw the scene
void MainWindow::drawScene()
{
   const GLfloat lightPosition[] = { 15.0, 10.0, 5.0, 1.0 };
   const GLfloat lightAmbient[]  = { 0.1f, 0.1f, 0.1f, 1.0 };
   const GLfloat lightDiffuse[]  = { 0.9f, 0.9f, 0.9f, 1.0 };
   const GLfloat redMaterial[]   = { 1.0, 0.0, 0.0, 1.0 };
   const GLfloat blueMaterial[]  = { 0.0, 0.0, 1.0, 1.0 };

   const GLdouble canvaswidth = glcanvas->getWidth();
   const GLdouble canvasheight = glcanvas->getHeight();
   const GLdouble aspect = canvasheight>0 ? canvaswidth/canvasheight : 1.0;

   // make context current
   if (glcanvas->makeCurrent()) {

      glViewport(0, 0, glcanvas->getWidth(), glcanvas->getHeight());

      glClearColor(1.0, 1.0, 1.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glEnable(GL_DEPTH_TEST);

      glDisable(GL_DITHER);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(30.0, aspect, 1.0, 100.0);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(5.0, 10.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

      glShadeModel(GL_SMOOTH);
      glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
      glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
      glEnable(GL_LIGHT0);
      glEnable(GL_LIGHTING);

      glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, blueMaterial);

      glPushMatrix();
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-1, -1, -1, 1, 1, 1);

      glMaterialfv(GL_FRONT, GL_AMBIENT, redMaterial);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, redMaterial);

      glPushMatrix();
      glTranslated(0.0, 1.75, 0.0);
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
      glPopMatrix();

      glPushMatrix();
      glTranslated(0.0, -1.75, 0.0);
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
      glPopMatrix();

      glPushMatrix();
      glRotated(90.0, 1.0, 0.0, 0.0);
      glTranslated(0.0, 1.75, 0.0);
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
      glPopMatrix();

      glPushMatrix();
      glRotated(90.0, -1.0, 0.0, 0.0);
      glTranslated(0.0, 1.75, 0.0);
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
      glPopMatrix();

      glPushMatrix();
      glRotated(90.0, 0.0, 0.0, 1.0);
      glTranslated(0.0, 1.75, 0.0);
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
      glPopMatrix();

      glPushMatrix();
      glRotated(90.0, 0.0, 0.0, -1.0);
      glTranslated(0.0, 1.75, 0.0);
      glRotated(angle, 0.0, 1.0, 0.0);
      drawBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
      glPopMatrix();

      glPopMatrix();

      // swap if it is double-buffered
      if ( glvisual->isDoubleBuffer() ) {
         glcanvas->swapBuffers();
      }

      // make context non-current
      glcanvas->makeNonCurrent();
   }
}

// opens a dialog showing OpenGL properties
long MainWindow::onCmdOpenGL(FXObject*, FXSelector, void*)
{
   SettingsDialog sd(this, glcanvas);
   sd.execute();
   return 1;
}

// switch multisampling on
long MainWindow::onCmdMultiSample(FXObject* sender, FXSelector sel, void*)
{
   FXint nsamples = 0;
   switch( FXSELID(sel) ) {
      case ID_MULTISAMPLE_OFF: nsamples = 0; break;
      case ID_MULTISAMPLE_2X : nsamples = 2; break;
      case ID_MULTISAMPLE_4X : nsamples = 4; break;
   }
   glcanvas->destroy();
   glvisual->destroy();
   glvisual->setMultiSamples(nsamples);
   glvisual->create();
   if ( glvisual->getActualMultiSamples() != nsamples ) {
      sender->handle(this, FXSEL(SEL_COMMAND, ID_DISABLE), nullptr);
   }
   glcanvas->create();
   return 1;
}

// update multisampling radio buttons
long MainWindow::onUpdMultiSample(FXObject* sender, FXSelector sel, void*)
{
   FXbool check = false;
   switch(FXSELID(sel)) {
      case ID_MULTISAMPLE_OFF: if ( glvisual->getActualMultiSamples() != 2 && glvisual->getActualMultiSamples() != 4 ) check = true; break;
      case ID_MULTISAMPLE_2X : if ( glvisual->getActualMultiSamples() == 2 ) check=true; break;
      case ID_MULTISAMPLE_4X : if ( glvisual->getActualMultiSamples() == 4 ) check=true; break;
   }
   if (check)
      sender->handle(this, FXSEL(SEL_COMMAND, ID_CHECK), nullptr);
   else
      sender->handle(this, FXSEL(SEL_COMMAND, ID_UNCHECK), nullptr);
   return 1;
}
