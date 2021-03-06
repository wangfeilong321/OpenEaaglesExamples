
#include "MapPage.h"
#include "TestStation.h"
#include "MapDisplay.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Autopilot.h"
#include "openeaagles/simulation/AirVehicle.h"

#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/Route.h"
#include "openeaagles/simulation/Steerpoint.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/graphics/SymbolLoader.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/models/dynamics/LaeroModel.h"
#include "openeaagles/graphics/Display.h"
#include "openeaagles/graphics/Shapes.h"

#include "openeaagles/base/units/Angles.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/units/Times.h"

#include <GL/glut.h>

namespace oe {
   namespace graphics { class OcclusionArc; }
}

using namespace oe;

IMPLEMENT_SUBCLASS(MapPage, "TestMapPage")
EMPTY_SLOTTABLE(MapPage)
EMPTY_SERIALIZER(MapPage)

MapPage::MapPage()
{
    STANDARD_CONSTRUCTOR()
    for (int i = 0; i < MAX_PLAYERS; i++) {
        player[i] = nullptr;
        playerIdx[i] = -1;
    }
    pStn = nullptr;
    loader = nullptr;

    for (int i = 0; i < MAX_READOUTS; i++) {
        latsSD[i].empty();
        lats[i] = 0;
        latReadoutXPosSD[i].empty();
        latReadoutXPos[i] = 0.0;
        latReadoutYPosSD[i].empty();
        latReadoutYPos[i] = 0.0;
        lonsSD[i].empty();
        lons[i] = 0;
        lonReadoutXPosSD[i].empty();
        lonReadoutXPos[i] = 0.0;
        lonReadoutYPosSD[i].empty();
        lonReadoutYPos[i] = 0.0;
    }

    routeLoaded = false;
}

void MapPage::copyData(const MapPage& org, const bool)
{
    // copy base class stuff first
    BaseClass::copyData(org);

    // regardless of copy, we will create all new symbols
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (player[i] != nullptr) {
            player[i]->unref();
            player[i] = nullptr;
        }
        if (org.player[i] != nullptr) {
            player[i] = org.player[i];
            player[i]->ref();
        }
        playerIdx[i] = org.playerIdx[i];
    }

    if (loader != nullptr) {
        loader->unref();
        loader = nullptr;
    }
    if (org.loader != nullptr) {
        loader = org.loader;
        loader->ref();
    }

    if (pStn != nullptr) {
        pStn->unref();
        pStn = nullptr;
    }
    if (org.pStn != nullptr) {
        pStn = org.pStn;
        pStn->ref();
    }

    for (int i = 0; i < MAX_READOUTS; i++) {
        latsSD[i].empty();
        lats[i] = org.lats[i];
        latReadoutXPosSD[i].empty();
        latReadoutXPos[i] = org.lonReadoutXPos[i];
        latReadoutYPosSD[i].empty();
        latReadoutYPos[i] = org.latReadoutYPos[i];
        lonsSD[i].empty();
        lons[i] = org.lons[i];
        lonReadoutXPosSD[i].empty();
        lonReadoutXPos[i] = org.lonReadoutXPos[i];
        lonReadoutYPosSD[i].empty();
        lonReadoutYPos[i] = org.lonReadoutYPos[i];
    }

    routeLoaded = org.routeLoaded;
}

void MapPage::deleteData()
{
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (player[i] != nullptr) {
            player[i]->unref();
            player[i] = nullptr;
        }
    }
    if (pStn != nullptr) {
        pStn->unref();
        pStn = nullptr;
    }
    if (loader != nullptr) {
        loader->unref();
        loader = nullptr;
    }
}

void MapPage::drawLine(const double x, const double y)
{
   glBegin(GL_LINES);
      glVertex2d(0.0, 0.0);
      glVertex2d(x, y);
   glEnd();
}

void MapPage::drawSemiCircle(const double startAngle, const double radius)
{
   const double eps = 0.015;
   const double innerRadius = radius - eps;
   const double outerRadius = radius + eps;
   GLUquadricObj* q1 = gluNewQuadric();
   gluQuadricDrawStyle(q1, GLU_FILL);
   gluPartialDisk(q1, innerRadius, outerRadius, 100, 1, startAngle, 180.0);
   gluDeleteQuadric(q1);
}

