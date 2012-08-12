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
 * File:   DRGeometrieSphere.h
 * Author: dario
 *
 * Created on 25. August 2011, 12:33
 */

#ifndef __DR_ENGINE2_GEOMETRIE_SPHERE__
#define	__DR_ENGINE2_GEOMETRIE_SPHERE__

class DRGeometrie;

class ENGINE2_API DRGeometrieSphere : public DRGeometrie
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
        PlaneData(GLuint gesPlaneCount, int _seed) 
        : mutex(NULL), planes(NULL), planeCount(0), absPlaneCount(gesPlaneCount), seed(_seed)
        {
            mutex = SDL_CreateMutex();
            planes = new DRPlane[gesPlaneCount];
        }
        
        ~PlaneData() {SDL_DestroyMutex(mutex); DR_SAVE_DELETE_ARRAY(planes);}
        
        SDL_mutex* mutex;
        DRPlane*   planes;
        GLuint        planeCount;
        GLuint        absPlaneCount;
        int            seed;
        
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

#endif	/* __DR_ENGINE2_GEOMETRIE_SPHERE__ */

