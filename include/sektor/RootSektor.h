#ifndef __SC_ROOT_SEKTOR__
#define __SC_ROOT_SEKTOR__

class RootSektor : public Sektor
{
public:
    RootSektor();
    virtual ~RootSektor();
    
    virtual DRReturn move(float fTime, Camera* cam) {return DR_ERROR;}
    virtual DRReturn render(float fTime, Camera* cam) {return DR_ERROR;}
        
protected:
};

#endif //__SC_ROOT_SEKTOR__