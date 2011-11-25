/* 
 * File:   DRGeometrieIcoSphere.h
 * Author: dario
 *
 * Created on 26. August 2011, 08:59
 */

#ifndef __DR_ENGINE_GEOMETRIE_ICO_SPHERE_H
#define	__DR_ENGINE_GEOMETRIE_ICO_SPHERE_H


class ENGINE_API DRGeometrieIcoSphere : public DRGeometrieSphere
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
#endif	/* __DR_ENGINE_GEOMETRIE_ICO_SPHERE_H */

