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
: DRObjekt(position), mAbsPosition(Unit(0, NONE))
{
}

Camera::~Camera() 
{
}

void Camera::setKameraMatrix()
{
    glMultMatrixf(mMatrix);
}

void Camera::setKameraMatrixRotation()
{
    glMultMatrixf(DRMatrix::axis(mXAxis, mYAxis, mZAxis));
}

void Camera::translateRel_AbsPosition(const DRVector3& translate, const UnitTypes& type)
{
    DRMatrix m1 = DRMatrix::axis(mXAxis, mYAxis, mZAxis);
    DRMatrix m2 = DRMatrix::translation(mAbsPosition.getVector3());
    DRMatrix m = DRMatrix(m1) * DRMatrix(m2);
    mAbsPosition -= Vector3Unit(translate.transformNormal(m), type);
    //mAbsPosition.print();
 /*   printf("\r x-axis: %f %f %f, y-axis: %f %f %f, z-axis: %f %f %f, abs: %s %s %s", mXAxis.x, mXAxis.y, mXAxis.z,
                                                    mYAxis.x, mYAxis.y, mYAxis.z,
                                                    mZAxis.x, mZAxis.y, mZAxis.z,       
                                                    mAbsPosition.x.print().data(), mAbsPosition.y.print().data(),
                                                    mAbsPosition.z.print().data());
  * */
}
void Camera::translateRel(const DRVector3& translate)
{
    DRObjekt::translateRel(translate);
    //printf("\r camera: %f %f %f l:%f", mPosition.x, mPosition.y, mPosition.z, mPosition.length());
    if(mPosition.lengthSq() > 1000.0f*1000.0f)
    {
        mAbsPosition += Vector3Unit(mPosition/1000.0f, KM);
        mPosition = 0.0f;
        update();
        printf("update sektor\n");
    }
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