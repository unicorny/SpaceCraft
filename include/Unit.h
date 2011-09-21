/* 
 * File:   Unit.h
 * Author: Dario
 *
 * Created on 20. September 2011, 19:57
 */

#ifndef __SPACE_CRAFT_UNIT_H
#define	__SPACE_CRAFT_UNIT_H

enum UnitTypes
{
    NONE = 0,
    M = 1, // meter
    KM = 2, // kilometre
    PARSEC = 3,
    KILOPARSEC = 4,
    LIGHTYEAR = 5,
    AE = 6, // Astronomische Einheit, Entfernung Erde - Sonne
};

class Unit
{
public:
    Unit(double value, UnitTypes unitType);
    Unit(const Unit& unit);
    Unit();
    
    std::string print();
    
    static const char* getUnitTypeName(UnitTypes unitType);
private:
    double mValue;
    UnitTypes mUnitType;
};

#endif	/* __SPACE_CRAFT_UNIT_H */

