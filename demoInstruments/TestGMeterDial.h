
#ifndef __TestGMeterDial_H__
#define __TestGMeterDial_H__

#include "openeaagles/graphics/Page.h"

//------------------------------------------------------------------------------
// Class: TestGMeterDial
//
// Description: This page will test the G meter, which is a dial showing
// the min and max Gs as well as the current Gs being pulled.
//------------------------------------------------------------------------------
class TestGMeterDial : public oe::graphics::Page
{
   DECLARE_SUBCLASS(TestGMeterDial, oe::graphics::Page)

public:
    TestGMeterDial();

    virtual void updateData(const double dt = 0) override;

private:
    double currGs;      // current G's
    SendData currGsSD;
    SendData currGsROSD;
    double gRate;
    double timer;       // our timer for resetting the g meter
    double timerRate;
};

#endif
