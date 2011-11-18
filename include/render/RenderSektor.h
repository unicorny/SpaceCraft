#ifndef __SC_RENDER_SEKTOR__
#define __SC_RENDER_SEKTOR__

class RenderSektor
{
public:
    RenderSektor() {};
    virtual ~RenderSektor() {};
    
    virtual DRReturn render(float fTime, Camera* cam) = 0;
protected:
private:
};

#endif //__SC_RENDER_SEKTOR__