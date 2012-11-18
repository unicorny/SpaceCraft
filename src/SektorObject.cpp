#include "SektorObject.h"
#include "Sektor.h"

SektorObject::SektorObject(const DRVector3& position, Sektor* sektor)
: DRObjekt(position), mSektorPosition(Unit(0, NONE)), mCurrentSektor(sektor), mSektorPath(sektor ? sektor->getSektorPathPtr(): NULL)
{
}

SektorObject::SektorObject()
: DRObjekt(), mSektorPosition(Unit(0, AE)), mCurrentSektor(NULL), mSektorPath(NULL)
{
    
}


void SektorObject::translateRel_SektorPosition(const DRVector3& translate, const UnitTypes& type)
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
void SektorObject::translateRel(const DRVector3& translate)
{
    DRObjekt::translateRel(translate);
    //printf("\r camera: %f %f %f l:%f", mPosition.x, mPosition.y, mPosition.z, mPosition.length());
    
    if(mPosition.lengthSq() > 1000.0f*1000.0f)
    {
        mSektorPosition += Vector3Unit(mPosition/1000.0f, KM);
        mPosition = 0.0f;
        update();
        //printf("update sektor\n");
    }
   
}


void SektorObject::updateSektorPath()
{
    if(mCurrentSektor)
        mSektorPath = mCurrentSektor->getSektorPathPtr();
    else
        return;
    /*std::stringstream s(std::stringstream::in|std::stringstream::out);
    s << "[Camera::updateSektorPath] Camera Sektor Path ./data/";
    for (uint i=0; i<mSektorPath->size(); i++)
        s << "_" << mSektorPath[i] << "/";
    s << std::endl;
    //DRLog.writeToLog(s.str());
     * */
}

Vector3Unit SektorObject::getSektorPositionAtSektor(const Sektor* targetSektor)
{
    if(!targetSektor) return Vector3Unit(0.0f, M);
    if(!mCurrentSektor) return Vector3Unit(0.0f, M);
    if(targetSektor == mCurrentSektor) return mSektorPosition;
    
    Vector3Unit newPos = mSektorPosition;
    Sektor* cur = mCurrentSektor;
    
    const std::vector<SektorID>& sektorPath = targetSektor->getSektorPath();
    
    //DRLog.writeToLog("[Camera::getSektorPositionAtSektor]: targetSektor: %s, currentSektor: %s", targetSektor->getSektorPathName().data(), cur->getSektorPathName().data());
    
    //find break index, last position where both sektor the same
    uint breakIndex = 0;
    while(breakIndex+1 < sektorPath.size() &&
		  breakIndex+1 < mSektorPath->size() &&
		  sektorPath[breakIndex+1] == (*mSektorPath)[breakIndex+1])
		  breakIndex++;
    //DRLog.writeToLog("[Camera::getSektorPositionAtSektor] breakIndex: %d", breakIndex);
    
    int index = 0;
    // 1 up
    {

        index = mSektorPath->size()-1;
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