//void MapPage::drawHoldingPattern(const double aLat, const double aLon, const double ibCrs, const double tas)
//{
   //if (pStn != 0) {
   //   simulation::Player* pPlr  = pStn->getOwnship();
   //   if (pPlr != 0) {
   //      Vehicle::LaeroModel* pRac = (Vehicle::LaeroModel*) pPlr->getDynamicsModel();
   //      if (pRac != 0) {

   //         pRac->setAnchorLat(aLat);
   //         pRac->setAnchorLon(aLon);
   //         pRac->setInboundCourse(ibCrs);
   //         pRac->setAcftSpeed(tas);

   //         this->drawHoldingPattern();
   //      }
   //   }
   //}
//}


void MapPage::drawHoldingPattern()
{
   if (pStn != nullptr) {
      simulation::Player* pPlr  = pStn->getOwnship();
      if (pPlr != nullptr) {
         simulation::Autopilot* pRac = static_cast<simulation::Autopilot*>(pPlr->getPilot());
         if (pRac != nullptr) {

         //---------------------------------------------------------------------------
         // Draw Holding Pattern
         //---------------------------------------------------------------------------
            double aLat = 0, aLon = 0, mLat = 0, mLon = 0;
            pRac->getLoiterPointAnchors(&aLat, &aLon, &mLat, &mLon);
            //std::cout << "MIRROR LAT/LON = " << mLat << ", " << mLon << std::endl;
            const double ibCrs = pRac->getLoiterCourse();
            const double osVel = pPlr->getTotalVelocityKts();

            //setReferenceLatDeg(0.0);
            //setReferenceLonDeg(0.0);

//            double refLat = getReferenceLatDeg();
//            double refLon = getReferenceLonDeg();

            const double omegaDps = 3.0;                                  //dps
            const double omegaRps = omegaDps * base::Angle::D2RCC;       //rps
            const double rocNM = (osVel / base::Time::H2S) / omegaRps;   //nm
            //double obTimeMin = 2.0;                                     //min
            //double obTimeSec = obTimeMin * base::Time::M2S;            //sec

            //double obDistNM = (osVel / base::Time::H2S) * obTimeSec;   //nm

            double obDistNM = 0;
            if (pRac->isLoiterTimeBased()) {
               obDistNM = (osVel / base::Time::H2S) * pRac->getLoiterTime();   //nm
            }
            else {
               obDistNM = pRac->getLoiterPatternLengthNM();
            }

            double aLatSU = 0.0;
            double aLonSU = 0.0;
            latLon2Screen(aLat, aLon, &aLatSU, &aLonSU);

            const double rocSU = rocNM * getScale();
            const double obDistSU = obDistNM * getScale();

            //==============================================
            // begin drawing holding pattern

            glPushMatrix();

               // convert to NED coordinate system
               glRotated(90.0, 0, 0, 1);
               glRotated(180.0, 1, 0, 0);

               // set anchor waypoint and inbound course
               glTranslated(aLonSU, aLatSU, 0.0);
               glRotated(ibCrs, 0.0, 0.0, 1.0);

               // draw holding pattern
               //glColor3f(1, 1, 0);  // yellow
               drawLine(-obDistSU, 0.0);  // inbound path

               //glColor3f(1, 0, 0);  // red
               glTranslated(-obDistSU, rocSU, 0.0);
               drawSemiCircle(180.0, rocSU);  // inbound turn

               //glColor3f(1, 1, 0);  // yellow
               glTranslated(0.0, rocSU, 0.0);
               drawLine(obDistSU, 0.0);  // outbound path

               //glColor3f(0, 1, 0);  // green
               glTranslated(obDistSU, -rocSU, 0.0);
               drawSemiCircle(0.0, rocSU);  // outbound turn

            glPopMatrix();

            // end drawing holding pattern
            //==============================================
         }
      }
   }
}

