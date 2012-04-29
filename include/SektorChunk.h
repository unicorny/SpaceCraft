/* 
 * File:   SektorChunk.h
 * Author: dario
 *
 * Created on 21. August 2011, 08:04
 */

#ifndef __SC_SEKTOR_CHUNK_H
#define	__SC_SEKTOR_CHUNK_H


class SektorChunk {
public:
    SektorChunk();
    SektorChunk(const SektorChunk& orig);
    virtual ~SektorChunk();
private:
    
    RenderBlock* mBlocks[16][16][16];

};

#endif	/* __SC_SEKTOR_CHUNK_H */

