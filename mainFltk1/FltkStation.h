
#ifndef __FltkStation_H__
#define __FltkStation_H__

#include "openeaagles/simulation/Station.h"

class MainWindow;
class FltkDisplay;

// ----------------------------------------------------------------------------
// Class: FltkStation
//
// Description: Station to start our fltk drawing
// NOTE: If you don't want to use the OpenEaagles simulation class, you create your
// own derived component to do this same functionality.  I am just
// using a Station so we can test interfacing and make sure our threads all
// still run correctly and work.
// ----------------------------------------------------------------------------
class FltkStation : public oe::simulation::Station
{
   DECLARE_SUBCLASS(FltkStation, oe::simulation::Station)

public:
   FltkStation();

   virtual void reset() override;
   virtual void updateData(const double dt = 0.0) override;

protected:
   void initData();

   bool setSlotDisplay1(FltkDisplay* const x);
   bool setSlotDisplay2(FltkDisplay* const x);

private:
   bool init;                  // initialized flag
   FltkDisplay* display1;      // first display
   FltkDisplay* display2;      // second display
   MainWindow* mainWin;        // main Fltk window
};

#endif