void MapPage::drawFunc()
{
      //-------------------------------------------------------
      // get data pointers
      //-------------------------------------------------------
   if (pStn != nullptr) {
      simulation::Player* pPlr  = pStn->getOwnship();
      if (pPlr != nullptr) {

         // get the autopilot
         simulation::Autopilot* ap = static_cast<simulation::Autopilot*>(pPlr->getPilot());
         if (ap != nullptr && ap->isLoiterModeOn()) drawHoldingPattern();

         //---------------------------------------------------------------------------
         // get our viewport
         //---------------------------------------------------------------------------
         MapDisplay* pDsp = static_cast<MapDisplay*>(getDisplay());
         //graphics::Display* pDsp = (graphics::Display*) getDisplay();

         if (pDsp != nullptr) {
            //-----------------------------------------------------------------
            // get reference lat/lon
            //-----------------------------------------------------------------
            const double refLat = getReferenceLatDeg();
            const double refLon = getReferenceLonDeg();

            //-----------------------------------------------------------------
            // get lat/lon drawing limits
            //-----------------------------------------------------------------
            GLdouble l = 0, r = 0, t = 0, b = 0, n = 0, f = 0;
            pDsp->getOrtho(l, r, b, t, n, f);

            const double latRange = getRange() / 60.0;
            const double inchPerDegNS = t / latRange;
            const double northernLat = refLat + latRange;
            const double southernLat = refLat - latRange;

            const double lonRange = latRange / getCosRefLat();
            const double inchPerDegEW = r / lonRange;
            const double easternLon = refLon + lonRange;
            const double westernLon = refLon - lonRange;

            glPushMatrix();
            glBegin(GL_LINES);
               int latIdx = 0;
               int startLat = base::nint(static_cast<double>(southernLat) - 1);
               const int endLat = base::nint(static_cast<double>(northernLat) + 1);
               while (startLat < endLat) {
                  GLfloat refLatDist = static_cast<GLfloat>(refLat - startLat);
                  if (latIdx < MAX_READOUTS) {
                     lats[latIdx] = startLat;
                     refLatDist *= static_cast<GLfloat>(inchPerDegNS);
                     latReadoutYPos[latIdx] = static_cast<float>(-refLatDist + 0.2);
                     latReadoutXPos[latIdx++] = static_cast<float>(l + 0.5f);
                  }
                  glVertex2f(static_cast<GLfloat>(l), -refLatDist);
                  glVertex2f(static_cast<GLfloat>(r), -refLatDist);
                  startLat++;
               }
            glEnd();

            // now make the other lat lines go away
            for (int i = latIdx; i < MAX_READOUTS; i++) {
               latReadoutYPos[i] = -10000;
            }
            glPopMatrix();

            glPushMatrix();
            glBegin(GL_LINES);
               int lonIdx = 0;
               int startLon = base::nint(static_cast<double>(westernLon) - 1);
               const int endLon = base::nint(static_cast<double>(easternLon) + 1);
               while (startLon < endLon) {
                  GLfloat refLonDist = static_cast<GLfloat>(refLon - startLon);
                  if (lonIdx < MAX_READOUTS) {
                     lons[lonIdx] = startLon;
                     refLonDist *= static_cast<GLfloat>(inchPerDegEW);
                     lonReadoutXPos[lonIdx] = static_cast<float>(-refLonDist + 0.2);
                     lonReadoutYPos[lonIdx++] = static_cast<float>(t - 0.5f);
                  }
                  glVertex2f(-refLonDist, static_cast<GLfloat>(b));
                  glVertex2f(-refLonDist, static_cast<GLfloat>(t));
                  startLon++;
               }
            glEnd();

            // now make the other lon lines go away
            for (int i = lonIdx; i < MAX_READOUTS; i++) {
               lonReadoutYPos[i] = -10000;
            }
            glPopMatrix();
         }
      }
   }
}

