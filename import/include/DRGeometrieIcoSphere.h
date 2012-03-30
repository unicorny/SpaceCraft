/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

/* 
 * File:   DRGeometrieIcoSphere.h
 * Author: dario
 *
 * Created on 26. August 2011, 08:59
 */

#ifndef __DR_ENGINE2_GEOMETRIE_ICO_SPHERE__
#define	__DR_ENGINE2_GEOMETRIE_ICO_SPHERE__

class ENGINE2_API DRHeightValueStorage
{
public:
    virtual float getHeightValue(DRVector3& position) = 0;
    virtual DRColor getColorValue(const float height) = 0;
};

class ENGINE2_API DRGeometrieIcoSphere : public DRGeometrieSphere
{
public:
    //! \param maxFaceBuffer count of keeping free memory for IceSphereFace
    DRGeometrieIcoSphere(GLuint maxFaceBuffer = 100);
    virtual ~DRGeometrieIcoSphere();
    
    /*! \param maxEbene höchste zu erstellende Ebene <br> \
     *  (z.B. VY Canis Majoris = 51, Erde = 23)
     * 
     */
    DRReturn initIcoSphere(u8 maxEbene = 0, int seed = 0);
    /*! \param relCameraPos camera position relative zu IcoSphere */
    DRReturn changeGeometrieTo(u8 ebene = 0, bool waitToComplete = false, DRVector3 relCameraPos = DRVector3(0.0f));
    DRReturn update();
    
    __inline__ void setHeightValueStorage(DRHeightValueStorage* heightValueStorage) {mHeightValues = heightValueStorage;}
    
private:
    DRGeometrieIcoSphere(const DRGeometrieIcoSphere& orig) {LOG_WARNING("Not exist");}
        static int updateGeometrieThread(void* data);
    
    struct IcoSphereFace
    {
        IcoSphereFace(int seed = 0);
        ~IcoSphereFace() {reset();}
        void reset(DRGeometrieIcoSphere* sphere = NULL);
        bool hasChilds();
        IcoSphereFace* getChildAtBorder(GLuint borderIndices[2], IcoSphereFace* caller = NULL);
        
        IcoSphereFace* mNeighbors[3];
        IcoSphereFace* mChilds[4];
        IcoSphereFace* mParent;
        GLuint         mIndices[3];
        int            mSeed;
    };
    
    IcoSphereFace* newFace();
    IcoSphereFace* newChildFace(IcoSphereFace* parent, int childCursor, int seeds);
    void deleteFace(IcoSphereFace* face);
    
    //achtung! rekursive funktion!
    void subdivide(IcoSphereFace* current = NULL, u8 currentEbene = 0);
    void removeLeafs(IcoSphereFace* current = NULL, u8 currentEbene = 0);
    
    //! \return true if face is visible (Horizont Culling)
    bool isFaceVisible(IcoSphereFace* face);
    
    //achtung! rekursive funktion!  
    DRReturn grabIndicesFromFaces(IcoSphereFace* current = NULL);
    void reset(bool full = true);
    
    DRReturn calculateNormals(uint oldVertexCount);    
    
    std::list<IcoSphereFace*>           mFreeIcoFaceMemory;
    IcoSphereFace                       mRootSphereFaces[20];
    DRIndexReferenzHolder*              mIndexReferenzen;
    u8                                  mMaxEbene;
    u8					mCurrentEbene;
    u8					mNewEbene;
    GLuint                              mMaxFaceBuffer;
    SDL_Thread*				mUpdateThread;
    SDL_sem *				mUpdateThreadSemaphore;
    SDL_mutex*                          mUpdateGeometrieMutex;
    
    static float                        mVektorLength;          
    unsigned int                        mVertexCursor;      
    unsigned int                        mEbeneNeighborCount;
    uint                                mFacesSphereCount;
    DRHeightValueStorage*               mHeightValues;
    DRVector3                           mRelCameraPos;
    float                               mHorizontAngle;
    bool                                mUpdateChanged;

};
//*/
#endif	/* __DR_ENGINE2_GEOMETRIE_ICO_SPHERE__ */

