
#ifndef __TestOilPressure_H__
#define __TestOilPressure_H__

#include "openeaagles/graphics/Page.h"

//------------------------------------------------------------------------------
// Class: TestOilPressure
//
// Description: Another example of using an analog dial.  This one is an F-16
// like oil pressure gauge
//------------------------------------------------------------------------------
class TestOilPressure : public oe::graphics::Page
{
   DECLARE_SUBCLASS(TestOilPressure, oe::graphics::Page)

public:
    TestOilPressure();

    virtual void updateData(const double dt = 0) override;

private:
    double oilPressure;
    SendData oilPressureSD;
    SendData oilPressureROSD;       // sends our readout data (to a numeric readout)
    double  oilRate;
};

#endif
