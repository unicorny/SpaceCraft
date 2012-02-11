/* 
 * File:   GlobalRenderer.h
 * Author: Dario
 *
 * Created on 7. Oktober 2011, 17:57
 */

#ifndef SPACE_CRAFT_GLOBAL_RENDERER_H
#define	SPACE_CRAFT_GLOBAL_RENDERER_H

class GlobalRenderer
{
public:
    ~GlobalRenderer() {if(isInitialized()) exit();};

    // Singleton-Daten
    static GlobalRenderer&		Instance();
    inline static GlobalRenderer& getSingleton() {return Instance();};
    inline static GlobalRenderer* getSingletonPtr() {return &Instance();};
    static bool	isInitialized()	{return Instance().m_bInitialized;};

    //init und exit
    DRReturn init(const char* configFilename);
    void exit();
    
    inline ShaderProgram* getPlanetShaderPtr() {return &mPlanetShader;}
    inline GenerateNoisePlanet* getGenerateNoisePlanet() {return mPlanetGen;}
    
    GLUquadricObj* getQuadric() {if(m_bInitialized) return mQuadratic; else return NULL;}
    
    //Config Details
    __inline__ GLuint getTextureRenderStepSize() {return mTextureRenderStepSize;}
    __inline__ GLuint getTextureRenderMaxResolution() {return mTextureRenderMaxResolution;}
    
    //! put task onto stack, call it if it is on top, until it is finished, than remove task from stack
    //! memory will not be touched!!
    void addRenderTask(RenderInStepsToTexture* newRenderTask, bool preview = false);
    
    // render current task
    DRReturn renderTasks();
    
private:
    GlobalRenderer();
    DRReturn setupFrameBuffer(GLuint textureID);
    static const char* getFrameBufferEnumName(GLenum name);
    
    bool				m_bInitialized;
    GLUquadricObj*                      mQuadratic; 
    GenerateNoisePlanet*                mPlanetGen;
    ShaderProgram                       mPlanetShader;
    
    //Render To texture
    GLuint                              mFrameBufferID;
    std::queue<RenderInStepsToTexture*> mRenderTasks;
    std::queue<RenderInStepsToTexture*> mPreviewRenderTasks;
    
    // Config
    GLuint                               mTextureRenderStepSize;
    GLuint                               mTextureRenderMaxResolution;
};


#endif	/* SPACE_CRAFT_GLOBAL_RENDERER_H */

