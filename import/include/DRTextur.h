#ifndef __DR_TEXTUR__
#define __DR_TEXTUR__

class ENGINE_API DRTextur
{
public:
    DRTextur();
    DRTextur(DRIImage* loadedImage, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    DRTextur(const char* filename, bool keepImage = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    virtual ~DRTextur();

    DRReturn load(const char* filename, bool keepImage = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    DRReturn loadFromMemory(DRColor* colors, DRVector2 size, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    void unload(bool full = true);
    
    bool isLoaded() {return mSucessfullLoaded;}

    DRReturn bind();
    
    __inline__ u32 getWidth() {if(mParent) return mParent->getWidth(); LOG_ERROR("parent is zero, set keepImage to true!", -1);}
    __inline__ u32 getHeight() {if(mParent) return mParent->getHeight(); LOG_ERROR("parent is zero, set keepImage to true!", -1);}
    
    //! return texture ID and set it to zero
    __inline__ GLuint removeTexturID() {GLuint id = mTexturID; mTexturID = 0; return id;}
protected:
    DRReturn load(GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    
    DRIImage* mParent;
    unsigned mTexturID;
    
    bool mSucessfullLoaded;

};

#endif //__DR_TEXTUR__
