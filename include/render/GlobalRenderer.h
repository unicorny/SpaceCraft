/* 
 * File:   GlobalRenderer.h
 * Author: Dario
 *
 * Created on 7. Oktober 2011, 17:57
 */

#ifndef SPACE_CRAFT_GLOBAL_RENDERER_H
#define	SPACE_CRAFT_GLOBAL_RENDERER_H

#include "RenderInStepsToTexture.h"

//class RenderInStepsToTexture;

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
    
    GLUquadricObj* getQuadric() {if(m_bInitialized) return mQuadratic; else return NULL;}
    
    //Config Details
    __inline__ GLuint getTextureRenderStepSize() {return mTextureRenderStepSize;}
    __inline__ GLuint getTextureRenderMaxResolution() {return mTextureRenderMaxResolution;}
    __inline__ float  getTimeForInactiveChilds() {return mTimeForInactiveChilds;}
    __inline__ void   setTimeForInactiveChild(float time) {mTimeForInactiveChilds = time;}
    
    //! put task onto stack, call it if it is on top, until it is finished, than remove task from stack
    //! memory will not be touched!!
    void addRenderTask(RenderInStepsToTexturePtr newRenderTask, bool preview = false);
    void removeRenderTask(RenderInStepsToTexturePtr renderTaskToDelete);
    
    // render current task
    DRReturn renderTasks();

    const DRMatrix& getProjectionMatrix() {return mProjectionMatrix;}
    void setProjectionMatrix(const DRMatrix& projectionMatrix) {mProjectionMatrix = projectionMatrix;}

	__inline__ void addGrafikMemTexture(GLuint addValue) {mGrafikMemTexture += addValue;}
	__inline__ void addGrafikMemGeometrie(GLuint addValue) {mGrafikMemGeometrie += addValue;}
	__inline__ void removeGrafikMemTexture(GLuint subtractValue) {mGrafikMemTexture -= subtractValue;}
	__inline__ void removeGrafikMemGeometrie(GLuint subtractValue) {mGrafikMemGeometrie -= subtractValue;}
	__inline__ GLuint getGrafikMemoryTexture() {return mGrafikMemTexture;}
	__inline__ GLuint getGrafikMemoryGeometrie() {return mGrafikMemGeometrie;}
	__inline__ GLuint getGrafikMemory() {return mGrafikMemGeometrie+mGrafikMemTexture;}
    
private:
    GlobalRenderer();
    DRReturn setupFrameBuffer(DRTexturePtr texture);
    static const char* getFrameBufferEnumName(GLenum name);
    DRReturn renderTaskFromQueue(std::list<RenderInStepsToTexturePtr>* list);
    
    bool				m_bInitialized;
    GLUquadricObj*                      mQuadratic; 
    DRMatrix							mProjectionMatrix;
    
    //Render To texture
    GLuint                              mFrameBufferID;
    std::list<RenderInStepsToTexturePtr> mRenderTasks;
    std::list<RenderInStepsToTexturePtr> mPreviewRenderTasks;
    DRHashList                          mDeleted;
    
    // Config
    GLuint                               mTextureRenderStepSize;
    GLuint                               mTextureRenderMaxResolution;
    
    //dynamic config
    float                               mTimeForInactiveChilds;

	//statistic reserved memory in Bytes
	GLuint								mGrafikMemTexture;
	GLuint								mGrafikMemGeometrie;
};


#endif	/* SPACE_CRAFT_GLOBAL_RENDERER_H */

