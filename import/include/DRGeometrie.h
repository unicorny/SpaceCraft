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
 * File:   DRGeometrie.j
 * Author: dario
 *
 * Created on 25. August 2011, 12:01
 */

#ifndef __DR_ENGINE2_GEOMETRIE__
#define	__DR_ENGINE2_GEOMETRIE__

class ENGINE2_API DRGeometrie : public DRIResource
{
public:
    DRGeometrie();
    virtual ~DRGeometrie();
    
    /*! \brief malloc memory for geometrie
     * 
     * reserviert Speicher für die Geometrie
     * falls noch Speicher reserviert war, wird dieser freigegeben, und alle Daten gelöscht!
     * \param vertexCount für wie viel Vertices Speicher reserviert werden soll (Vector3, Color und TextureCoords)
     * \param indexCount für wie viele Indices Speicher reserviert werden soll
     * \param textureCount wie viele Textur-Koordinaten-Arrays benötigt werden (Anzahl der Texturen, Multitexturing)
     * \param color bei false wird kein Speicher für das color-array reserviert
     * \return DR_OK wenn alles okay ist, oder DR_ERROR bei Fehler 
     * */
    DRReturn init(u32 vertexCount, u32 indexCount = 0, u32 textureCount = 0, bool color = false, bool normals = false);
    
    
    //! \param clearData if set to true, after copying data to vertexbuffer, local copy will be deleted
    DRReturn copyDataToVertexBuffer(GLenum usage = GL_STATIC_DRAW_ARB, bool clearData = false);
    DRReturn updateIndexDataIntoVertexBuffer(int from = 0, int to = 0, GLenum usage = GL_STATIC_DRAW_ARB);
    
    void setRenderMode(GLenum renderMode) {mRenderMode = renderMode;}
    DRReturn render();
    
    __inline__ DRVector3* getVertexPointer() {return mVertices;}
    __inline__ DRColor*   getColorPointer()  {return mColors;}
    __inline__ GLuint*    getIndexPointer()  {return mIndices;}
    __inline__ DRVector2* getTextureCoordsPointer(u32 index) {if(index >= mNumTextureCoords) return NULL; 
                                                              if(!mTextureCoords) return NULL;
                                                              return mTextureCoords[index];}
    
    __inline__ u32    getVertexCount() {return mVertexCount;}
    
    void clearData();
    void releaseVertexBuffer();
    
    virtual const char* getResourceType() const {return "Geometrie";}
    virtual bool less_than(DRIResource& geo) const
    {
        return false;
    }
    
protected:
    DRVector3*  mVertices;
    DRVector3*  mNormals;
    GLuint*     mIndices;
    DRColor*    mColors;
    DRVector2** mTextureCoords;
    u32         mVertexCount;
    u32         mIndexCount;
    u32         mMaxVertexCount;
    u32         mMaxIndexCount;
    u32         mNumTextureCoords;
    
    GLuint      mVertexBufferObjects[5];
    bool        mRenderVertexBuffer;
    
    GLenum      mRenderMode;

private:
        DRReturn initVertexBuffer();
	void lock() {if(mBufferMutex) SDL_mutexP(mBufferMutex);}
	void unlock() {if(mBufferMutex) SDL_mutexV(mBufferMutex);}
	SDL_mutex*	mBufferMutex;
       
};

typedef DRResourcePtr<DRGeometrie> DRGeometriePtr;

#endif	/* __DR_ENGINE2_GEOMETRIE__ */

