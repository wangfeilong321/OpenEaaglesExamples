
#ifndef __AdiDisplay_H__
#define __AdiDisplay_H__

#include "openeaagles/gui/glut/GlutDisplay.h"

namespace oe {
   namespace simulation { class Station; class Aircraft; }
}

//------------------------------------------------------------------------------
// Class: AdiDisplay
//------------------------------------------------------------------------------
class AdiDisplay : public oe::glut::GlutDisplay
{
   DECLARE_SUBCLASS(AdiDisplay, oe::glut::GlutDisplay)

public:
   AdiDisplay();

   oe::simulation::Aircraft* getOwnship();
   oe::simulation::Station* getStation();

   virtual void updateData(const double dt = 0.0) override;

private:
   oe::base::safe_ptr<oe::simulation::Station> myStation;

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

#endif
