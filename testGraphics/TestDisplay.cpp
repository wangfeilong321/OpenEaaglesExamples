
#include "TestDisplay.h"

#include "openeaagles/graphics/Graphic.h"
#include "openeaagles/graphics/Image.h"

#include <GL/glut.h>

using namespace oe;

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestDisplay, "TestDisplay")
EMPTY_SERIALIZER(TestDisplay)

TestDisplay::TestDisplay()
{
   STANDARD_CONSTRUCTOR()
   selected = nullptr;
}

EMPTY_COPYDATA(TestDisplay)
EMPTY_DELETEDATA(TestDisplay)

// Event() map
BEGIN_EVENT_HANDLER(TestDisplay)
   ON_EVENT('f',onFrameBufferKey)
   ON_EVENT('F',onFrameBufferKey)
END_EVENT_HANDLER()

void TestDisplay::drawIt()
{
   BaseClass::drawIt();
   //clear();
   //draw();
   //swapBuffers();
}

// mouseEvent() -- Handle mouse inputs
void TestDisplay::mouseEvent(const int button, const int state, const int x, const int y)
{
   setMouse(x,y);
   if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
      if (selected != nullptr) selected->setFlashRate(0.0);
      selected = pick(0);
      if (selected != nullptr) selected->setFlashRate(2.0f);
   }
}

// reset simulation
bool TestDisplay::onFrameBufferKey()
{
   graphics::Image* image = readFrameBuffer();
   image->writeFileBMP("./test.bmp");
   return true;
}
