//------------------------------------------------------------------------------
// Class: TestPfd
//
// Description: Tests another pfd page
//------------------------------------------------------------------------------
#ifndef __oe_demo_TestPfd_H__
#define __oe_demo_TestPfd_H__

#include "openeaagles/graphics/Page.h"

namespace oe {
namespace demo {

class TestPfd : public BasicGL::Page
{
   DECLARE_SUBCLASS(TestPfd, BasicGL::Page )

public:
    TestPfd();

    void updateData(const LCreal dt = 0.0) override;

private:
    // pitch and roll
    LCreal pitch;           // degs
    LCreal pitchRate;       // degs/sec
    LCreal roll;            // degs
    LCreal rollRate;        // degs/sec

    // heading and nav stuff
    LCreal trueHdg;         // degs
    LCreal tHdgRate;        // degs/sec
    LCreal cmdHdg;          // commanded heading (heading bug) (degs)
    LCreal cmdHdgRate;      // degs/sec

    // airspeed
    LCreal airSpd;
    LCreal airSpdRate;

    // altitude
    LCreal alt;
    LCreal altRate;

    // side slip
    LCreal slip;            // degs
    LCreal slipRate;        // degs/sec

    // glide slope (in dots)
    LCreal gSlope;
    LCreal gSlopeRate;

    // Lateral dev
    LCreal latDev;
    LCreal ldRate;

    // commanded speed
    LCreal cmdSpd;

    // commanded alt
    LCreal cmdAlt;

    // vvi
    LCreal vvi;
    LCreal vviRate;

    // flight director stuff (in inches)
    LCreal fDirBank;
    LCreal fDirBankRate;
    LCreal fDirPitch;
    LCreal fDirPitchRate;

    // barometric pressure
    LCreal baro;
    LCreal baroRate;
};

} // end of demo namespace
} // end of oe namespace

#endif

