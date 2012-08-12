/*/*************************************************************************
 *                                                                         *
 * Core, Core-Lib for my programs, Core doesn't need any libraries	   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski                           *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.                                                      *
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
 * File:   DRFrustumCulling.h
 * Author: Dario
 *
 * Created on 20. Oktober 2011, 21:39
 */

#ifndef __DR_CORE2_FRUSTUM_CULLING__
#define	__DR_CORE2_FRUSTUM_CULLING__

enum DRFrustumPosition
{
    OUTSIDE = -1,
    INTERSECT = 0,
    INSIDE = 1,
    FR_ERROR = 2,
    BEVORE = 3,
    BEHIND = 4
};

class CORE2_API DRFrustumCulling
{
public:
    // view = Kamera Matrix, projection = DRMatrix::view_frustum
    DRFrustumCulling(DRObjekt* camera, DRReal fovAngle, DRReal aspectRatio, DRReal zNearPlane, DRReal zFarPlane);
    
    // Camera Matrix wird automatisch geupdatet, wenn camera gesetzt wurde, wenn nicht, return DR_ERROR
    DRReturn updateMatritzen(DRReal fovAngle, DRReal aspectRatio, DRReal zNearPlane, DRReal zFarPlane);
    
    DRFrustumPosition isPointInFrustum(const DRVector3& point);
    DRFrustumPosition isSphereInFrustum(const DRVector3& middlePoint, DRReal radius);
    DRFrustumPosition isBoxInFrustum(const DRVector3& boxMin, const DRVector3& boxMax, const DRMatrix& transform = DRMatrix::identity());
private:
    
    DRFrustumPosition classifyBox(const DRVector3& boxMin, const DRVector3& boxMax, const DRMatrix& invTransform, int planeIndex);
    
    DRPlane   mViewPlanes[6];
    DRObjekt* mCamera;    
    DRReal    mNearPlane;
    DRReal    mFarPlane;
    DRReal    mAspectRatio;
    DRReal    mFovAngle;
    double    mFovAngleTangent;
    double    mSphereFactorX;
    
};

#endif	/* __DR_CORE2_FRUSTUM_CULLING__ */

