/* 
 * File:   Camera.h
 * Author: dario
 *
 * Created on 20. August 2011, 21:17
 */

#ifndef CAMERA_H
#define	CAMERA_H

class Sektor;
struct SektorID;

//#include "main.h"
#include "Observer.h"
#include "Sektor.h"
#include "Vector3Unit.h"



class Camera : public Observer
{
public:
    Camera();
    Camera(const DRVector3& position, Sektor* sektor = NULL);
    virtual ~Camera();
    
    //void rotateAbs(const DRVector3& rotation);
    //void rotateRel(const DRVector3& rotation);
    
     //! diese Funktion sollte vorm rendern aller anderen Objekte aufgerufen werden
    //! \brief setzt die Objekt Matrix als Kameramatrix
    void setKameraMatrix();
    
    //! diese Funktion sollte vorm rendern aller anderen Objekte aufgerufen werden
    //! \brief setzt die Objekt-Rotation als Kameramatrix
    void setKameraMatrixRotation();

    DRMatrix getKameraMatrixRotation() {return DRMatrix::axis(mXAxis, mYAxis, mZAxis);}
    
    __inline__ Vector3Unit getSektorPosition() const {return mSektorPosition;}
    //! \brief calculate camera sektor position relative to targetSektor
    Vector3Unit getSektorPositionAtSektor(const Sektor* targetSektor);
    __inline__ void setSektorPosition(Vector3Unit absPosition) {mSektorPosition = absPosition;}
    
    //! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
    //! \brief bewegt die Kamera relativ
    //! \param translate die Bewegung des Objektes auf allen drei Achsen
    void translateRel_SektorPosition(const DRVector3& translate, const UnitTypes& type);
    
    void translateRel(const DRVector3& translate);
    
    void setCurrentSektor(Sektor* current);
    __inline__ const Sektor* getCurrentSektor() const {return mCurrentSektor;}
    __inline__ uint getSektorPathSize() const {return mSektorPath.size();}
    
    void updateSektorPath();
    
   
protected:
    virtual void update();
        
private:
    //! absolute position of camera in the current sector in sector coordinates (example: AE)
    //! the position of camera is the distance from the abs position, abs position is local zero-point
    Vector3Unit mSektorPosition;
    Sektor*     mCurrentSektor;
    std::vector<SektorID> mSektorPath;
        
};

#endif	/* CAMERA_H */

