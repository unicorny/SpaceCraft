#include "Observer.h"

const char* Observer::ObserverTypeName(ObserverType type)
{
    switch(type)
    {
        case OBSERVER_NONE: return "none";
        case OBSERVER_CAMERA: return "camera";
        default: return "-unknown-";
    }
    return "-error-";
}