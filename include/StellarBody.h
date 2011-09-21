/* 
 * File:   StellarBody.h
 * Author: Dario
 *
 * Created on 20. September 2011, 19:10
 * Modell Class, Parent class for planets, stars, asteroids, etc.
 */

#ifndef __SPACE_CRAFT_STELLAR_BODY_H
#define	__SPACE_CRAFT_STELLAR_BODY_H

class Sektor;

class StellarBody
{
public:
    StellarBody(Unit radius, Vector3Unit position, int seed = 0, Sektor* parent = NULL);
    virtual ~StellarBody();
    
    DRReturn render(float fTime);
    
protected:
    Sektor* mParent;
    Unit mRadius;
    Vector3Unit mPosition;
    int mSeed;
    
    //not to save
    DRGeometrieIcoSphere mGeometrie;
};


#endif	/* __SPACE_CRAFT_STELLAR_BODY_H */

