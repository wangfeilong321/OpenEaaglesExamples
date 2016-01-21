
#include "MfdPage.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basicGL/Rotators.h"
#include "openeaagles/basic/PairStream.h"

namespace oe {
namespace example {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(MfdPage,"MfdPage")
EMPTY_SERIALIZER(MfdPage)

EMPTY_COPYDATA(MfdPage)
EMPTY_DELETEDATA(MfdPage)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
MfdPage::MfdPage()
{
   STANDARD_CONSTRUCTOR()
   rotate = 0;
   rotateRate = 5;
   rSD.empty();
}

//------------------------------------------------------------------------------
// Macro event handler for MfdPage
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(MfdPage)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// onEntry() -- default ON_ENTRY handler
//------------------------------------------------------------------------------
bool MfdPage::onEntry()
{
   Basic::PairStream* subcomponents = getComponents();
    if(subcomponents != nullptr) {
        Basic::List::Item* item = subcomponents->getFirstItem();
        while (item != nullptr) {
            Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
            Basic::Component* cp = static_cast<Basic::Component*>(pair->object());
            if (cp != nullptr) cp->event(RESET_EVENT);
            item = item->getNext();
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
    return true;
}

void MfdPage::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);
    
    rotate += (rotateRate * dt);
    if (rotate > 360) {
        rotate = 360;
        rotateRate = -rotateRate;
    }
    if (rotate < -360) {
        rotate = -360;
        rotateRate = -rotateRate;
    }
    
    send("rotator", UPDATE_VALUE2, rotate, rSD);
}

} // End example namespace
} // End oe namespace
