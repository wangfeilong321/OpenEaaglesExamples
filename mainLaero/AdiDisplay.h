//------------------------------------------------------------------------------
// Class: AdiDisplay
//------------------------------------------------------------------------------
#ifndef __oe_example_AdiDisplay_H__
#define __oe_example_AdiDisplay_H__

#include "openeaagles/gui/glut/GlutDisplay.h"

namespace oe {
   namespace simulation {
      class Station;
      class Aircraft;
   }

namespace example {

class AdiDisplay : public glut::GlutDisplay
{
   DECLARE_SUBCLASS(AdiDisplay, glut::GlutDisplay)

public:
   AdiDisplay();

   simulation::Aircraft* getOwnship();
   simulation::Station* getStation();

   void updateData(const double dt = 0.0) override;

private:
   base::safe_ptr<simulation::Station> myStation;

   //-----------------------------
   double psiRO;     // [deg]
   double thtRO;     // [deg]
   double phiRO;     // [deg]
   double velRO;     // [kts]
   double altRO;     // [ft]

   double pRO;       // [deg/sec]
   double qRO;       // [deg/sec]
   double rRO;       // [deg/sec]

   double bankADI;   // [deg]
   double pitchADI;  // [deg]

   //-----------------------------
   SendData psiRO_SD;
   SendData thtRO_SD;
   SendData phiRO_SD;
   SendData velRO_SD;
   SendData altRO_SD;

   SendData pRO_SD;
   SendData qRO_SD;
   SendData rRO_SD;

   SendData bankADI_SD;
   SendData pitchADI_SD;
};

}
}

#endif
