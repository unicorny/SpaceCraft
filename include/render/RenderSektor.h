#ifndef __SC_RENDER_SEKTOR__
#define __SC_RENDER_SEKTOR__

class RenderSektor
{
public:
    RenderSektor();
    virtual ~RenderSektor() {};
        
    virtual DRReturn render(float fTime, Camera* cam) = 0;
    __inline__ void setCurrentDetail(int detail) {mDetailLevel = detail;}
protected:    
    int                         mDetailLevel;
    
private:
};

#endif //__SC_RENDER_SEKTOR__