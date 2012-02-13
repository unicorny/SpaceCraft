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
    __inline__ void setCurrentDetail(int detail) {mDetailLevel = detail;}
	__inline__ ShaderProgram* getShaderProgram() {return mShader;}
protected:    
    int                         mDetailLevel;
	ShaderProgram*				mShader;
    
private:
};

#endif //__SC_RENDER_SEKTOR__