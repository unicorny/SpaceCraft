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
    void setCameraMatrix();
    
    //! diese Funktion sollte vorm rendern aller anderen Objekte aufgerufen werden
    //! \brief setzt die Rotations-Matrix als Kameramatrix
    void setCameraMatrixRotation();

    DRMatrix getCameraMatrixRotation() {return DRMatrix::axis(mXAxis, mYAxis, mZAxis);}
    
     
   
protected:
   // virtual void update();
        

        
};

#endif	/* CAMERA_H */

