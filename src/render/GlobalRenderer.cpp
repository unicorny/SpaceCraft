#include "main.h"


GlobalRenderer::GlobalRenderer()
: m_bInitialized(false), mQuadratic(NULL), mPlanetGen(NULL), mFrameBufferID(0),
  mTextureRenderStepSize(0), mTextureRenderMaxResolution(0)
{
    
}

GlobalRenderer& GlobalRenderer::Instance()
{
    static GlobalRenderer TheOneAndOnly;
    return TheOneAndOnly;
}

DRReturn GlobalRenderer::init(const char* configFilename)
{
    mQuadratic = gluNewQuadric(); // erzeugt einen Zeiger auf das Quadric Obkect ( NEU ) 
    if(!mQuadratic) LOG_ERROR("error by call gluNewQuadric", DR_ERROR);
    gluQuadricNormals(mQuadratic, GLU_SMOOTH); // erzeugt Normalen ( NEU ) 
    gluQuadricTexture(mQuadratic, GL_TRUE); // erzeugt Textur Koordinaten ( NEU )
    
    //render to texture
     glGenFramebuffersEXT(1, &mFrameBufferID);
    
    if(mPlanetShader.init("data/shader/planet2.vert", "data/shader/planet2.frag"))
        LOG_ERROR("Fehler beim laden des Shaders", DR_ERROR);
    
    DRIni cfg(configFilename);
    mTextureRenderStepSize = cfg.getInt("RenderToTexte", "StepSize");
    mTextureRenderMaxResolution = cfg.getInt("RenderToTexte", "Resolution");
   // mPlanetGen = new GenerateNoisePlanet;
    
    m_bInitialized= true;
    return DR_OK;
}

void GlobalRenderer::addRenderTask(RenderInStepsToTexture* newRenderTask)
{
    if(mRenderTasks.empty()) 
    {
        if(setupFrameBuffer(newRenderTask->getTextureID()))
            LOG_ERROR_VOID("Fehler bei setupFrameBuffer"); 
    }
    mRenderTasks.push(newRenderTask);
}

DRReturn GlobalRenderer::renderTasks()
{
    Uint32 start = SDL_GetTicks();
    
	if(mRenderTasks.empty()) return DR_OK;
    RenderInStepsToTexture* current = mRenderTasks.front();
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBufferID);   
    if(current->step()) LOG_ERROR("Fehler bei Step a RenderTask", DR_ERROR);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);   
    
    if(current->isFinished())
    {
        mRenderTasks.pop(); //remove finished Task
        if(mRenderTasks.empty()) return DR_OK; //return if no taks is left
        
        current = mRenderTasks.front(); //get new task
        if(setupFrameBuffer(current->getTextureID())) //setup framebuffer with new texture
            LOG_ERROR("Fehler bei setupFrameBuffer", DR_ERROR);
    }
    
    printf("\r time used: %f", static_cast<float>(SDL_GetTicks()-start)/1000.0f);
    return DR_OK;
}

DRReturn GlobalRenderer::setupFrameBuffer(GLuint textureID)
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBufferID);
    //create texture
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, textureID, 0);
    GLenum ret = GL_FRAMEBUFFER_COMPLETE_EXT;
    ret = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(ret != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        DRLog.writeToLog("Fehler bei Check Framebuffer Status: %s", getFrameBufferEnumName(ret));
        LOG_ERROR("Fehler bei setupFrameBuffer", DR_ERROR);
    }
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    return DR_OK;
}


void GlobalRenderer::exit()
{
    if(mFrameBufferID)
        glDeleteFramebuffersEXT(1, &mFrameBufferID);
    mFrameBufferID = 0;
    gluDeleteQuadric(mQuadratic); 
    DR_SAVE_DELETE(mPlanetGen);
    m_bInitialized = false;    
}

const char* GlobalRenderer::getFrameBufferEnumName(GLenum name)
{
    switch(name)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT: return "GL_FRAMEBUFFER_COMPLETE_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT"; 
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT";
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT: return "GL_FRAMEBUFFER_UNSUPPORTED_EXT";
    default: return "-unknown enum-";
    }
    return "-error-";
}