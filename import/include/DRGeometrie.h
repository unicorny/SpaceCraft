/* 
 * File:   DRGeometrie.j
 * Author: dario
 *
 * Created on 25. August 2011, 12:01
 */

#ifndef _DR_ENGINE_GEOMETRIE_h
#define	_DR_ENGINE_GEOMETRIE_h

class ENGINE_API DRGeometrie
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
    DRReturn initVertexBuffer();
    
    //! \param clearData if set to true, after copying data to vertexbuffer, local copy will be deleted
    DRReturn copyDataToVertexBuffer(GLenum usage = GL_STATIC_DRAW_ARB, bool clearData = false);
    DRReturn updateIndexDataIntoVertexBuffer(int from = 0, int to = 0, GLenum usage = GL_STATIC_DRAW_ARB);
    
    void setRenderMode(GLenum renderMode) {mRenderMode = renderMode;}
    DRReturn render();
    
    DRVector3* getVertexPointer() {return mVertices;}
    DRColor*   getColorPointer()  {return mColors;}
    GLuint*    getIndexPointer()  {return mIndices;}
    
    u32         getVertexCount() {return mVertexCount;}
    
    void clearData();
    void releaseVertexBuffer();
    
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
	void lock() {if(mBufferMutex) SDL_mutexP(mBufferMutex);}
	void unlock() {if(mBufferMutex) SDL_mutexV(mBufferMutex);}
	SDL_mutex*	mBufferMutex;
       
};


#endif	/* _DR_ENGINE_GEOMETRIE_h */

