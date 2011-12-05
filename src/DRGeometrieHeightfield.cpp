#include "main.h"


DRGeometrieHeightfield::DRGeometrieHeightfield(double spherical/* = 0.0f*/)
: mHeightValues(NULL), mSphericalRadius(spherical)
{
    
}

DRGeometrieHeightfield::~DRGeometrieHeightfield()
{
    mHeightValues = NULL;
}

DRReturn DRGeometrieHeightfield::initHeightfield(DRVector3 edgePoints[4], 
                                                 u32 gridSize, DRHeightValueStorage* heightValues,
                                                 bool normals/* = true*/, bool color/* = true*/, u8 textureCount/* = 0*/)
{
    mHeightValues = heightValues;
    //! memory allocation
    // size = gridSize
    // vertexCount = 4 +(size-1)*2 + (size-1)*2 + (size-1)*(size-1)*1 => 2*size + size*size + 1
    u32 vertexCount = 2*gridSize + gridSize*gridSize +1;
    // indexCount = (size-1) * (4 +(size-1)*2) + 2*size+2 => 2*size + 2*(size*size)
    u32 indexCount = 2*(gridSize*gridSize)+2*gridSize;
    //if(init(vertexCount, indexCount, textureCount, color, normals))
    if(init(vertexCount, indexCount, 0, color, false))
        LOG_ERROR("no memory allocatet for geometrie!", DR_ERROR);
    
    DRVector3 xVectorPart = (edgePoints[1]-edgePoints[0])/gridSize;
    DRVector3 yVectorPart = (edgePoints[2]-edgePoints[0])/gridSize;
    
    for(u32 j = 1; j < gridSize+1; j++)
    {
        for(u32 i = 0; i < gridSize+1; i++)
        {
            mIndices[mIndexCount++] = mVertexCount;
            mVertices[mVertexCount++] = edgePoints[0] + xVectorPart*i + yVectorPart*j;
            if(j == 1)
            {
                mIndices[mIndexCount++] = mVertexCount;
                mVertices[mVertexCount++] = edgePoints[0] + xVectorPart*i;
            }
            else if(j == 2)
                mIndices[mIndexCount++] = i*2;
            else
                mIndices[mIndexCount++] = (gridSize*2+2)+i+(j-3)*(gridSize+1);
        }
    }
    /*mIndices[mIndexCount++] = mVertexCount;
    mVertices[mVertexCount++] = edgePoints[0] + xVectorPart*0 + yVectorPart*3;
    mIndices[mIndexCount++] = 101-33;
    
    mIndices[mIndexCount++] = mVertexCount;
    mVertices[mVertexCount++] = edgePoints[0] + xVectorPart*1 + yVectorPart*3;
    mIndices[mIndexCount++] = 102-33;
     * */
    
    /*mVertices[mVertexCount++] = edgePoints[1];
    mVertices[mVertexCount++] = edgePoints[2];
    mVertices[mVertexCount++] = edgePoints[3];
    //*/
    
    DRLog.writeToLog("vertexCount: %d, indexCount: %d", mVertexCount, mIndexCount);
    double* radien = NULL;
    double* relRadien = NULL;
    u32     radienCount = 0;
    //mSphericalRadius /= 2.0;
    if(mSphericalRadius)
    {
     
        radienCount = (u32)ceil((float)gridSize/2.0f);
        radien = new double[radienCount];
        relRadien = new double[radienCount];
        printf("radienCount: %d\n", radienCount);
        double radiusPart = mSphericalRadius/radienCount;
        double relRadiusPart = edgePoints[0].length()/radienCount;
        for(u32 i = 0; i < radienCount; i++)
        {
            radien[i] = radiusPart*(double)(i+1);
            relRadien[i] = relRadiusPart*(double)(i+1);
            printf("radien(%d): %f, relRadien: %f\n", i, radien[i], relRadien[i]);
        }        
    }
    for(u32 i = 0; i < mVertexCount; i++)
    {
        if(mHeightValues)
        {
            DRVector3 norm = mVertices[i];
            mColors[i] = mHeightValues->getColorValue(mHeightValues->getHeightValue(norm));        
        }
        
        if(mSphericalRadius)
        {
            float length = mVertices[i].length();
            u32 radienIndex = 0;
            for(; radienIndex < radienCount-1; radienIndex++)
                if(relRadien[radienIndex] > length) break;
            printf("lenght: %f, radius(%d): %f, relRadien: %f\n", length, radienIndex, radien[radienIndex], relRadien[radienIndex]);
            if(radienIndex)
            {
                mVertices[i] = (mVertices[i] / length)*relRadien[radienIndex];// * radien[radienIndex];
                mVertices[i].z += mSphericalRadius - sqrt(mSphericalRadius - radien[radienIndex]*radien[radienIndex]);
            }
            else
                mVertices[i].z += mSphericalRadius - sqrt(mSphericalRadius - length*length);
                
          //  mVertices[i] = mVertices[i].normalize();
         //   printf("size(%d): %f\n", i, mVertices[i].length());
        }
    }
    DR_SAVE_DELETE_ARRAY(radien);
    DR_SAVE_DELETE_ARRAY(relRadien);
    radienCount = 0;
    
    setRenderMode(GL_TRIANGLE_STRIP);   
    copyDataToVertexBuffer(GL_STATIC_DRAW_ARB, true);
    
    return DR_OK;
}