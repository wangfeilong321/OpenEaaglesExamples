
#ifndef __TestAdi2_H__
#define __TestAdi2_H__

#include "openeaagles/graphics/Page.h"

//------------------------------------------------------------------------------
// Class: TestAdi2
//
// Description: Tests an MFD version of the ADI
//------------------------------------------------------------------------------
class TestAdi2 : public oe::graphics::Page
{
   DECLARE_SUBCLASS(TestAdi2, oe::graphics::Page)

public:
    TestAdi2();

    virtual void updateData(const double dt = 0) override;

private:
    double pitch;           // our pitch (90 to -90)
    SendData pitchSD;
    double  pitchRate;
    double roll;            // our roll (180 to -180)
    SendData rollSD;
    SendData rollBASD;      // bank angle roll value
    double rollRate;
    double slip;            // our side slip (20 to -20)
    SendData slipSD;
    double slipRate;
    double calAS;           // calibrated airspeed
    double calASRate;
    SendData calASSD;
    double alt;             // altitude
    SendData altSD;
    double altRate;
    double mag;             // magnetic heading
    SendData magSD;
    double magRate;
    double cdmX;            // x (pitch) value of our climb dive marker (inches)
    double cdmY;            // y (slip) value of our climb dive marker (inches)
    SendData cdmXSD;
    SendData cdmYSD;
    SendData cdmRollSD;
    SendData ghostPitchSD;  // ghost horizon pitch
    SendData ghostRollSD;   // ghost horizon roll
};

#endif
