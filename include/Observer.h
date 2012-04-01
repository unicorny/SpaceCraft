#ifndef __SC_OBSERVER__
#define __SC_OBSERVER__

#include "main.h"

enum ObserverType
{
    OBSERVER_NONE = 0,
    OBSERVER_CAMERA = 1
};

class Observer : public DRObjekt
{
public:
    Observer(const DRVector3& position) : DRObjekt(position), mType(OBSERVER_NONE) {}
    Observer(): mType(OBSERVER_NONE) {}
    
    __inline__ ObserverType getType() {return mType;}
    static const char* ObserverTypeName(ObserverType type);
protected:
    ObserverType mType;
};

#endif //__SC_OBSERVER__