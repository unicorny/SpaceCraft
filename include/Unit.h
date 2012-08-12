/* 
 * File:   Unit.h
 * Author: Dario
 *
 * Created on 20. September 2011, 19:57
 */

#ifndef __SPACE_CRAFT_UNIT_H
#define	__SPACE_CRAFT_UNIT_H

#include "main.h"

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


/*typedef u8 UnitTypes;
#define NONE 0
#define M 1
#define KM 2
#define AE 3
#define LIGHTYEAR 4
#define PARSEC 5
#define KILOPARSEC 6
*/

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
    
    Unit operator * (const Unit& b) const;
    __inline__ Unit operator *= (const Unit& b) {*this = *this * b; return *this;}
    
    __inline__ Unit operator *  (const double b) const {return Unit(this->mValue*b, this->mUnitType);}
    __inline__ Unit operator *= (const double b) {*this = Unit(this->mValue*b, this->mUnitType); return *this;}
        
    double operator /(const Unit& b) const;
    //__inline__ double operator /= (const Unit& b) {*this = *this / b; return *this;}
    
    __inline__ Unit operator / (const double b) const {return Unit(this->mValue/b, this->mUnitType);}
    __inline__ Unit operator /= (const double b) {*this = Unit(this->mValue/b, this->mUnitType); return *this;}
    
    __inline__ bool operator == (const Unit& b) const {if(this->mUnitType == b.mUnitType && this->mValue == b.mValue) return true; return false;}
    __inline__ bool operator != (const Unit& b) const {if(this->mUnitType == b.mUnitType && this->mValue == b.mValue) return false; return true;}
    
    bool operator < (const Unit& b) const;
    bool operator <= (const Unit& b) const;
    
    //__inline__ operator double() const {return this->mValue;}
    
    static const char* getUnitTypeName(UnitTypes unitType);
    __inline__ UnitTypes getType() const {return mUnitType;}
    
    operator double() const {return mValue;}
private:
    //!
    static void makeSameType(Unit& a, Unit& b);
    static double getConversionFaktor(UnitTypes a, UnitTypes b);
    
    double mValue;
    UnitTypes mUnitType;
};

__inline__ Unit sqrt(Unit& a) {return Unit(sqrt(a), a.getType());}
//__inline__ Vector3Unit operator *(DRVector3 v, Unit f) {return Vector3Unit(v.x*f, v.y*f, v.z*f, f.getType());}

#endif	/* __SPACE_CRAFT_UNIT_H */

