#ifndef __DR_TEXTUR__
#define __DR_TEXTUR__

class ENGINE_API DRTextur
{
public:
    DRTextur();
    DRTextur(const char* filename, bool keepImage = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    virtual ~DRTextur();

    DRReturn load(const char* filename, bool keepImage = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    void unload();
    
    bool isLoaded() {return mSucessfullLoaded;}

    DRReturn bind();
    
    __inline__ u32 getWidth() {if(mParent) return mParent->getWidth(); LOG_ERROR("parent is zero, set keepImage to true!", -1);}
    __inline__ u32 getHeight() {if(mParent) return mParent->getHeight(); LOG_ERROR("parent is zero, set keepImage to true!", -1);}

protected:
    DRIImage* mParent;
    unsigned mTexturID;
    
    bool mSucessfullLoaded;

};

#endif //__DR_TEXTUR__
