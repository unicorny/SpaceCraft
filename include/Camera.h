/* 
 * File:   Camera.h
 * Author: dario
 *
 * Created on 20. August 2011, 21:17
 */

#ifndef CAMERA_H
#define	CAMERA_H

//class DRObject;

class Camera : public DRObjekt
{
public:
    Camera();
    Camera(const DRVector3& position, const DRVector3& rotation);
    virtual ~Camera();
    
    void rotateAbs(const DRVector3& rotation);
    void rotateRel(const DRVector3& rotation);
    
     //! diese Funktion sollte vorm rendern aller anderen Objekte aufgerufen werden
    //! \brief setzt die Objekt Matrix als Kameramatrix
    void setKameraMatrix();
private:

};

#endif	/* CAMERA_H */

