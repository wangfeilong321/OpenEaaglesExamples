
#include "TestDisplay.h"
#include "TestStation.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Aam.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/StoresMgr.h"
#include "openeaagles/graphics/SymbolLoader.h"
#include "openeaagles/base/Boolean.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/units/Distances.h"

IMPLEMENT_SUBCLASS(TestDisplay, "TestDisplay")
EMPTY_SLOTTABLE(TestDisplay)
EMPTY_SERIALIZER(TestDisplay)
EMPTY_DELETEDATA(TestDisplay)

// Event() map
BEGIN_EVENT_HANDLER(TestDisplay)
   ON_EVENT('r',onResetKey)        // Station Reset
   ON_EVENT('R',onResetKey)        // Station Reset
   ON_EVENT('f',onFreezeKey)       // Simulation Freeze
   ON_EVENT('F',onFreezeKey)       // Simulation Freeze
   ON_EVENT('l',onWpnRelKey)       // Launch
   ON_EVENT('L',onWpnRelKey)       // Launch
   ON_EVENT('p',onPreRelKey)       // Pre-launch
   ON_EVENT('P',onPreRelKey)       // Pre-launch
   ON_EVENT('i',onIncRngKey)       // Increase range
   ON_EVENT('I',onIncRngKey)       // Increase range
   ON_EVENT('d',onDecRngKey)       // Decrease range
   ON_EVENT('D',onDecRngKey)       // Decrease range
   ON_EVENT('s',onStepOwnshipKey)  // Step ownship
   ON_EVENT('S',onStepOwnshipKey)  // Step ownship
END_EVENT_HANDLER()

TestDisplay::TestDisplay() : myStation(nullptr)
{
   STANDARD_CONSTRUCTOR()

   for (unsigned int i = 0; i < MAX_TRACKS; i++) {
      tracks[i] = nullptr;
      trkIdx[i] = 0;
   }
   range = 40.0;
}

void TestDisplay::copyData(const TestDisplay& org, const bool)
{
   BaseClass::copyData(org);

   myStation = nullptr;
   for (int i = 0; i < MAX_TRACKS; i++) {
      tracks[i] = nullptr;
      trkIdx[i] = 0;
   }

   range = org.range;
}


//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// reset simulation
bool TestDisplay::onResetKey()
{
   if ( getSimulation() != nullptr ) {
      getSimulation()->event(RESET_EVENT);
   }
   return true;
}

// freeze simulation
bool TestDisplay::onFreezeKey()
{
   if ( getSimulation() != nullptr ) {
      oe::base::Boolean newFrz( !getSimulation()->isFrozen() );
      getSimulation()->event(FREEZE_EVENT, &newFrz);
   }
   return true;
}


// Weapon Release Switch
bool TestDisplay::onWpnRelKey()
{
   if (getOwnship() != nullptr) {
      oe::simulation::StoresMgr* sms = getOwnship()->getStoresManagement();
      if (sms != nullptr) {
         sms->setWeaponDeliveryMode(oe::simulation::StoresMgr::A2A);
         getOwnship()->event(WPN_REL_EVENT);
      }
   }
   return true;
}

// Pre-Release Switch
bool TestDisplay::onPreRelKey()
{
    if (getOwnship() != nullptr) {
       oe::simulation::StoresMgr* sms = getOwnship()->getStoresManagement();
        if (sms != nullptr) {
            sms->setWeaponDeliveryMode(oe::simulation::StoresMgr::A2A);
            oe::simulation::Weapon* wpn = sms->getCurrentWeapon();
            if (wpn != nullptr) {
               wpn->prerelease();
               std::cout << "Prelaunched wpn = " << wpn << std::endl;
            }
        }
    }
    return true;
}

// Increase range key
bool TestDisplay::onIncRngKey()
{
   double rng = range * 2.0;
   if (rng < 200) range =  rng;
   return true;
}

// Decrease range key
bool TestDisplay::onDecRngKey()
{
   double rng = range / 2.0;
   if (rng >= 2.0) range = rng;
   return true;
}

