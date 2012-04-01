#ifndef __SC_SEKTOR_
#define __SC_SEKTOR_

#include "Vector3Unit.h"
#include "Camera.h"
#include "RenderSektor.h"

struct SektorID
{
    SektorID(u64 id) : id(id) {}
    SektorID(s16 _x, s16 _y, s16 _z): x(_x), y(_y), z(_z), count(0) {}
    SektorID(DRVector3 id): x(static_cast<short>(id.x*1000.0f)),
                            y(static_cast<short>(id.y*1000.0f)), 
                            z(static_cast<short>(id.z*1000.0f)) {}
    
    bool operator() (const SektorID& x, const SektorID& y) const {
        return x.id<y.id;
    }
    operator u64() {return id;}
    operator DRVector3() const {return DRVector3(x, y, z)/1000.0f;}
    
    // multipliziert x, y und z ccordinate with scalar
    __inline__ SektorID const operator *(short scalar) {return SektorID(x*scalar, y*scalar, z*scalar);}
    __inline__ SektorID const operator /(short scalar) {return SektorID(x/scalar, y/scalar, z/scalar);}
    
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
    SEKTOR_ROOT = 1,
    SUPER_GALAXIE_CLUSTER = 2,    
    SUPER_GALAXIE = 3,
    GALAXIE_CLUSTER = 4,
    GALAXIE = 5,
    STAR_CLUSTER = 6,
    SOLAR_SYSTEM = 7,
    STELLAR_BODY = 8,
    PLANET = 9,
    SUB_PLANET = 10,
    SUB_PATCH_PLANET = 11
};

/*!
 * \brief ein Sektor ist ein Teil der Welt
 * 
 * class Sektor
 * ein Sektor ist ein Teil der Welt, mit einer bestimmten Größe.. ein bestimmter Zoomausschnitt
 * ein Sektor kann wiederum aus vielen unter-Sektoren bestehen
 * bei Sektoren wie Galaxien haben die Unter-Sektoren beliebige ids
 * bei Sektoren mit hoher Materie-Dichte (wie PLaneten, Planetenteile) beschreibt die SektorID auch die Position
 * im Grid, in dem sich alle Sektoren der gleichen Ebene befinden
 * Sektoren sind hierachisch aufgebaut
 * => : enthält 
 * 1 => 2, 2 => 3, 3 => 4, 4 => 6 usw.
 * contains other sektors 
 * can be a stellar body (like a planet, etc), a star-system, a galaxie,
 * a star-heap (sternenhaufen), a galaxie-heap,
 * a super-galaxie-heap, a super-galaxie 
 */

//! TODO: Matrix-Stack ersetzen durch manuelle implementation, OpenGL Matrix Stack kann mindestens 32 matritzen halten

class Sektor
{
public:
    Sektor(Vector3Unit position, Unit radius, SektorID id, Sektor* parent);
    virtual ~Sektor();
    
    /*! \brief render sektor and childs
     * 
     *  using transformation and render sektor,     *  
     */    
    virtual DRReturn render(float fTime, Camera* cam) = 0;
    
    /*! \brief call render for all parents and childs
     *
     * first traverse to root node and then call the childs rekursiv
     * \param fTime time since last frame for render purpose
     * \param cam pointer to camera for correct positioning and culling
     * \param rootRendered set to false (used intern)
     */
    DRReturn renderAll(float fTime, Camera* cam, bool rootRendered = false);
    /*! \brief move sektor
     * 
     * check if child are visible, and move (logical, physical)
     * sektors longer not visible, will be deleted
     * generate all sektors, which are now visible,
     * 
     */
    virtual DRReturn move(float fTime, Camera* cam);
    
    /*! \brief call move for all parents and childs
     *
     * first traverse to root node and then call the childs rekursiv
     * \param fTime time since last frame for move purpose
     * \param cam pointer to camera for correct positioning and culling
     * \param rootMoved set to false (used intern)
     */
    DRReturn moveAll(float fTime, Camera* cam, bool rootMoved = false);
    
    // is the position inside the current sektor/ is the current sektor visible from that position
    virtual bool isObjectInSektor(Vector3Unit positionInSektor);
    
    static const char* getSektorTypeName(SektorType type);
    virtual void printTypeInfos(const char* name);
    __inline__ SektorType getType() const {return mType;} 
    __inline__ SektorID getID() const {return mID;}
    
    //! inline getter and setter
    __inline__ RenderSektor* getRenderer() const {return mRenderer;}
    
    __inline__ Vector3Unit getPosition() const {return mSektorPosition;}
    __inline__ const DRMatrix& getMatrix() {return mMatrix;}
    __inline__ Unit getRadius() const {return mRadius;}
    __inline__ void setParent(Sektor* parent) {mParent = parent;}
    __inline__ Sektor* getParent() const {return mParent;}
    virtual Sektor* getChild(SektorID childID) {if(mChilds.find(childID) != mChilds.end()) return mChilds[childID]; return NULL;}
        
    
    Sektor* getSektorByPath(std::vector<SektorID>& path, int thisIndex = 0);
    
    //! \brief fill a vector with all sektorID
    //!
    //! begin with root sektor (place zero)
    void getSektorPath(std::vector<SektorID>& storage) const;
    
    DRString getSektorPathName() const;
    
    __inline__ const Vector3Unit& getCameraPosition() {return mLastRelativeCameraPosition;}
    //! Observer
    void addObserver(DHASH hash, Observer* data);
    void removeObserver(DHASH hash);
    // weder sektor noch einer der childs hat noch einen Observer
    //! \return true if no observer exist and sektor can be delete
    bool hasObserver();
    
protected:    
    
    //! remove childs where mIdleSeconds is greater than idleThreshold and which haven't an observer
    virtual void removeInactiveChilds(double idleThreshold = 1.0);
    void updateCameraSektor(Camera* cam);
    
    DRReturn callForChilds(DRReturn (*callbackFunction)(Sektor* sektor, void* data), void* data);
    
    void setSektorSeed();
    
    //! id des sektors, gleichzeitig der seed
    SektorID            mID;
    SektorType          mType;
    //! Position of sektor inside the parent
    Vector3Unit         mSektorPosition; 
    //! last known camera position in sektor space
    Vector3Unit         mLastRelativeCameraPosition;
    //! die größe des Sektors, oder der Sektor-Einschließenden Kugel
    Unit                mRadius;
    //! Pointer at the parent-sektor
    Sektor*             mParent;
    
    //! renderer for this sektor
    RenderSektor*       mRenderer;

    //! matrix for this sektor, contain rotation, translation and scaling of this sector
    DRMatrix		mMatrix;
    
    
    //! seconds since last use (visible for camera) 
    float              mIdleSeconds;
    //! seconds since last rendered, but visible
    float              mNotRenderSeconds;              
    
    std::map<u64, Sektor*> mChilds;
    typedef std::pair<u64, Sektor*> SEKTOR_ENTRY;
    
    std::map<DHASH, Observer*> mObserver;
    typedef std::pair<DHASH, Observer*> SEKTOR_OBSERVER;
    
    std::vector<SektorID> mSektorPath;
    
private:
    
};

#endif //__SC_SEKTOR_