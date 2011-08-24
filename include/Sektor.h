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
    
    union
    {
        struct
        {
            u8 left;
            u8 right;
            u8 front;
            u8 back;
            u8 top;
            u8 bottom;
            u8 empty[2];
        };
        u64 id;
    };
};


class Sektor {
public:
    Sektor();
    Sektor(const Sektor& orig);
    virtual ~Sektor();
private:

};

#endif	/* SEKTOR_H */