// Step ownship key
bool TestDisplay::onStepOwnshipKey()
{
   TestStation* ts = dynamic_cast<TestStation*>(getStation());
   if ( ts != nullptr ) {
      ts->stepOwnshipPlayer();
   }
   return true;
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void TestDisplay::updateData(const double dt)
{
    // Send flight data to readouts
    if (getOwnship() != nullptr) {

       send("hsi", UPDATE_VALUE5, getOwnship()->getHeadingR(), headingSD);
       send("rangeRO", UPDATE_VALUE, range, rangeSD);

       // Maintain Air Tracks
       oe::base::Pair* pair = findByName("airTracks");
       if (pair != nullptr) {
          oe::graphics::SymbolLoader* myLoader = dynamic_cast<oe::graphics::SymbolLoader*>(pair->object());
          if (myLoader != nullptr) {
             myLoader->setRange(range);
             myLoader->setHeadingDeg(getOwnship()->getHeadingD());
             myLoader->setNorthUp(false);

             maintainAirTrackSymbols(myLoader, range);
          }
       }
    }

    // Update base classes stuff
    BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// maintainAirTrackSymbols() -- maintain the air track symbology
//------------------------------------------------------------------------------
void TestDisplay::maintainAirTrackSymbols(oe::graphics::SymbolLoader* loader, const double rng)
{
   int codes[MAX_TRACKS];              // Work codes: empty(0), matched(1), unmatched(-1)
   double rng2 = (rng * rng);          // Range squared (KM * KM)

   oe::simulation::Player* newTracks[MAX_TRACKS];  // New tracks to add
   int nNewTracks = 0;                             // Number of new tracks
   oe::simulation::Player* target = nullptr;

   // The real maximum number of tracks is the smaller of MAX_TRACKS and the loader's maximum
   int maxTracks = loader->getMaxSymbols();
   if (MAX_TRACKS < maxTracks) maxTracks = MAX_TRACKS;

   // Set the initial codes
   for (int i = 0; i < maxTracks; i++) {
      if (tracks[i] != nullptr) codes[i] = -1;  // needs to be matched
      else codes[i] = 0;                        // empty slot
   }

   // find all air vehicles within range
   {
      // get the player list
      oe::simulation::Simulation* sim = getSimulation();
      oe::base::PairStream* plist = sim->getPlayers();

      // search for air vehicles or missiles within range
      oe::base::List::Item* item = plist->getFirstItem();
      while (item != nullptr && nNewTracks < maxTracks) {
         oe::base::Pair* pair = static_cast<oe::base::Pair*>(item->getValue());
         oe::simulation::Player* p = static_cast<oe::simulation::Player*>(pair->object());
         oe::osg::Vec3 rpos = p->getPosition() - getOwnship()->getPosition();
         double x = rpos[0] * oe::base::Distance::M2NM;
         double y = rpos[1] * oe::base::Distance::M2NM;

         oe::simulation::Weapon* weapon = dynamic_cast<oe::simulation::Weapon*>(p);
         if (weapon && (weapon->isMode(oe::simulation::Player::PRE_RELEASE) || weapon->isActive())) {
            target = weapon->getTargetPlayer();
         }

         if (
            p != getOwnship() &&
            p->isActive() &&
            ((x*x + y*y) < rng2) &&
            (p->isClassType(typeid(oe::simulation::AirVehicle)) || p->isClassType(typeid(oe::simulation::Missile))) ) {
               // Ok, it's an active air vehicle or missile that's within range, and it's not us.

               // Are we already in the track list?
               bool found = false;
               for (int i = 0; !found && i < maxTracks; i++) {
                  if (p == tracks[i]) {
                     // Yes it is.  So mark the slot as matched!
                     codes[i] = 1;
                     found = true;
                  }
               }

               // If not found then add it to the new tracks list
               if (!found) {
                  p->ref();
                  newTracks[nNewTracks++] = p;
               }

         }
         item = item->getNext();
      }

      plist->unref();
   }


   // Now remove any unmatched tracks
   for (int i = 0; i < maxTracks; i++) {
      if (codes[i] == -1) {
         // This is an old symbol that wasn't matched with a valid player
         loader->removeSymbol( trkIdx[i] );
         tracks[i]->unref();       // unref the player
         tracks[i] = nullptr;      // clear the player pointer
         trkIdx[i] = 0;            // clear the index
         codes[i]  = 0;            // slot is now empty
      }
   }

   // Now add any new tracks
   {
      int islot = 0;      // slot index
      int inew = 0;       // new track index

      while (inew < nNewTracks && islot < maxTracks) {
         if (codes[islot] == 0) {
            // We have an empty slot, so add the symbol

            int type = 4;                                                            // unknown
            if (newTracks[inew]->isClassType(typeid(oe::simulation::AirVehicle))) {
               if (newTracks[inew]->isSide(oe::simulation::Player::BLUE)) type = 1;      // friend
               else if (newTracks[inew]->isSide(oe::simulation::Player::RED)) type = 2;  // foe
               else type = 3; // neutral/commercial
            }
            else if (newTracks[inew]->isClassType(typeid(oe::simulation::Missile))) {
               type = 5;                                                             // missile
            }

            tracks[islot] = newTracks[inew];
            trkIdx[islot] = loader->addSymbol( type, nullptr);
            if (trkIdx[islot] == 0) {
               // it didn't make it in for some unknown reason
               tracks[islot]->unref();
               tracks[islot] = nullptr;
            }
            inew++;

         }
         islot++;
      }

      // unref any air vehicles that didn't make it
      while (inew < nNewTracks) {
         newTracks[inew++]->unref();
      }
   }

   // now update the active tracks
   for (int i = 0; i < maxTracks; i++) {
      double osX = getOwnship()->getXPosition();
      double osY = getOwnship()->getYPosition();
      if (tracks[i] != nullptr && trkIdx[i] != 0) {
         double xp = tracks[i]->getXPosition() - osX;
         double yp = tracks[i]->getYPosition() - osY;
         loader->updateSymbolPositionXY( trkIdx[i], (xp * oe::base::Distance::M2NM), (yp * oe::base::Distance::M2NM) );
         loader->updateSymbolHeading( trkIdx[i], tracks[i]->getHeadingD() );
         if (tracks[i]==target) {
            //base::Identifier* temp = new base::Identifier("green");
            //loader->changeSymbolColor(trkIdx[i], 0, temp);
            loader->setSymbolFlashRate(trkIdx[i], nullptr, 2);
         }
      }
   }
}

//------------------------------------------------------------------------------
// Simulation access functions
//------------------------------------------------------------------------------
oe::simulation::Player* TestDisplay::getOwnship()
{
   oe::simulation::Player* p = nullptr;
   oe::simulation::Station* sta = getStation();
   if (sta != nullptr) p = sta->getOwnship();
   return p;
}

oe::simulation::Simulation* TestDisplay::getSimulation()
{
   oe::simulation::Simulation* s = nullptr;
   oe::simulation::Station* sta = getStation();
   if (sta != nullptr) s = sta->getSimulation();
   return s;
}

oe::simulation::Station* TestDisplay::getStation()
{
   if (myStation == nullptr) {
      oe::simulation::Station* s = dynamic_cast<oe::simulation::Station*>( findContainerByType(typeid(oe::simulation::Station)) );
      if (s != nullptr) myStation = s;
   }
   return myStation;
}
