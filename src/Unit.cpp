#include "main.h"

Unit::Unit(double value, UnitTypes unitType)
: mValue(value), mUnitType(unitType)
{
}

Unit::Unit(const Unit& unit)
{
    memcpy(this, &unit, sizeof(Unit));
}

Unit::Unit()
: mValue(0.0), mUnitType(NONE)
{
    
    //LOG_WARNING("Bitte nicht verwenden!");
}

std::string Unit::print()
{
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << mValue << " " << getUnitTypeName(mUnitType);
    return s.str();
}

const char* Unit::getUnitTypeName(UnitTypes unitType)
{
    switch(unitType)
    {
        case NONE: return "- none -";
        case M: return "meter";
        case KM: return "kilometer";
        case LIGHTYEAR: return "lightyears";
        case PARSEC: return "parsec";
        case KILOPARSEC: return "kparsec";
        case AE: return "AE";
        default: return "- default -";
    }
    return "- unknown -";
}