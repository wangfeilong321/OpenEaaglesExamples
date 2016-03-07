//------------------------------------------------------------------------------
// Class: TdElevPtr
//------------------------------------------------------------------------------
#ifndef __oe_test_TdElevPtr_H__
#define __oe_test_TdElevPtr_H__

#include "openeaagles/graphics/Graphic.h"

//------------------------------------------------------------------------------
// Class: TdElevPtr
// Description: Display an elevation pointer.  Before drawing, we position the
//              pointer based our received elevation angle.  The shape of the
//              pointer is defined by our graphic subcomponents.
//
// Notes:
//    1) Elevation is in degrees.
//    2) Movement is limited to +/- 30 degrees, which is scaled to +/- 1.0 in
//       world coordinates.
//
// Factory name: TdElevPtr
// Events:
//    UPDATE_VALUE  <Number>  ! elevation angle (degs)
//------------------------------------------------------------------------------
namespace oe {
   namespace base { class Number; }
namespace test {

class TdElevPtr : public graphics::Graphic
{
   DECLARE_SUBCLASS(TdElevPtr,graphics::Graphic)

public:
   TdElevPtr();

   double getElevation() const;     // degs
   bool setElevation(const double); // degs

   void draw() override;

   bool event(const int event, base::Object* const obj = nullptr) override;

private:
   bool onUpdateValue(const base::Number* const);

   double elev;  // (degs)
};

} // End test namespace
} // End oe namespace

#endif
