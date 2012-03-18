#ifndef __DR_TEXTURE_MANAGER__
#define __DR_TEXTURE_MANAGER__

#include "SDL/SDL_thread.h"
#include "Texture.h"

class DRTextureManager
{
    friend class Texture;
public:
    ~DRTextureManager() {if(isInitialized()) exit();};

    // Singleton-Daten
    static DRTextureManager&		Instance();
    inline static DRTextureManager& getSingleton() {return Instance();};
    inline static DRTextureManager* getSingletonPtr() {return &Instance();};
    static bool	isInitialized()	{return Instance().mInitalized;};
    
    DRReturn init();
    
    void exit();
    //! lädt oder return instance auf Textur
    TexturePtr getTexture(const char* filename, bool loadAsynchron = false, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    TexturePtr getTexture(DRVector2i size, GLuint format, GLubyte* data = NULL, GLint dataSize = 0);
    
    //! schaut nach ob solche eine Texture in der Liste steckt, wenn nicht wird eine neue erstellt
    GLuint    getGLTextureMemory(GLuint width, GLuint height, GLuint format);
    //! packt die Textur in die Liste, falls noch jemand den Speicher benötigt
    void      freeTexture(GLuint textureID);

    
    void      addAsynchronTextureSaveTask(TexturePtr texture);
    
    // update timeout, release lange nicht verwendete Texturen
    DRReturn move(float fTime);     
    
    void test();
    
    __inline__ GLuint getGrafikMemoryTexture() const {return mGrafikMemTexture;} 
private:
    DRTextureManager();
    __inline__ void addGrafikMemTexture(GLuint addValue) {mGrafikMemTexture += addValue;}
    __inline__ void removeGrafikMemTexture(GLuint subtractValue) {mGrafikMemTexture -= subtractValue;}
    GLuint _getTexture(DRVector2i size, GLuint format);

    void      addAsynchronTextureLoadTask(TexturePtr texture);
    static int asynchronTextureLoadThread(void* data);
    
    //! daten für alte Einträge, dessen Speicher noch Verwendung finden könnte
    struct TextureMemoryEntry
    {
        TextureMemoryEntry(DRVector2i _size, GLint _format)
                : textureID(0), size(_size), format(_format) {}
        TextureMemoryEntry() 
                : textureID(0), size(0, 0), format(0) {}
        void print() {printf("width: %d, height: %d, format: %d", size.x, size.y, format);}
        void clear();
        GLuint textureID;
        DRVector2i size;
        GLint format;
        float  timeout; //Speicher wird freigegeben, wenn null erreicht,  0 kein timeout
    };
    
    DHASH makeTextureHash(const char* filename, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    DHASH makeTextureHash(const TextureMemoryEntry &entry);   
    
    //DRHashList mTextureEntrys;
    std::map<DHASH, TexturePtr>                         mTextureEntrys;
    typedef std::pair<DHASH, TexturePtr>                DR_TEXTURE_ENTRY;
    typedef std::map<DHASH, TexturePtr>::iterator       DR_TEXTURE_ENTRY_ITERATOR;
    std::multimap<DHASH, TextureMemoryEntry>            mTextureMemoryEntrys;
    typedef std::pair<DHASH, TextureMemoryEntry>        DR_TEXTURE_MEMORY_ENTRY;
    bool                                                mInitalized;
    GLuint						mGrafikMemTexture;

    //! stuff for asynchron texture load and save
    std::queue<TexturePtr> mAsynchronLoadTextures;
    std::queue<TexturePtr> mLoadedAsynchronLoadTextures;
    std::queue<TexturePtr> mAsynchronSaveTextures;
    std::queue<TexturePtr> mAsynchronReadyToSaveTextures;
    std::queue<TexturePtr> mEmpty;
    SDL_mutex*		   mTextureLoadMutex;
    SDL_Thread*		   mTextureLoadThread;
    SDL_cond*		   mTextureLoadCondition;
    SDL_sem*		   mTextureLoadSemaphore;
};

#endif //__DR_TEXTURE_MANAGER__