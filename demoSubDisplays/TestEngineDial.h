
#ifndef __TestEngineDial_H__
#define __TestEngineDial_H__

#include "openeaagles/graphics/Page.h"

//------------------------------------------------------------------------------
// Class: TestEngineDial
//
// Description: Example Analog Dial Page
// This page will supply test data to the engine dial
//------------------------------------------------------------------------------
class TestEngineDial : public oe::graphics::Page
{
   DECLARE_SUBCLASS(TestEngineDial, oe::graphics::Page)

public:
    TestEngineDial();

    virtual void updateData(const double dt = 0) override;

private:
    double engineRotation;          // our engine rpm position (1 - 100)
    SendData engineRotationSD;
    SendData engineReadoutSD;       // sends our readout data (to a numeric readout)
    double  engineRate;             // rate which are going (up or down)
};

#endif
