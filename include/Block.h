/* 
 * File:   Block.h
 * Author: dario
 *
 * Created on 20. August 2011, 23:52
 */

#ifndef BLOCK_H
#define	BLOCK_H

#include "main.h"

struct BlockName
{
    BlockName() : name(0) {}
    BlockName(const char* name) {
        size_t s = 4;
        if(strlen(name) < 4) s = strlen(name);
        memset(zeichen, 0, 4);
        memcpy(zeichen, name, 4);
    }
    
    union {
        struct {
            u8 zeichen[4];
        };
        int name;
    };
    
    operator int() {return this->name;}
};

class Block {
public:
    Block();
    Block(const Block& orig);
    virtual ~Block();
private:

};

#endif	/* BLOCK_H */

