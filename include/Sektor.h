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
    SektorID(s16 _x, s16 _y, s16 _z): x(_x), y(_y), z(_z), count(0) {}
    
    bool operator() (const SektorID& x, const SektorID& y) const {return x.id<y.id;}
    operator u64() {return id;}
    
    union
    {
        struct
        {
            // x, y z beschreiben sektorposition im grid
            // count wird hochgezählt, falls mehr als ein sektor sich an der selben Grid-Position befindet
            s16 x;
            s16 y;
            s16 z;
            s16 count;
        };
        u64 id;
    };
};

enum SektorType
{
    SEKTOR_NONE = 0,
    ROOT = 1,
    SUPER_GALAXIE_CLUSTER = 2,    
    SUPER_GALAXIE = 3,
    GALAXIE_CLUSTER = 4,
    GALAXIE = 5,
    STAR_CLUSTER = 6,
    SOLAR_SYSTEM = 7,
    STELLAR_BODY = 8        
};

/*!
 * \brief ein Sektor ist ein Teil der Welt
 * 
 * class Sektor
 * ein Sektor ist ein Teil der Welt, mit einer bestimmten Größe.. ein bestimmter Zoomausschnitt
 * ein Sektor kann wiederum aus vielen unter-Sektoren bestehen
 * contains other sektors 
 * can be a stellar body (like a planet, etc), a star-system, a galaxie,
 * a star-heap (sternenhaufen), a galaxie-heap,
 * a super-galaxie-heap, a super-galaxie 
 */

class Sektor {
public:
    Sektor(Sektor* parent = NULL, SektorType type = SEKTOR_NONE, SektorID id = SektorID(0), int seed = 0, u64 serverID = 0);
    Sektor(const Sektor& orig);
    virtual ~Sektor();
    
    //! \brief set new stellar body
    //! 
    //! old stellar body will be deletet first
    void setStellarBody(StellarBody* newBody);
    void setSize(Vector3Unit size) {mSektorSize = size;}
    void setPosition(Vector3Unit position) {mSektorPosition = position;}
    void setServer(u64 serverID) {mServerID = serverID;}
    
    DRReturn addSektor(Sektor* newSektor);
    
    DRReturn save(DRFile* openFile);
    
    DRReturn render(float fTime, Camera* camera);
    DRReturn move(float fTime, Camera* camera);
    
    static const char* getSektorTypeName(SektorType type);
    std::list<u64>* getSektorPath(std::list<u64>* buffer);
    void setID(SektorID newID) {mID = newID;}
    SektorID getID() {return mID;}
    
    void seed() {DRRandom::seed(mSeed);}
    
private:
    StellarBody* mStellarBody;
    SektorID mID;
    SektorType mType;
    int mSeed;
    Sektor* mParent;
    
    // size of the sektor
    Vector3Unit mSektorSize;
    // zentrum des sektors, relativ zum Parent sektor
    Vector3Unit mSektorPosition; 
    
    std::map<u64, Sektor*> mChilds;
    typedef std::pair<u64, Sektor*> SEKTOR_ENTRY;
    
    // only for render, not for save, 
    u64 mServerID;
    //position of player
    Vector3Unit mLastPosition;
};

#endif	/* SEKTOR_H */

