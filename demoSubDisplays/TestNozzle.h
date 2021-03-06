
#ifndef __TestNozzle_H__
#define __TestNozzle_H__

#include "openeaagles/graphics/Page.h"

//------------------------------------------------------------------------------
// Class: TestNozzle
//
// Description: Another example of using an analog dial.
//------------------------------------------------------------------------------
class TestNozzle : public oe::graphics::Page
{
   DECLARE_SUBCLASS(TestNozzle, oe::graphics::Page)

public:
    TestNozzle();

    virtual void updateData(const double dt = 0) override;

private:
    double nozPos;
    SendData nozPosSD;
    SendData nozPosROSD;       // sends our readout data (to a numeric readout)
    double  nozRate;
};

#endif
