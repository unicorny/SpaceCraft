#ifndef __DR_TEXTURE_MANAGER__
#define __DR_TEXTURE_MANAGER__

#include "Sdl/SDL_thread.h"
#include "Texture.h"

class DRTextureManager
{
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
    DRTextur* getTexture(const char* filename, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    //! reduziert reference, bei null wird Textur gelöscht und OpenGL Texture in liste eingetragen
    void      releaseTexture(const char* filename, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    //! schaut nach ob solche eine Texture in der Liste steckt, wenn nicht wird eine neue erstellt
    GLuint    getGLTextureMemory(GLuint width, GLuint height, GLuint bpp, GLuint format);
    //! packt die Textur in die Liste, falls noch jemand den Speicher benötigt
    void      freeTexture(GLuint textureID);

	void	  addAsynchronTextureLoadTask(Texture* texture);
	void	  addAsynchronTextureSaveTask(Texture* texture);
    
    // update timeout, release lange nicht verwendete Texturen
    DRReturn move(float fTime);     
    
    void test();
    
    
private:
    DRTextureManager();
	static int asynchronTextureLoadThread(void* data);
    
    //! daten für alte Einträge, dessen Speicher noch Verwendung finden könnte
    struct TextureMemoryEntry
    {
        TextureMemoryEntry(GLint _width, GLint _height, GLint _bpp, GLint _format)
                : textureID(0), width(_width), height(_height), bpp(_bpp), format(_format) {}
        TextureMemoryEntry() 
                : textureID(0), width(0), height(0), bpp(0), format(0) {}
        void print() {printf("width: %d, height: %d, bpp:%d, format: %d", width, height, bpp, format);}
        GLuint textureID;
        GLint width;
        GLint height;
        GLint bpp;
        GLint format;
        float  timeout; //Speicher wird freigegeben, wenn null erreicht,  0 kein timeout
    };
    
    struct TextureEntry
    {
        DRTextur* texture;
        int referenzCounter;
    };
    
    DHASH makeTextureHash(const char* filename, GLint glMinFilter = GL_LINEAR, GLint glMagFilter = GL_LINEAR);
    DHASH makeTextureHash(const TextureMemoryEntry &entry);   
    
    DRHashList mTextureEntrys;
    std::multimap<DHASH, TextureMemoryEntry> mTextureMemoryEntrys;
    typedef std::pair<DHASH, TextureMemoryEntry> DR_TEXTURE_MEMORY_ENTRY;
    bool mInitalized;

	std::queue<Texture*> mAsynchronLoadTextures;
	std::queue<Texture*> mLoadedAsynchronLoadTextures;
	std::queue<Texture*> mAsynchronSaveTextures;
	std::queue<Texture*> mAsynchronReadyToSaveTextures;
	SDL_mutex*			mTextureLoadMutex;
	SDL_Thread*		    mTextureLoadThread;
	SDL_cond*			mTextureLoadCondition;
	SDL_sem*			mTextureLoadSemaphore;
};

#endif //__DR_TEXTURE_MANAGER__