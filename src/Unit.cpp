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

std::string Unit::print() const
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

Unit Unit::convertTo(UnitTypes type) const
{
    return Unit(this->mValue * getConversionFaktor(this->mUnitType, type), type);
}

Unit Unit::operator + (const Unit& b) const
{
    if(this->mUnitType == b.mUnitType)
    {
        return Unit(mValue + b.mValue, this->mUnitType); 
    }
    else
    {
        if((int)this->mUnitType < (int)b.mUnitType)
            return *this + b.convertTo(this->mUnitType);
        else
            return convertTo(b.mUnitType) + b;
       //convertTo() 
    }
}

Unit Unit::operator - (const Unit& b) const
{
    if(this->mUnitType == b.mUnitType)
    {
        return Unit(mValue - b.mValue, this->mUnitType); 
    }
    else
    {
        if((int)this->mUnitType < (int)b.mUnitType)
            return *this - b.convertTo(this->mUnitType);
        else
            return convertTo(b.mUnitType) - b;
       //convertTo() 
    }
}

Unit Unit::operator * (const Unit& b) const
{
    if(this->mUnitType == b.mUnitType)
    {
        return Unit(mValue * b.mValue, this->mUnitType); 
    }
    else
    {
        if((int)this->mUnitType < (int)b.mUnitType)
            return *this * b.convertTo(this->mUnitType);
        else
            return convertTo(b.mUnitType) * b;
       //convertTo() 
    }
}

Unit Unit::operator / (const Unit& b) const
{
    if(this->mUnitType == b.mUnitType)
    {
        return Unit(mValue / b.mValue, this->mUnitType); 
    }
    else
    {
        if((int)this->mUnitType < (int)b.mUnitType)
            return *this / b.convertTo(this->mUnitType);
        else
            return convertTo(b.mUnitType) / b;
       //convertTo() 
    }
}

bool Unit::operator < (const Unit& b) const
{
    if(this->mUnitType == b.mUnitType)
    {
        return mValue < b.mValue; 
    }
    else
    {
        if((int)this->mUnitType < (int)b.mUnitType)
            return mValue < b.convertTo(this->mUnitType).mValue;
        else
            return convertTo(b.mUnitType).mValue < b.mValue;
       //convertTo() 
    }
}
bool Unit::operator <= (const Unit& b) const
{
    if(this->mUnitType == b.mUnitType)
    {
        return mValue <= b.mValue; 
    }
    else
    {
        if((int)this->mUnitType < (int)b.mUnitType)
            return mValue <= b.convertTo(this->mUnitType).mValue;
        else
            return convertTo(b.mUnitType).mValue <= b.mValue;
       //convertTo() 
    }
}

double Unit::getConversionFaktor(UnitTypes a, UnitTypes b)
{
    if(a == b) return 1.0;
    if(a > KILOPARSEC || a < NONE) LOG_ERROR("unknown value for a", 0.0);
    if(b > KILOPARSEC || b < NONE) LOG_ERROR("unknown value for b", 0.0);
    double conversion = 1.0;
    bool turn = true;
    if(a > b)
    {
        UnitTypes t = a;
        a = b;
        b = t;
        turn = false;
    }
   // printf("turn: %d\n", turn);
    
    //jetzt ist a immer kleiner als b
    if(a > b) LOG_ERROR("[critical]", 0.0);
    
    if(a+1 < b)
    {
      //  printf("a: %d, b: %d (a+1 <= b)\n", (int)a, (int)b);
        while(a+1 < b)
        {
            conversion *= 1.0/getConversionFaktor((UnitTypes)((int)b-1), b);
         //   printf("zwischenschritt conversion: %f\n", conversion);
            b = (UnitTypes)((int)b - 1);
        }            
    }
    if(a +1 == b)
    {
    //    printf("a: %d, b: %d (a+1 == b)\n", (int)a, (int)b);
        double temp = 0.0;
        switch(a)
        {
            case NONE: break;;
            case M: temp = 1000.0; break;
            case KM: temp = 149.6E6; break;
            case AE: temp = 6.3243E4; break;
            case LIGHTYEAR: temp = 3.2616; break;
            case PARSEC: temp = 1000.0; break;
            default: break;
        }
    //    printf("temp: %f\n", temp);
        conversion *= temp;
//        printf("conversion *= temp: %f\n", conversion);
        if(turn)
            conversion = 1.0 / conversion;
   //     printf("conversion = 1.0/ conversion: %f\n", conversion);
        return conversion;
    }  
    else
    {
        printf("a: %d, b: %d (else)\n", (int)a, (int)b);
        LOG_WARNING("unknown situation");
        return 0.0;
    }
}