
#include "Display.h"
#include "TestObject.h"

#include "openeaagles/base/Color.h"
#include "openeaagles/graphics/Material.h"
#include "openeaagles/base/Rng.h"
#include "openeaagles/base/Number.h"
#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/base/util/math_utils.h"
#include "openeaagles/base/util/system.h"

#include <cstring>

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

using namespace oe;

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Display, "SendDataDisplay")
EMPTY_SERIALIZER(Display)

Display::Display()
{
    STANDARD_CONSTRUCTOR()

    myBool = false;
    boolSD.empty();
    myInt = 0;
    intSD.empty();
    myFloat = 0.0f;
    floatSD.empty();
    myDouble = 0.0;
    doubleSD.empty();
    obj = new TestObject();
    base::utStrcpy(myChar, sizeof(myChar), "ASCII");
    charSD.empty();
    myColor = new base::Color();
    myColor->setRed(0.0);
    myColor->setBlue(0.0);
    myColor->setGreen(0.0);

    // setup a random number generator to start our colors
    base::Rng* rng = new base::Rng();
    osg::Vec4 diffColor[MAX_MATERIALS];
    // this will get our computer time, and take the result, giving us
    // a random seed to start our generator
    double x = base::getComputerTime();
    x -= static_cast<int>(x);
    x *= 10;
    int seed = base::nint(static_cast<double>(x));

    // go through x amount of numbers before we get our next random number
    // this will allow for some pseudo-randomness.
    for (int i = 0; i < seed; i++) rng->drawClosed();

    for (int i = 0; i < MAX_MATERIALS; i++) {
        materials[i] = new graphics::Material();
        materialSD[i].empty();
        diffColor[i].set(static_cast<double>(rng->drawClosed()),
                         static_cast<double>(rng->drawClosed()),
                         static_cast<double>(rng->drawClosed()), 1);
        //std::cout << "DIFF COLOR = " << diffColor[i].x() << ", " << diffColor[i].y() << ", " << diffColor[i].z() << std::endl;
        materials[i]->setDiffuseColor(diffColor[i]);
        // set up initial different colors
        diffColorRate[i].set(1,1,1);
        rotations[i] = 0;
        rotationsSD[i].empty();
    }

    rng->unref();
    counter = 0;
}

void Display::copyData(const Display& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        obj = nullptr;
        myColor = nullptr;
        for (int i = 0; i < MAX_MATERIALS; i++) materials[i] = nullptr;
    }

    myBool = org.myBool;
    boolSD.empty();
    myInt = org.myInt;
    intSD.empty();
    myFloat = org.myFloat;
    floatSD.empty();
    myDouble = org.myDouble;
    doubleSD.empty();
    if (obj != nullptr) obj->unref();
    if (org.obj != nullptr) {
        obj = org.obj->clone();
    }
    if (myColor != nullptr) myColor->unref();
    if (org.myColor != nullptr) {
        myColor = org.myColor->clone();
    }

    for (int i = 0; i < MAX_MATERIALS; i++) {
        if (materials[i] != nullptr) {
            materials[i]->unref();
            materials[i] = nullptr;
        }
        diffColorRate[i] = org.diffColorRate[i];
        if (org.materials[i] != nullptr) materials[i] = org.materials[i]->clone();
        materialSD[i].empty();
        rotations[i] = org.rotations[i];
        rotationsSD[i].empty();
    }

    base::utStrcpy(myChar, sizeof(myChar), org.myChar);

    counter = org.counter;
}

void Display::deleteData()
{
    if (obj != nullptr) {
        obj->unref();
        obj = nullptr;
    }
    if (myColor != nullptr) {
        myColor->unref();
        myColor = nullptr;
    }
    for (int i = 0; i < MAX_MATERIALS; i++) {
        if (materials[i] != nullptr) {
            materials[i]->unref();
            materials[i] = nullptr;
        }
    }
}

void Display::updateData(const double dt)
{
    BaseClass::updateData(dt);

    counter++;
    if (counter > 1) {
        myBool = !myBool;

        myInt++;
        if (myInt > 999) myInt = 0;

        myFloat += 0.002f;
        if (myFloat > 100) myFloat = 0;

        myDouble += 0.00002f;
        if (myDouble > 2) myDouble = 0;

        if (std::strcmp(myChar, "ASCII") == 0) base::utStrcpy(myChar, sizeof(myChar), "TEXT");
        else base::utStrcpy(myChar, sizeof(myChar), "ASCII");

        obj->setBoolean(!obj->getBoolean());
        obj->setInteger(obj->getInteger() + 1);
        obj->setFloat(obj->getFloat() + 0.01f);
        obj->setDouble(obj->getDouble() + 0.0003);
        obj->setReal(obj->getReal() + 0.1f);
        if (std::strcmp(obj->getChar(), "ASCII") == 0) obj->setChar("TEXT");
        else obj->setChar("ASCII");

        if (myColor->red() < 0.9f) myColor->setRed(myColor->red() + dt);
        else myColor->setRed(0.0f);
        if (myColor->blue() < 0.9f) myColor->setBlue(myColor->blue() + (2 * dt));
        else myColor->setBlue(0.0);
        if (myColor->green() < 0.9f) myColor->setGreen(myColor->green() + (3 * dt));
        else myColor->setGreen(0.0);

        // our materials
        osg::Vec4 diff;
        double x = 0, y = 0, z = 0;
        for (int i = 0; i < MAX_MATERIALS; i++) {
            if (materials[i] != nullptr) {
                if (i == 0) {
                    diff = materials[i]->getDiffuseColor();
                    rotations[i] += 40 * dt;
                }
                else if (i == 1) {
                    diff = materials[i]->getAmbientColor();
                    rotations[i] -= 45 * dt;
                }
                else if (i == 2) {
                    diff = materials[i]->getEmissiveColor();
                    rotations[i] += 30 * dt;
                }
                x = diff.x();
                y = diff.y();
                z = diff.z();
                if (x > 1 || x < 0) diffColorRate[i].x() = -diffColorRate[i].x();
                x += diffColorRate[i].x() * 0.01f;
                if (y > 1 || y < 0) diffColorRate[i].y() = -diffColorRate[i].y();
                y += diffColorRate[i].y() * 0.02f;
                if (z > 1 || z < 0) diffColorRate[i].z() = -diffColorRate[i].z();
                z += diffColorRate[i].z() * 0.025f;
                diff.set(x,y,z,1);
                if (i == 0) materials[i]->setDiffuseColor(diff);
                else if (i == 1) materials[i]->setAmbientColor(diff);
            }
        }

        counter = 0;
    }


    send("boolean", UPDATE_VALUE, myBool, boolSD);
    send("integer", UPDATE_VALUE, myInt, intSD);
    send("float", UPDATE_VALUE, myFloat, floatSD);
    send("double", UPDATE_VALUE, myDouble, doubleSD);
    send("ascii", UPDATE_VALUE, myChar, charSD);
    send("objtest", UPDATE_VALUE, obj, objSD);
    send("colors", SET_COLOR, myColor, colorSD);
    // convert materials to objects real quick, so we can send them down
    base::Object* tempMat[MAX_MATERIALS];
    for (int i = 0; i < MAX_MATERIALS; i++) {
        tempMat[i] = static_cast<base::Object*>(materials[i]);
    }
    send("matarray%d", SET_MATERIAL, tempMat, materialSD, MAX_MATERIALS);
    // send rotations to our objects as well
    send("rotators%d", UPDATE_VALUE2, rotations, rotationsSD, MAX_MATERIALS);
}
