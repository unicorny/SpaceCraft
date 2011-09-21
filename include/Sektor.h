/* 
 * File:   Sektor.h
 * Author: dario
 *
 * Created on 20. August 2011, 18:58
 */

#ifndef __SC_SEKTOR_H
#define	__SC_SEKTOR_H


struct SektorID
{
    SektorID(u64 id) : id(id) {}
    
    bool operator() (const SektorID& x, const SektorID& y) const {return x.id<y.id;}
    
    union
    {
        struct
        {
            s16 x;
            s16 y;
            s16 z;
            s16 empty;
        };
        u64 id;
    };
};


class Sektor {
public:
    Sektor(SektorID id = SektorID(0));
    Sektor(const Sektor& orig);
    virtual ~Sektor();
    
    void addStellarBody(StellarBody* newBody);
    DRReturn save(DRFile* openFile);
    
    DRReturn render(float fTime, Camera* camera);
    
private:
    std::list<StellarBody*> mStellarBodys;
    SektorID mID;
};

#endif	/* SEKTOR_H */

