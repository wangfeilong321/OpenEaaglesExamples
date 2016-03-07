#include "TestHsi.h"

namespace oe {
namespace demo  {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TestHsi, "TestHsi")
EMPTY_SERIALIZER(TestHsi)
EMPTY_DELETEDATA(TestHsi)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TestHsi::TestHsi()
{
    STANDARD_CONSTRUCTOR()
    heading = 0.0;
    headingSD.empty();
    headingRate = 20.0;
    headingROSD.empty();
    dme = 0.0;
    dmeROSD.empty();
}

//------------------------------------------------------------------------------
// copyData()
//------------------------------------------------------------------------------
void TestHsi::copyData(const TestHsi& org, const bool)
{
    BaseClass::copyData(org);
    heading = org.heading;
    headingSD.empty();
    headingRate = org.headingRate;
    headingROSD.empty();
    dme = org.dme;
    dmeROSD.empty();
}

//------------------------------------------------------------------------------
// updateData() -- update non time-critical stuff here
//------------------------------------------------------------------------------
void TestHsi::updateData(const double dt)
{
    BaseClass::updateData(dt);

    heading += (headingRate * dt);
    if (heading > 360) {
        heading = 360;
        headingRate = -headingRate;
    }
    if (heading < 0) {
        heading = 0;
        headingRate = -headingRate;
    }

    dme += dt;
    if (dme > 100) dme = 0;

    send("hsi", UPDATE_VALUE6, heading, headingSD);
    send("hsiRO", UPDATE_VALUE, heading, headingROSD);
    send("dmeRO", UPDATE_VALUE, dme, dmeROSD);
}

} // end of demo  namespace
} // end of oe namespace

