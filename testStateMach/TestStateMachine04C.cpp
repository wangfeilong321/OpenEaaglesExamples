
#include "TestStateMachine04C.h"
#include "TestStateMachine04.h"

#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Boolean.h"

using namespace oe;

IMPLEMENT_SUBCLASS(TestStateMachine04C, "TestStateMachine04C")
EMPTY_SLOTTABLE(TestStateMachine04C)
EMPTY_CONSTRUCTOR(TestStateMachine04C)
EMPTY_COPYDATA(TestStateMachine04C)
EMPTY_DELETEDATA(TestStateMachine04C)
EMPTY_SERIALIZER(TestStateMachine04C)

// State dispatch table
BEGIN_STATE_TABLE(TestStateMachine04C)
   STATE_FUNC( INIT_STATE, stateFunc00)
END_STATE_TABLE()

//------------------------------------------------------------------------------
// Pre/Post functions
//------------------------------------------------------------------------------

void TestStateMachine04C::preStateProc(const double)
{
   // Start line
   std::cout << "SM(#4C)(" << getState() << "," << getSubstate() << "): ";
}

//------------------------------------------------------------------------------
// State functions
//------------------------------------------------------------------------------

void TestStateMachine04C::stateFunc00(const double)
{
   switch (getSubstate()) {

      case INIT_STATE : {
         std::cout << "nextSubstate()";
         nextSubstate();
         break;
      }

      case 1 : {
         if (getMode() != RTN_STATE) {
            std::cout << "callState(CALL_02,arg=13)";
            base::Number* arg = new base::Integer(13);
            callState(TestStateMachine04::CALL_02, arg);
            arg->unref();
         }
         else {
            std::cout << "Returned from call; ";
            const base::Boolean* arg = dynamic_cast<const base::Boolean*>( getArgument() );
            if (arg != nullptr) {
               std::cout << "arg(" << arg->getBoolean() << "); ";
            }
            std::cout << "nextSubstate()";
            nextSubstate();
         }
         break;
      }

      case 2 : {
         std::cout << "nextState()";
         nextState();
         break;
      }

   }
}
