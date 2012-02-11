/* 
 * File:   SektorChunk.h
 * Author: dario
 *
 * Created on 21. August 2011, 08:04
 */

#ifndef SEKTORCHUNK_H
#define	SEKTORCHUNK_H

//#include "RenderBlock.h"

class SektorChunk {
public:
    SektorChunk();
    SektorChunk(const SektorChunk& orig);
    virtual ~SektorChunk();
private:
    
    //RenderBlock* mBlocks[16][16][16];

};

#endif	/* SEKTORCHUNK_H */

