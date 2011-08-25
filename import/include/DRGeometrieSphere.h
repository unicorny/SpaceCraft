/* 
 * File:   DRGeometrieSphere.h
 * Author: dario
 *
 * Created on 25. August 2011, 12:33
 */

#ifndef _DR_ENGINE_GEOMETRIE_SPHERE_H
#define	_DR_ENGINE_GEOMETRIE_SPHERE_H

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
};

#endif	/* _DR_ENGINE_GEOMETRIE_SPHERE_H */

