/* 
 * File:   DRGeometrieIcoSphere.h
 * Author: dario
 *
 * Created on 26. August 2011, 08:59
 */

#ifndef _DR_GEOMETRIE_ICO_SPHERE_H
#define	_DR_GEOMETRIE_ICO_SPHERE_H

class DRGeometrieIcoSphere : public DRGeometrieSphere
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

#endif	/* _DR_GEOMETRIE_ICO_SPHERE_H */

