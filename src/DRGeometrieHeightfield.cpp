#include "main.h"


DRGeometrieHeightfield::DRGeometrieHeightfield(DRVector3 sphericalCenter/* = DRVector3(0.0f)*/)
: mHeightValues(NULL), mSphericalCenter(sphericalCenter)
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
    
    if(mSphericalCenter != DRVector3(0.0f))
    {
        for(int i = 0; i < 4; i++)
                edgePoints[0] -= mSphericalCenter.normalize();
    }
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
    DRLog.writeVector3ToLog(mSphericalCenter, "Spherical Center");
    float centerH = mSphericalCenter.length();
    printf("center h: %f\n", centerH);
    
    for(u32 i = 0; i < mVertexCount; i++)
    {
        if(mSphericalCenter != DRVector3(0.0f))
        {
            DRVector3 dir = DRVector3(mVertices[i]-mSphericalCenter);
            float length = dir.length();
            printf("length: %f, newSize: %f, vertex: %d: %f, %f, %f\n", length, 1.0f-length, i, mVertices[i].x, mVertices[i].y, mVertices[i].z);
            //mVertices[i] = mVertices[i]+ (dir / length);// * (length);
        }
        
        if(mHeightValues)
        {
            DRVector3 norm = mVertices[i];
            mColors[i] = mHeightValues->getColorValue(mHeightValues->getHeightValue(norm));        
        }
            
    }
        
    setRenderMode(GL_TRIANGLE_STRIP);   
    copyDataToVertexBuffer(GL_STATIC_DRAW_ARB, true);
    
    return DR_OK;
}