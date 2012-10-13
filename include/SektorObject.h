#ifndef __SC_SEKTOR_OBJECT__
#define __SC_SEKTOR_OBJECT__

#include "main.h"
#include "Sektor.h"
#include "Vector3Unit.h"

class SektorObject : public DRObjekt
{
public:
    SektorObject(const DRVector3& position, Sektor* sektor);
    SektorObject();
    
    __inline__ Vector3Unit getSektorPosition() const {return mSektorPosition;}
    //! \brief calculate camera sektor position relative to targetSektor
    Vector3Unit getSektorPositionAtSektor(const Sektor* targetSektor);
    __inline__ void setSektorPosition(Vector3Unit absPosition) {mSektorPosition = absPosition;}
    
    //! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
    //! \brief bewegt die Kamera relativ
    //! \param translate die Bewegung des Objektes auf allen drei Achsen
    void translateRel_SektorPosition(const DRVector3& translate, const UnitTypes& type);
    
    void translateRel(const DRVector3& translate);
    
    
    __inline__ const Sektor* getCurrentSektor() const {return mCurrentSektor;}
    __inline__ uint getSektorPathSize() const {return mSektorPath.size();}
    
    void updateSektorPath();
    
protected:
    //! absolute position of object in the current sector in sector coordinates (example: AE)
    //! the position of object is the distance from the abs position, abs position is local zero-point
    Vector3Unit mSektorPosition;
    Sektor*     mCurrentSektor;
    std::vector<SektorID> mSektorPath;
    
};

#endif //__SC_SEKTOR_OBJECT__