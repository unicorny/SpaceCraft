/* 
 * File:   Camera.h
 * Author: dario
 *
 * Created on 20. August 2011, 21:17
 */

#ifndef CAMERA_H
#define	CAMERA_H

class Sektor;

class Camera : public DRObjekt
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
    
    __inline__ Vector3Unit getSektorPosition() const {return mSektorPosition;}
    __inline__ void setSektorPosition(Vector3Unit absPosition) {mSektorPosition = absPosition;}
    
    //! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
    //! \brief bewegt die Kamera relativ
    //! \param translate die Bewegung des Objektes auf allen drei Achsen
    void translateRel_SektorPosition(const DRVector3& translate, const UnitTypes& type);
    
    void translateRel(const DRVector3& translate);
    
    __inline__ void setCurrentSektor(Sektor* current) {mCurrentSektor = current;}
    __inline__ const Sektor* getCurrentSektor() const {return mCurrentSektor;}
    
protected:
    virtual void update();
        
private:
    //! absolute position of camera in the current sector in sector coordinates (example: AE)
    //! the position of camera is the distance from the abs position, abs position is local zero-point
    Vector3Unit mSektorPosition;
    Sektor*     mCurrentSektor;

};

#endif	/* CAMERA_H */

