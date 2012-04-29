/* 
 * File:   Camera.cpp
 * Author: dario
 * 
 * Created on 20. August 2011, 21:17
 */

#include "Camera.h"
#include "Sektor.h"

Camera::Camera()
: mSektorPosition(Unit(0, AE)), mCurrentSektor(NULL)
        
{
    mType = OBSERVER_CAMERA;
}

Camera::Camera(const DRVector3& position, Sektor* sektor) 
: Observer(position), mSektorPosition(Unit(0, NONE)), mCurrentSektor(sektor)
{
    mType = OBSERVER_CAMERA;
}

Camera::~Camera() 
{
    setCurrentSektor(NULL);    
}

void Camera::setKameraMatrix()
{
    glMultMatrixf(mMatrix);
}

void Camera::setKameraMatrixRotation()
{
    glMultMatrixf(DRMatrix::axis(mXAxis, mYAxis, mZAxis));
}

void Camera::translateRel_SektorPosition(const DRVector3& translate, const UnitTypes& type)
{
    DRMatrix m1 = DRMatrix::axis(mXAxis, mYAxis, mZAxis);
    DRMatrix m2 = DRMatrix::translation(mSektorPosition.getVector3());
    DRMatrix m = DRMatrix(m1) * DRMatrix(m2);
    mSektorPosition += Vector3Unit(translate.transformNormal(m), type);
    //mAbsPosition.print();
  /*  printf("\r x-axis: %f %f %f, y-axis: %f %f %f, z-axis: %f %f %f, abs: %s %s %s", mXAxis.x, mXAxis.y, mXAxis.z,
                                                    mYAxis.x, mYAxis.y, mYAxis.z,
                                                    mZAxis.x, mZAxis.y, mZAxis.z,       
                                                    mAbsPosition.x.print().data(), mAbsPosition.y.print().data(),
                                                    mAbsPosition.z.print().data());
 // * */
}
void Camera::translateRel(const DRVector3& translate)
{
    DRObjekt::translateRel(translate);
    //printf("\r camera: %f %f %f l:%f", mPosition.x, mPosition.y, mPosition.z, mPosition.length());
    
    if(mPosition.lengthSq() > 1000.0f*1000.0f)
    {
        mSektorPosition += Vector3Unit(mPosition/1000.0f, KM);
        mPosition = 0.0f;
        update();
        printf("update sektor\n");
    }
   
}


void Camera::update()
{
	Vector3Unit pos = mSektorPosition.convertTo(KM);
    //printf("\r camera: %s %s %s: %s", pos.x.print().data(), pos.y.print().data(),
      //                                              pos.z.print().data(), pos.length().print().data());
    DRMatrix m1 = DRMatrix::axis(mXAxis, mYAxis, mZAxis);
    DRMatrix m2 = DRMatrix::translation(-mPosition);
    //mMatrix = DRMatrix(m1) * DRMatrix(m2);
    mMatrix = DRMatrix(m2) * DRMatrix(m1);
   // mMatrix.print();
}

void Camera::updateSektorPath()
{
    if(mCurrentSektor)
        mCurrentSektor->getSektorPath(mSektorPath);
    else
        return;
    std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "[Camera::updateSektorPath] Camera Sektor Path ./data/";
    for (uint i=0; i<mSektorPath.size(); i++) s << "_" << mSektorPath[i] << "/";
    s << std::endl;
    //DRLog.writeToLog(s.str());
}

void Camera::setCurrentSektor(Sektor* current)
{
    if(mCurrentSektor) mCurrentSektor->removeObserver(DRMakeStringHash("Camera"));
    if(current)
		current->addObserver(DRMakeStringHash("Camera"), this);
    mCurrentSektor = current;
}

Vector3Unit Camera::getSektorPositionAtSektor(const Sektor* targetSektor)
{
    if(!targetSektor) return Vector3Unit(0.0f, M);
    if(!mCurrentSektor) return Vector3Unit(0.0f, M);
    if(targetSektor == mCurrentSektor) return mSektorPosition;
    
    Vector3Unit newPos = mSektorPosition;
    Sektor* cur = mCurrentSektor;
    
    std::vector<SektorID> sektorPath;
    targetSektor->getSektorPath(sektorPath);
    
    //DRLog.writeToLog("[Camera::getSektorPositionAtSektor]: targetSektor: %s, currentSektor: %s", targetSektor->getSektorPathName().data(), cur->getSektorPathName().data());
    
    //find break index, last position where both sektor the same
    int breakIndex = 0;
    while(sektorPath[breakIndex+1] == mSektorPath[breakIndex+1]) breakIndex++;
    //DRLog.writeToLog("[Camera::getSektorPositionAtSektor] breakIndex: %d", breakIndex);
    
    int index = 0;
    // TODO: go from root throw list, because childs have same name, with different parents!
    // 1 up
    {
        /*int stop = 0;
        while(sektorPath[stop] == mSektorPath[stop] &&
              stop < mSektorPath.size()-1 &&
              stop < sektorPath.size()-1) 
            stop++;*/
        index = mSektorPath.size()-1;
        //while(index >= static_cast<int>(sektorPath.size()) || sektorPath[index] != mSektorPath[index])
        //while(index >= stop && index >= static_cast<int>(sektorPath.size()))
        while(index > breakIndex)
        {
            newPos += cur->getPosition();
            cur = cur->getParent(); 
            index--;
            if(index < 0)
				LOG_ERROR("haven't the same root", Vector3Unit(0.0, M));
        }
    }
    // 2 down
    {
        while(index < static_cast<int>(sektorPath.size())-1)
        {
            index++;
            SektorID i = sektorPath[index];
            cur = cur->getChild(i);
            if(!cur) LOG_ERROR("sektor didn't exist", Vector3Unit(0.0, M))
            newPos -= cur->getPosition();
        }
    }
    return newPos;
}