
#include "Pfd.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/units/Distances.h"

// disable all deprecation warnings for now, until we fix
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

namespace oe {
namespace xpanel {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Pfd, "Pfd")
EMPTY_SERIALIZER(Pfd)
EMPTY_DELETEDATA(Pfd)

Pfd::Pfd()
{
    STANDARD_CONSTRUCTOR()
    // pitch and roll
    pitch = 0;
    pitchSD.empty();
    roll = 0;
    rollSD.empty();
    // bank angle
    baSD.empty();
    bascaleSD.empty();
    // heading and nav stuff
    trueHdg = 0;
    tHdgSD.empty();
    cmdHdg = 0;
    cmdHdgROSD.empty();
    // airspeed
    airSpd = 100;
    airSpdTpSD.empty();
    onesSD.empty();
    spdRstSD.empty();
    // altitude
    alt = 1000;
    altSD.empty();
    alt1SD.empty();
    alt2SD.empty();
    alt3SD.empty();
    altTpSD.empty();
    altTensSD.empty();
    altSelectSD.empty();
    // side slip

    slip = 0;
    slipSD.empty();
    // glideslope
    gSlope = 0;
    gSlopeSD.empty();
    // lateral deviation
    latDev = 0;
    latDevSD.empty();
    // commanded speed
    cmdSpd = 0;
    aBugSD.empty();
    diffSD.empty();
    // commanded alt
    cmdAlt = 5000;
    altBugSD.empty();
    altDiffSD.empty();
    // vvi
    vvi = 0;
    vviSD.empty();
    vviROSD.empty();
    // flight director (command bars)
    fDirBank = 0;
    fDirBankSD.empty();
    fDirPitch = 0;
    fDirPitchSD.empty();
    // selected barometric pressure
    baro = 0;
    baroSD.empty();
    // Hsi send data
    trueHdgSD.empty();
    cmdHdgSD.empty();
    // lat and lon
    refLat = 0;
    refLon = 0;
    range = 0;
    // Gs
    gLoad = 0;
    // mach
    mach = 0;
    //// altitude (meters)
    mAltSD.empty();
    cmdMAltSD.empty();
    pitchGhostSD.empty();
}

void Pfd::copyData(const Pfd& org, const bool)
{
    // Always copy base class stuff first
    BaseClass::copyData(org);

    pitch = org.pitch;
    roll = org.roll;
    trueHdg = org.trueHdg;
    cmdHdg = org.cmdHdg;
    airSpd = org.airSpd;
    alt = org.alt;
    slip = org.slip;
    gSlope = org.gSlope;
    latDev = org.latDev;
    cmdSpd = org.cmdSpd;
    cmdAlt = org.cmdAlt;
    vvi = org.vvi;
    fDirBank = org.fDirBank;
    fDirPitch = org.fDirPitch;
    baro = org.baro;
    refLat = org.refLat;
    refLon = org.refLon;
    range = org.range;
    gLoad = org.gLoad;
    mach = org.mach;
}

bool Pfd::setPitchDeg(const double newP)
{
    pitch = newP;
    return true;
}

bool Pfd::setPitchRad(const double newP)
{
    // convert to degrees
    pitch = static_cast<double>(newP * base::Angle::R2DCC);
    return true;
}

bool Pfd::setRollDeg(const double newR)
{
    roll = newR;
    return true;
}

bool Pfd::setRollRad(const double newR)
{
    // convert to degrees
    roll = static_cast<double>(newR * base::Angle::R2DCC);
    return true;
}

bool Pfd::setTrueHeading(const double newTH)
{
    trueHdg = newTH;
    return true;
}


bool Pfd::setCmdHdg(const double newCH)
{
    cmdHdg = newCH;
    return true;
}

bool Pfd::setAirSpeedKts(const double newAS)
{
    airSpd = newAS;
    return true;
}

bool Pfd::setAltitudeFt(const double newA)
{
    alt = newA;
    return true;
}

bool Pfd::setSideSlip(const double newSS)
{
    slip = newSS;
    return true;
}

bool Pfd::setGlideslope(const double newGS)
{
    gSlope = newGS;
    return true;
}

bool Pfd::setLatDev(const double newLD)
{
    latDev = newLD;
    return true;
}

bool Pfd::setCmdAirSpdKts(const double newCAS)
{
    cmdSpd = newCAS;
    return true;
}

bool Pfd::setCmdAltFt(const double newCA)
{
    cmdAlt = newCA;
    return true;
}

bool Pfd::setVVI(const double newVVI)
{
    vvi = newVVI;
    return true;
}

bool Pfd::setFltDirBankDeg(const double newFDB)
{
    fDirBank = newFDB;
    return true;
}

bool Pfd::setFltDirBankRad(const double newFDB)
{
    fDirBank = static_cast<double>(newFDB * base::Angle::R2DCC);
    return true;
}

bool Pfd::setFltDirPitchDeg(const double newFDP)
{
    fDirPitch = newFDP;
    return true;
}

bool Pfd::setFltDirPitchRad(const double newFDP)
{
    fDirPitch = static_cast<double>(newFDP * base::Angle::R2DCC);
    return true;
}

bool Pfd::setBaroPress(const double newBOP)
{
    baro = newBOP;
    return true;
}

bool Pfd::setMach(const double x)
{
    mach = x;
    return true;
}

bool Pfd::setRefLat(const double newRL)
{
    refLat = newRL;
    return true;
}
bool Pfd::setRefLon(const double newRL)
{
    refLon = newRL;
    return true;
}
bool Pfd::setRange(const double newR)
{
    range = newR;
    return true;
}

bool Pfd::setGLoad(const double newLoad)
{
    gLoad = newLoad;
    return true;
}

void Pfd::updateData(const double dt)
{
    // update our BaseClass
    BaseClass::updateData(dt);

    // find the last digit for the readout tape
    double ones = ((airSpd / 10) - static_cast<int>(airSpd / 10)) * 10;
    // find the 100s value for the dynamic arc segment
    int rest = static_cast<int>(airSpd / 10.0f);

    double diff = airSpd - cmdSpd;

    double altDiff = alt - cmdAlt;
    // let's break the altitude down into ones and tens, so we can
    // send that data to the tape gauge
    double altTens = ((alt/100) - static_cast<int>(alt/100)) * 10;
    // now figure the rest of the number
    int altRest = static_cast<int>(alt/99.9999);

    // all the sends are here
    // hsi
    send("cmdhdg", UPDATE_VALUE, cmdHdg, cmdHdgROSD);
    send("hsi", UPDATE_VALUE6, trueHdg, trueHdgSD);
    send("selhdg", UPDATE_VALUE2, cmdHdg, cmdHdgSD);
    send("spdtape", UPDATE_INSTRUMENTS, ones, onesSD);
    send("spdrest", UPDATE_VALUE, rest, spdRstSD);
    send("fdirbnkang", UPDATE_INSTRUMENTS, fDirBank, fDirBankSD);
    send("baro", UPDATE_VALUE, baro, baroSD);
    send("fdirpitch", UPDATE_INSTRUMENTS, fDirPitch, fDirPitchSD);
    send("vvi", UPDATE_INSTRUMENTS, vvi, vviSD);
    send("vviro", UPDATE_VALUE, vvi, vviROSD);
    send("latdev", UPDATE_INSTRUMENTS, latDev, latDevSD);
    if (altRest < 10) {
        send("altsel", SELECT, 1, altSelectSD);
        send("altrest1", UPDATE_VALUE, altRest, alt1SD);
    }
    else if (altRest < 100) {
        send("altsel", SELECT, 2, altSelectSD);
        send("altrest2", UPDATE_VALUE, altRest, alt2SD);
    }
    else {
        send("altsel", SELECT, 3, altSelectSD);
        send("altrest3", UPDATE_VALUE, altRest, alt3SD);
    }
    // gslope
    send("glideslope", UPDATE_INSTRUMENTS, gSlope, gSlopeSD);
    send("alttens", UPDATE_INSTRUMENTS, altTens, altTensSD);
   send("alttape", UPDATE_INSTRUMENTS, alt, altTpSD);
    send("altbug", UPDATE_INSTRUMENTS, altDiff, altDiffSD);
    send("cmdalt", UPDATE_VALUE, cmdAlt, altBugSD);
    send("spdbug", UPDATE_INSTRUMENTS, diff, diffSD);
    send("cmdspd", UPDATE_VALUE, cmdSpd, aBugSD);
    // actual airspeed tape, not just the readout
    send("airspd", UPDATE_INSTRUMENTS, airSpd, airSpdTpSD);
    // send our pitch(1) and roll(2) to our adi
    send("adi", UPDATE_INSTRUMENTS, pitch, pitchSD);
    send("adi", UPDATE_VALUE, roll, rollSD);
    // send our bank scale the roll value, so it can display properly
    send("bankscale", UPDATE_INSTRUMENTS, roll, bascaleSD);
    // send our bank angle the roll
    send("bankangle", UPDATE_INSTRUMENTS, -roll, baSD);
    // sideslip
    send("sideslip", UPDATE_INSTRUMENTS, slip, slipSD);
    // send our ghost horizon data
    send("ghosthorizonbar", UPDATE_INSTRUMENTS, pitch, pitchGhostSD);
    // convert alt to meters and send it to our meters readout
    int mAlt = static_cast<int>(base::Distance::FeetToMeters(alt));
    double mAltBug = base::Distance::FeetToMeters(cmdAlt);
    send("malt", UPDATE_VALUE, mAlt, mAltSD);
    send("cmdmalt", UPDATE_VALUE, mAltBug, cmdMAltSD);
}

}
}
