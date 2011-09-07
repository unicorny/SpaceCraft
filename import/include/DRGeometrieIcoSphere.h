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
    DRGeometrieIcoSphere();
    virtual ~DRGeometrieIcoSphere();
    
    DRReturn initIcoSphere(GLuint subdivide = 0);
    
private:
    DRGeometrieIcoSphere(const DRGeometrieIcoSphere& orig) {LOG_WARNING("Not exist");}
    
    struct IcoSphereFace
    {
        IcoSphereFace* mNeighbors[3];
        GLuint         mIndices[3];
    };
    
    IcoSphereFace* addNewFace(GLuint index1, GLuint index2, GLuint index3); 
    IcoSphereFace* addNewFace(GLuint index[3]); 
    DRReturn addNewFacesAtBorder();
    DRReturn grabIndicesFromFaces();
    
    
    // neue Faces (Rand) stehen am Anfang der Liste (start bei top, bzw freeCount)
    DRMemoryList<IcoSphereFace>* mFaceMemoryList;
    //
    std::list<IcoSphereFace*>           mBorderFaces;

};
//*/
#endif	/* __DR_ENGINE_GEOMETRIE_ICO_SPHERE_H */

