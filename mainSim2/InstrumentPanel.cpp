
#include "InstrumentPanel.h"
#include "SimStation.h"
#include "SimPlayer.h"

#include "openeaagles/instruments/eadi3D/Eadi3DPage.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/Boolean.h"
#include <GL/glut.h>

IMPLEMENT_SUBCLASS(InstrumentPanel, "InstrumentPanel")
EMPTY_SERIALIZER(InstrumentPanel)
EMPTY_SLOTTABLE(InstrumentPanel)
EMPTY_DELETEDATA(InstrumentPanel)

// Event() map
BEGIN_EVENT_HANDLER(InstrumentPanel)
   ON_EVENT('r',onResetKey)
   ON_EVENT('R',onResetKey)
   ON_EVENT('f',onFreezeKey)
   ON_EVENT('F',onFreezeKey)
   ON_EVENT('+',onStepOwnshipKey)
END_EVENT_HANDLER()

InstrumentPanel::InstrumentPanel()
{
   STANDARD_CONSTRUCTOR()
   myStation = nullptr;
}

void InstrumentPanel::copyData(const InstrumentPanel& org, const bool)
{
   BaseClass::copyData(org);
   myStation = nullptr;
}

oe::simulation::Player* InstrumentPanel::getOwnship()
{
   oe::simulation::Player* p = nullptr;
   oe::simulation::Station* sta = getStation();
   if (sta != nullptr) p = sta->getOwnship();
   return p;
}

oe::simulation::Simulation* InstrumentPanel::getSimulation()
{
   oe::simulation::Simulation* s = nullptr;
   oe::simulation::Station* sta = getStation();
   if (sta != nullptr) s = sta->getSimulation();
   return s;
}

oe::simulation::Station* InstrumentPanel::getStation()
{
   if (myStation == nullptr) {
      oe::simulation::Station* s = dynamic_cast<oe::simulation::Station*>( findContainerByType(typeid(oe::simulation::Station)) );
      if (s != nullptr) myStation = s;
   }
   return myStation;
}

void InstrumentPanel::updateData(const double dt)
{
   // update our base class first
   BaseClass::updateData(dt);

   // try to get an Sim3 first.  If that doesn't work, then get a generic air vehicle
   // Get the data from our ownship, if we have a valid one.  Else everything goes to a default value
   // we need to dynamically cast to an AirVehicle* for this instrument panel
   oe::simulation::AirVehicle* tempOwnship = dynamic_cast<oe::simulation::AirVehicle*>( getOwnship() );
   if (tempOwnship != nullptr) {
      tempOwnship->ref();
#if 0
      sBrakePos = tempOwnship->getSpeedBrakePosition();
#endif
      gForce = tempOwnship->getGload();
      aoa = tempOwnship->getAngleOfAttackD();
      airSpeed = tempOwnship->getCalibratedAirspeed();
      heading = tempOwnship->getHeadingD();
      altitude = tempOwnship->getAltitudeFt();
      vvi = tempOwnship->getVelocity();
      slip = tempOwnship->getSideSlipD();
      pitch = tempOwnship->getPitchD();
      roll = tempOwnship->getRollD();
      mach = tempOwnship->getMach();
      gload = tempOwnship->getGload();

      tempOwnship->unref();
   }
   else {
      SimPlayer* player = dynamic_cast<SimPlayer*>( getOwnship() );
      if (player != nullptr) {
         player->ref();

#if 0
         course = static_cast<double>(player->getCourse());
         sBrakePos = player->getSpeedBrakePosition();
#endif
         gForce = player->getGload();
         aoa = player->getAngleOfAttackD();
         airSpeed = player->getCalibratedAirspeed();
         heading = player->getHeadingD();
         altitude = player->getAltitudeFt();
         vvi = player->getVelocity();
         slip = player->getSideSlipD();
         pitch = player->getPitchD();
         roll = player->getRollD();
         mach = player->getMach();
         gload = 1.0;

         player->unref();
      }
      else {
#if 0
         sBrakePos = 0;
         course = 0;
#endif
         gForce = 1.0;
         aoa = 0;
         dme = 0;
         airSpeed = 0;
         heading = 0;
         altitude = 0;
         vvi.set(0,0,0);
         bpAngle = 0;
      }
   }

   oe::base::Pair* a = findSubpageByType(typeid(oe::instruments::Eadi3DPage));
   if (a != nullptr) {
      oe::instruments::Eadi3DPage* eadi = dynamic_cast<oe::instruments::Eadi3DPage*>(a->object());
      if (eadi != nullptr) {
         eadi->setAltitude(altitude);
         eadi->setAirspeed(airSpeed);
         eadi->setHeading(heading);
         eadi->setAOA(aoa);
         eadi->setVVI(-vvi.z() * oe::base::Distance::M2FT * 60.0);
         eadi->setPitch(pitch);
         eadi->setRoll(roll);
         eadi->setMach(mach);
         eadi->setGLoad(gload);
         eadi->setLandingMode(false);
         eadi->setPitchSteeringCmd(0.0);
         eadi->setRollSteeringCmd(0.0);
         eadi->setPitchSteeringValid(false);
         eadi->setRollSteeringValid(false);
         eadi->setGlideslopeDev(0.0);
         eadi->setLocalizerDev(0.0);
         eadi->setTurnRate(0.0);
         eadi->setSlipInd(0.0);
         eadi->setGlideslopeValid(false);
         eadi->setLocalizerValid(false);
      }
   }
}

//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// reset simulation
bool InstrumentPanel::onResetKey()
{
   if ( getSimulation() != nullptr ) {
      getSimulation()->event(RESET_EVENT);
   }
   return true;
}

// freeze simulation
bool InstrumentPanel::onFreezeKey()
{
   if ( getSimulation() != nullptr ) {
      oe::base::Boolean newFrz( !getSimulation()->isFrozen() );
      getSimulation()->event(FREEZE_EVENT, &newFrz);
   }
   return true;
}

// Step ownship key
bool InstrumentPanel::onStepOwnshipKey()
{
   SimStation* ts = dynamic_cast<SimStation*>(getStation());
   if ( ts != nullptr ) {
      ts->stepOwnshipPlayer();
   }
   return true;
}
