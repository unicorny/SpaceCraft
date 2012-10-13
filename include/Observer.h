#ifndef __SC_OBSERVER__
#define __SC_OBSERVER__

#include "SektorObject.h"

enum ObserverType
{
    OBSERVER_NONE = 0,
    OBSERVER_CAMERA = 1
};

class Observer : public SektorObject
{
public:
    Observer(const DRVector3& position, Sektor* sektor) : SektorObject(position, sektor), mType(OBSERVER_NONE) {}
    Observer(): mType(OBSERVER_NONE) {}
    
    __inline__ ObserverType getType() {return mType;}
    static const char* ObserverTypeName(ObserverType type);
    
    void setCurrentSektor(Sektor* current);
protected:
    ObserverType mType;
};

#endif //__SC_OBSERVER__