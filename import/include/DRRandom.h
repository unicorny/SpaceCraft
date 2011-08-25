/* 
 * File:   DRRandom.h
 * Author: dario
 *
 * Created on 25. August 2011, 13:28
 */

#ifndef DR_ENGINE_RANDOM_H
#define	DR_ENGINE_RANDOM_H


class ENGINE_API DRRandom
{
public:
    DRRandom() {/*srand(time(NULL));*/}
    
    static u64 r64() { return rand();}// + ((long long)rand()) << 32;}
    static DRReal rReal(DRReal fMax, DRReal fMin)
    {
            return fMin + (fMax - fMin) * ((DRReal)(rand() % 10001) / 10000.0f);
    }
    static DRVector3 rVector3(DRReal maxLengthSq)
    {
            DRVector3 ret;
            ret.x = rReal(maxLengthSq/2.0f, -maxLengthSq/2.0f);
            ret.y = rReal((maxLengthSq - ret.x)/1.5f, -((maxLengthSq - ret.x)/1.5f));
            ret.z = rReal(maxLengthSq - ret.x - ret.y, -(maxLengthSq -ret.x - ret.y));
            return ret;
    }
    static void seed(GLuint seed) {srand(seed);}
private:
};


#endif	/* DRRANDOM_H */

