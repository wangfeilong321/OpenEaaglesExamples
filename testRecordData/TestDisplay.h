
#ifndef __TestDisplay_H__
#define __TestDisplay_H__

#include "openeaagles/gui/glut/GlutDisplay.h"

namespace oe {
   namespace simulation { class Missile; class Player; class Simulation; class Station; }
   namespace graphics { class SymbolLoader; }
   namespace xpanel { class DspRadar; class DspRwr; }
}

//------------------------------------------------------------------------------
// Class: TestDisplay
// Description: Test GLUT-display that will manage a simple real-beam, b-scan radar
//              display, plus a Radar receiver display, which shows received
//              signal strength and angle of arrival, a simple situation
//              display that shows the 'truth' location of the players, and
//              Primary Flight Display (PFD).
//
// Factory name: TestDisplay
//
// Events: (all keyboard events)
//   'r' or 'R'   -- Reset simulation
//   'f' or 'F'   -- Toggle simulation freeze
//   'l' or 'L'   -- Launch Missile
//   'a' or 'A'   -- A/A mode (TWS)
//   'g' or 'G'   -- A/G mode (GMTI)
//   's' or 'S'   -- Target step
//   'i' or 'I'   -- Increase Range
//   'd' or 'D'   -- Decrease Range
//   '+'          -- Ownship step (to next local air vehicle)
//------------------------------------------------------------------------------
class TestDisplay : public oe::glut::GlutDisplay
{
    DECLARE_SUBCLASS(TestDisplay, oe::glut::GlutDisplay)

public:
    enum { MAX_TRACKS = 60 };

    enum { NCHAR_NAV1_ID = 3, NCHAR_NAV2_ID = 5 };

public:
    TestDisplay();

    oe::simulation::Player* getOwnship();
    oe::simulation::Simulation* getSimulation();
    oe::simulation::Station* getStation();

    void maintainAirTrackSymbols(oe::graphics::SymbolLoader* loader, const double rng);

    virtual void mouseEvent(const int button, const int state, const int x, const int y) override;

    virtual bool event(const int event, oe::base::Object* const obj = nullptr) override;
    virtual void updateData(const double dt = 0.0) override;

protected:
    virtual bool shutdownNotification() override;

private:
    // Key event handlers
    bool onMarkerKey();
    bool onResetKey();
    bool onFreezeKey();
    bool onWpnRelKey();
    bool onTgtStepKey();
    bool onRtn2SearchKey();
    bool onAir2AirKey();
    bool onAir2GndKey();
    bool onIncRngKey();
    bool onDecRngKey();
    bool onStepOwnshipKey();

    void updatePfd(const double dt);

    // Extended data recorder test values
    unsigned int fooCnt;
    unsigned int myDataCnt;

    oe::xpanel::DspRadar*   rdrDisplay;     // Test RADAR display
    oe::xpanel::DspRwr*     rwrDisplay;     // Test RWR display
    double range;                           // SD range

    SendData headingSD;
    SendData rangeSD;

    oe::base::safe_ptr<oe::simulation::Station> myStation;

    oe::simulation::Player* tracks[MAX_TRACKS];    // players that we're displaying
    int                 trkIdx[MAX_TRACKS];        // Index of track symbols

    // pitch and roll
    double pitch;           // degs
    double pitchRate;       // degs/sec
    double roll;            // degs
    double rollRate;        // degs/sec

    // heading and nav stuff
    double trueHdg;         // degs
    double tHdgRate;        // degs/sec
    double cmdHdg;          // commanded heading (heading bug) (degs)
    double cmdHdgRate;      // degs/sec

    // airspeed
    double airSpd;
    double airSpdRate;

    // altitude
    double alt;
    double altRate;

    // side slip
    double slip;            // degs
    double slipRate;        // degs/sec

    // glideslope (in dots)
    double gSlope;
    double gSlopeRate;

    // Lateral dev
    double latDev;
    double ldRate;

    // commanded speed
    double cmdSpd;

    // commanded alt
    double cmdAlt;

    // vvi
    double vvi;
    double vviRate;

    // flight director stuff (in inches)
    double fDirBank;
    double fDirBankRate;
    double fDirPitch;
    double fDirPitchRate;

    // barometric pressure
    double baro;
    double baroRate;
};

#endif
