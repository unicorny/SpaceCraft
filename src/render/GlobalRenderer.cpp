#include "GlobalRenderer.h"

GlobalRenderer::GlobalRenderer()
: m_bInitialized(false), mQuadratic(NULL), mFrameBufferID(0),
  mTextureRenderStepSize(0), mTextureRenderMaxResolution(0),
  mGrafikMemTexture(0), mGrafikMemGeometrie(0)
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
        
    DRIni cfg(configFilename);
    mTextureRenderStepSize = cfg.getInt("RenderToTexte", "StepSize");
    mTextureRenderMaxResolution = cfg.getInt("RenderToTexte", "Resolution");
    
    m_bInitialized= true;
    return DR_OK;
}

void GlobalRenderer::addRenderTask(RenderInStepsToTexturePtr newRenderTask, bool preview/* = false*/)
{
    if(preview)
        mPreviewRenderTasks.push_back(newRenderTask);
    else
        mRenderTasks.push_back(newRenderTask);
}
void GlobalRenderer::removeRenderTask(RenderInStepsToTexturePtr renderTaskToDelete)
{
    mDeleted.addByHash(reinterpret_cast<DHASH>(renderTaskToDelete.getResourcePtrHolder()->mResource), reinterpret_cast<void*>(1));
}

DRReturn GlobalRenderer::renderTaskFromQueue(std::list<RenderInStepsToTexturePtr>* list)
{
    list->sort();
    //remove already deleted objects (which can be find in mDeleted List)
    while(!list->empty() && 
           mDeleted.getNItems() > 0 &&  
           mDeleted.findByHash(reinterpret_cast<DHASH>(list->front().getResourcePtrHolder()->mResource)))
    {
        mDeleted.removeByHash(reinterpret_cast<DHASH>(list->front().getResourcePtrHolder()->mResource));
        list->pop_front();
    }
    RenderInStepsToTexturePtr current;
    //procceed list
    if(!list->empty())
    {
        current = list->front();
		DHASH hash = reinterpret_cast<DHASH>(list->front().getResourcePtrHolder()->mResource);
        
        if(setupFrameBuffer(current->getTextur())) //setup framebuffer with new texture
            LOG_ERROR("Fehler bei setupFrameBuffer", DR_ERROR);
        if(current->step()) LOG_ERROR("Fehler bei Step", DR_ERROR);
    
        if(current->isFinished())
            list->pop_front(); //remove finished Task
    }
    return DR_OK;
}

DRReturn GlobalRenderer::renderTasks()
{
    Uint32 start = SDL_GetTicks();
    static bool bswitch = true;
    
    // procceed mRenderTasks
    if(mPreviewRenderTasks.empty())
    {
        if(renderTaskFromQueue(&mRenderTasks))
            LOG_ERROR("Fehler bei mRenderTasks", DR_ERROR);
    }
    else
    {
        if(bswitch)
        {
            // procceed mPreviewRenderTasks
           if(renderTaskFromQueue(&mPreviewRenderTasks)) 
                LOG_ERROR("Fehler bei mPreviewRenderTasks", DR_ERROR);
        }
        else
        {
            // procceed mRenderTasks
            if(renderTaskFromQueue(&mRenderTasks))
                LOG_ERROR("Fehler bei mRenderTasks", DR_ERROR);
        }
        bswitch = !bswitch;             
    }
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);   
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    
    //printf("\r time used: %f", static_cast<float>(SDL_GetTicks()-start)/1000.0f);
    return DR_OK;
}

DRReturn GlobalRenderer::setupFrameBuffer(DRTexturePtr texture)
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBufferID);
    //create texture
    //bind to the new texture ID
    texture->bind();    
    
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D, texture->getId(), 0);
    GLenum ret = GL_FRAMEBUFFER_COMPLETE_EXT;
    ret = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(ret != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        DRLog.writeToLog("Fehler bei Check Framebuffer Status: %s", getFrameBufferEnumName(ret));
        LOG_ERROR("Fehler bei setupFrameBuffer", DR_ERROR);
    }
   // glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    return DR_OK;
}


void GlobalRenderer::exit()
{
    if(mFrameBufferID)
        glDeleteFramebuffersEXT(1, &mFrameBufferID);
    mFrameBufferID = 0;
    gluDeleteQuadric(mQuadratic); 
	if(m_bInitialized)
	{
		DRLog.writeToLog("[GlobalRenderer::exit] geo memory by exit: %f MByte, texture memory by exit: %f MByte",
			static_cast<double>(mGrafikMemGeometrie)/(1024.0*1024.0),
			static_cast<double>(mGrafikMemTexture)/(1024.0*1024.0));
	}
	mRenderTasks.clear();
	mPreviewRenderTasks.clear();
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