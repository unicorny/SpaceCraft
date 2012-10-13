/* 
 * File:   Camera.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 21:17
 */

#include "Camera.h"
#include "Sektor.h"

Camera::Camera()       
{
    mType = OBSERVER_CAMERA;
}

Camera::Camera(const DRVector3& position, Sektor* sektor) 
: Observer(position, sektor)
{
    mType = OBSERVER_CAMERA;
}

Camera::~Camera() 
{
    setCurrentSektor(NULL);    
}

void Camera::setCameraMatrix()
{
    glMultMatrixf(mMatrix);
}

void Camera::setCameraMatrixRotation()
{
    glMultMatrixf(DRMatrix::axis(mXAxis, mYAxis, mZAxis));
}

