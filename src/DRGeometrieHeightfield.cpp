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
    if(DRGeometrie::init(vertexCount, indexCount, 0, color, false))
        LOG_ERROR("no memory allocated for geometrie!", DR_ERROR);
    
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

    
    DRLog.writeToLog("vertexCount: %d, indexCount: %d", mVertexCount, mIndexCount);
    DRLog.writeVector3ToLog(mSphericalCenter, "Spherical Center");    
    for(u32 i = 0; i < mVertexCount; i++)
    {
        if(mSphericalCenter != DRVector3(0.0f))
        {
            DRVector3 dir = DRVector3(mVertices[i]-mSphericalCenter);
            float length = dir.length();
            //printf("length: %f, newSize: %f, vertex: %d: %f, %f, %f", length, 1.0f-length, i, mVertices[i].x, mVertices[i].y, mVertices[i].z);
            //printf(" dir: %f, %f, %f ", dir.x, dir.y, dir.z);
            //mVertices[i] += (dir / length) * (1.0f-length);
            //printf(" newvertex: %f, %f, %f\n", mVertices[i].x, mVertices[i].y, mVertices[i].z);
            //mVertices[i] = dir.normalize();
            
            if(mHeightValues)
            {
                mColors[i] = mHeightValues->getColorValue(mHeightValues->getHeightValue(dir));        
            }
        }
        else   
        {
            if(mHeightValues)
            {
                mColors[i] = mHeightValues->getColorValue(mHeightValues->getHeightValue(mVertices[i]));        
            }
        }
    }
        
    setRenderMode(GL_TRIANGLE_STRIP);   
    copyDataToVertexBuffer(GL_STATIC_DRAW_ARB, true);
    
    return DR_OK;
}

DRReturn DRGeometrieHeightfield::init(int gridSize, DRVector3 edgePoints[4], GeometrieGridFormat format)
{
    //! memory allocation
    // size = gridSize
    // vertexCount = 4 +(size-1)*2 + (size-1)*2 + (size-1)*(size-1)*1 => 2*size + size*size + 1
    u32 vertexCount = 2*gridSize + gridSize*gridSize +1;
    // indexCount = (size-1) * (4 +(size-1)*2) + 2*size+2 => 2*size + 2*(size*size)
    u32 indexCount = 2*(gridSize*gridSize)+2*gridSize;
    //if(init(vertexCount, indexCount, textureCount, color, normals))
    if(DRGeometrie::init(vertexCount, indexCount, 0, false, false))
        LOG_ERROR("no memory allocated for geometrie!", DR_ERROR);
    
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
    
    DRLog.writeToLog("vertexCount: %d, indexCount: %d", mVertexCount, mIndexCount);
    
    setRenderMode(GL_TRIANGLE_STRIP);   
    copyDataToVertexBuffer(GL_STATIC_DRAW_ARB, true);  
    
    return DR_OK;
}