/* 
 * File:   DRGeometrieSphere.h
 * Author: dario
 *
 * Created on 25. August 2011, 12:33
 */

#ifndef __DR_ENGINE_GEOMETRIE_SPHERE_H
#define	__DR_ENGINE_GEOMETRIE_SPHERE_H

class DRGeometrie;

class ENGINE_API DRGeometrieSphere : public DRGeometrie
{
public:
    DRGeometrieSphere();
    virtual ~DRGeometrieSphere();
    
    /*! \brief generate a sphere geometrie
     *  \param segmentSize  gesamt vertexCount = segmentSize²
     * */
    DRReturn initSphere(GLuint segmentSize);
    
    /*! \brief manipulate a sphere, to make a landscape from it
     *  \param numIterations iteration count, for every iteration,<br>
     *         every point will be manipulated
     * \param randomSeed seed for landscape, same seed, same landscape
     */
    void makeSphericalLandscape(GLuint numIterations, GLuint randomSeed);
    
    
private:
    DRGeometrieSphere(const DRGeometrieSphere& orig) {LOG_WARNING("not exist");};
    static int makeLandscapeThread(void* data);
    
    struct PlaneData
    {
        PlaneData(GLuint gesPlaneCount) 
        : mutex(NULL), planes(NULL), planeCount(0), absPlaneCount(gesPlaneCount)
        {
            mutex = SDL_CreateMutex();
            planes = new DRPlane[gesPlaneCount];
        }
        
        ~PlaneData() {SDL_DestroyMutex(mutex); DR_SAVE_DELETE_ARRAY(planes);}
        
        SDL_mutex* mutex;
        DRPlane*   planes;
        GLuint        planeCount;
        GLuint        absPlaneCount;
        
        void lock() {SDL_mutexP(mutex);}
        void unlock() {SDL_mutexV(mutex);}
    };
    
    struct LandscapeGenerateMultithreadData
    {
        PlaneData* planes;        
        DRVector3* vertices;
        int        vertexCount;
    };
};

#endif	/* __DR_ENGINE_GEOMETRIE_SPHERE_H */