//------------------------------------------------------------------------------
// updateData() -- update background data
//------------------------------------------------------------------------------
void MapPage::updateData(const double dt)
{
    BaseClass::updateData(dt);

    // get our pointers
    if (loader == nullptr) {
        base::Pair* pair = findByName("playerLoader");
        if (pair != nullptr) {
            loader = dynamic_cast<graphics::SymbolLoader*>(pair->object());
            if (loader != nullptr) loader->ref();
        }
    }

    if (pStn == nullptr) {
        graphics::Display* pDsp = getDisplay();
        if (pDsp != nullptr) {
            pStn = static_cast<TestStation*>(pDsp->findContainerByType(typeid(TestStation)));
            if (pStn != nullptr) {
                pStn->ref();
                // set our reference lat / lon initially
                simulation::Simulation* sim = pStn->getSimulation();
                if (sim != nullptr) {
                    setReferenceLatDeg(sim->getRefLatitude());
                    setReferenceLonDeg(sim->getRefLongitude());
                }
            }
        }
    }

   // go through one time and add our symbols for the route
   if (!routeLoaded && pStn != nullptr) {
      base::Pair* pair = findByName("routeLoader");
      if (pair != nullptr) {
         graphics::SymbolLoader* routeLoader = dynamic_cast<graphics::SymbolLoader*>(pair->object());
         if (routeLoader != nullptr) {
            // get our player's route
            simulation::Player* ply = pStn->getOwnship();
            if (ply != nullptr) {
               simulation::Navigation* nav = ply->getNavigation();
               if (nav != nullptr) {
                  simulation::Route* rte = nav->getPriRoute();
                  if (rte != nullptr) {
                     base::safe_ptr<simulation::Steerpoint> stpts[10];
                     unsigned int numStpts = rte->getAllSteerpoints(stpts, 10);
                     for (unsigned int i = 0; i < numStpts; i++) {
                        if (stpts[i] != nullptr) {
                           int pos = routeLoader->addSymbol(1, "stpt");
                           // now update the position
                           routeLoader->updateSymbolPositionLL(pos, stpts[i]->getLatitude(), stpts[i]->getLongitude());
                           // unref when we are done
                           stpts[i] = nullptr;
                        }
                     }
                  }
               }
            }
         }
      }
      routeLoaded = true;
   }

    // let's update our players
    if (loader != nullptr && pStn != nullptr) {
        base::PairStream* stream = pStn->getPlayers();
        if (stream != nullptr) {
            // create our new player list
            simulation::Player* newPlayers[MAX_PLAYERS];
            int numNewPlayers = 0;
            // go through all of our non-ownship players and populate our new list
            base::List::Item* item = stream->getFirstItem();
            while (item != nullptr && numNewPlayers < MAX_PLAYERS) {
                base::Pair* pair = static_cast<base::Pair*>(item->getValue());
                if (pair != nullptr) {
                    simulation::Player* pPlr = dynamic_cast<simulation::Player*>(pair->object());
                    if (pPlr != nullptr) {
                        newPlayers[numNewPlayers] = pPlr;
                        newPlayers[numNewPlayers++]->ref();
                    }
                }
                item = item->getNext();
            }

            // ok, go through our new list and match it with our old, and throw
            // away any old players that aren't in the new list, and add any new
            // players that arent in the old list
            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (player[i] != nullptr) {
                    bool match = false;
                    for (int j = 0; j < numNewPlayers && !match; j++) {
                        if (player[i] == newPlayers[j]) {
                            // if they do match, get rid of our new player, so we don't re-add it
                            // later accidentally
                            match = true;
                            newPlayers[j]->unref();
                            newPlayers[j] = nullptr;
                        }
                    }
                    // if our player doesn't match, we remove it from our list
                    if (!match) {
                        loader->removeSymbol(playerIdx[i]);
                        player[i]->unref();
                        player[i] = nullptr;
                        playerIdx[i] = -1;
                    }
                }
            }

            // ok, now we have removed our old players (and our matched ones), let's add our new ones!
            for (int i = 0; i < numNewPlayers; i++) {
                // make sure this player wasn't deleted earlier
                if (newPlayers[i] != nullptr) {
                    bool found = false;
                    for (int j = 0; j < MAX_PLAYERS && !found; j++) {
                        if (player[j] == nullptr) {
                            found = true;
                            // found an empty player, let's set him!
                            player[j] = newPlayers[i];
                            player[j]->ref();
                            int type = 1;
                            if (player[j]->isSide(simulation::Player::RED)) type = 2;
                            playerIdx[j] = loader->addSymbol(type, "");              //<LDB - "player"
                            if (player[j]->getName() != nullptr) {
                                loader->updateSymbolText(playerIdx[j], "name", player[j]->getName()->getString());
                            }
                            // now let's empty our new player list
                            newPlayers[i]->unref();
                            newPlayers[i] = nullptr;
                        }
                    }
                }
            }

            // ok, now update our symbols' positions
            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (player[i] != nullptr) {
                    loader->updateSymbolPositionLL(playerIdx[i], player[i]->getLatitude(), player[i]->getLongitude());
                    loader->updateSymbolHeading(playerIdx[i], player[i]->getHeadingD());
                }
            }
        }
    }

    // now send our lat / lon text data
    send("lattext%d", UPDATE_VALUE,  lats,           latsSD,           MAX_READOUTS);
    send("latline%d", UPDATE_VALUE,  latReadoutXPos, latReadoutXPosSD, MAX_READOUTS);
    send("latline%d", UPDATE_VALUE2, latReadoutYPos, latReadoutYPosSD, MAX_READOUTS);

    send("lontext%d", UPDATE_VALUE,  lons,           lonsSD,           MAX_READOUTS);
    send("lonline%d", UPDATE_VALUE,  lonReadoutXPos, lonReadoutXPosSD, MAX_READOUTS);
    send("lonline%d", UPDATE_VALUE2, lonReadoutYPos, lonReadoutYPosSD, MAX_READOUTS);
}
