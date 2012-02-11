#ifndef __SC_ROOT_SEKTOR__
#define __SC_ROOT_SEKTOR__

class RootSektor : public Sektor
{
public:
    RootSektor(SektorID id);
    virtual ~RootSektor();
    
    virtual DRReturn move(float fTime, Camera* cam);
    virtual DRReturn render(float fTime, Camera* cam);
    
    void addSektor(Sektor* sektor, SektorID id);
    
    // is the position inside the current sektor
    virtual bool isObjectInSektor(Vector3Unit positionInSektor) {return true;}
        
protected:
};

#endif //__SC_ROOT_SEKTOR__