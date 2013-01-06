#ifndef __SC_RENDER_SEKTOR__
#define __SC_RENDER_SEKTOR__

#include "ShaderProgram.h"

class Camera;

class RenderSektor
{
public:
    RenderSektor();
    virtual ~RenderSektor() {};
        
    virtual DRReturn render(float fTime, Camera* cam) = 0;
    __inline__ virtual bool isFinishLoading() = 0;
    __inline__ virtual bool isErrorOccured() = 0;
    __inline__ virtual bool isInitalized() = 0;
    
    __inline__ void setCurrentDetail(int detail) {mDetailLevel = detail;}
	__inline__ ShaderProgramPtr getShaderProgram() {return mShader;}
protected:    
    int                         mDetailLevel;
	ShaderProgramPtr				mShader;
    
private:
};

#endif //__SC_RENDER_SEKTOR__