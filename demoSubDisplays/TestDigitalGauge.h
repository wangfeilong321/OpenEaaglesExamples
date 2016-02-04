//------------------------------------------------------------------------------
// Class: TestDigitalGauge
//
// Description: Example Digital AOA Gauge Page
// This page will show an analog F16-like aoa gauge beside a digital aoa gauge
//
//------------------------------------------------------------------------------
#ifndef __oe_demo_TestDigitalGauge_H__
#define __oe_demo_TestDigitalGauge_H__

#include "openeaagles/graphics/Page.h"

namespace oe {
namespace demo {

class TestDigitalGauge : public BasicGL::Page
{
   DECLARE_SUBCLASS(TestDigitalGauge, BasicGL::Page)

public:
   TestDigitalGauge();

   void updateData(const LCreal dt = 0) override;

private:
    LCreal aoa;
    SendData aoaSD;
    LCreal  aoaRate;        // rate which are going (up or down)
    SendData aoaROSD;       // aoa readout
    SendData aoaASD;        // analog readout
};

} // end of Instruments namespace
} // end of oe namespace

#endif

