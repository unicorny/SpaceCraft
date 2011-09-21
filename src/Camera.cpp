/* 
 * File:   Camera.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 21:17
 */

#include "main.h"

Camera::Camera() 
{
}

Camera::Camera(const DRVector3& position) 
: DRObjekt(position)
{
}

Camera::~Camera() 
{
}

void Camera::setKameraMatrix()
{
    glMultMatrixf(mMatrix.transpose());
}
/*
void Camera::rotateAbs(const DRVector3& rotation)
{
    // Rotation um die x-Achse
        DRMatrix mRotation(DRMatrix::rotationX(rotation.x));
	mYAxis = mYAxis.transformNormal(mRotation);
	mZAxis = mXAxis.cross(mYAxis);

	// Rotation um die y-Achse
        mRotation = DRMatrix::rotationY(rotation.y);
	mXAxis = mXAxis.transformNormal(mRotation);
	mZAxis = mXAxis.cross(mYAxis);

	// Rotation um die z-Achse
        mRotation = DRMatrix::rotationZ(rotation.z);
	mXAxis = mXAxis.transformNormal(mRotation);
	mYAxis = mYAxis.transformNormal(mRotation);

	// Matrizen aktualisieren
	update();
}

void Camera::rotateRel(const DRVector3& rotation)
{
    // Rotation um die x-Achse des Objekts
        DRMatrix mRot = DRMatrix::rotationAxis(mXAxis, rotation.x);
	mYAxis = mYAxis.transformNormal(mRot);
	mZAxis = mXAxis.cross(mYAxis);
                
	// Rotation um die y-Achse des Objekts
        mRot = DRMatrix::rotationAxis(mYAxis, rotation.y);
	mXAxis = mXAxis.transformNormal(mRot);
	mZAxis = mXAxis.cross(mYAxis);

	// Rotation um die z-Achse des Objekts
        mRot = DRMatrix::rotationAxis(mZAxis, rotation.z);
	mXAxis = mXAxis.transformNormal(mRot);
	mYAxis = mYAxis.transformNormal(mRot);

	// Matrizen aktualisieren
	update();
}
 * //*/