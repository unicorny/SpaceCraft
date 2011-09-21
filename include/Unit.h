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
    AE = 3, // Astronomische Einheit, Entfernung Erde - Sonne
    LIGHTYEAR = 4,
    PARSEC = 5,
    KILOPARSEC = 6    
};

class Unit
{
public:
    Unit(double value, UnitTypes unitType);
    Unit(const Unit& unit);
    Unit();
    
    Unit convertTo(UnitTypes type) const;
    std::string print() const;
    //! \return true if both have the same type
    bool compareType(Unit& b) const {if(this->mUnitType == b.mUnitType) return true; return false;}
    
    // for calculating
    Unit operator + (const Unit& b) const;
    __inline__ Unit operator += (const Unit& b) {*this = *this + b; return *this;}
    
    Unit operator -(const Unit& b) const;
    __inline__ Unit operator -() const {return Unit(-this->mValue, this->mUnitType);}
    __inline__ Unit operator -= (const Unit& b) {*this = *this - b; return *this;}
    
    Unit operator *(const Unit& b) const;
    __inline__ Unit operator *= (const Unit& b) {*this = *this * b; return *this;}
    
    Unit operator /(const Unit& b) const;
    __inline__ Unit operator /= (const Unit& b) {*this = *this / b; return *this;}
    
    __inline__ bool operator == (const Unit& b) const {if(this->mUnitType == b.mUnitType && this->mValue == b.mValue) return true; return false;}
    __inline__ bool operator != (const Unit& b) const {if(this->mUnitType == b.mUnitType && this->mValue == b.mValue) return false; return true;}
    
    __inline__ operator double() const {return this->mValue;}
    
    static const char* getUnitTypeName(UnitTypes unitType);
    __inline__ UnitTypes getType() const {return mUnitType;}
private:
    //!
    static double getConversionFaktor(UnitTypes a, UnitTypes b);
    
    double mValue;
    UnitTypes mUnitType;
};

#endif	/* __SPACE_CRAFT_UNIT_H */

