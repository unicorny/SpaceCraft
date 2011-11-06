/* 
 * File:   RenderStellarBody.h
 * Author: Dario
 *
 * Created on 6. November 2011, 14:43
 */

#ifndef __SC_RENDER_STELLAR_BODY_H
#define	__SC_RENDER_STELLAR_BODY_H

class RenderStellarBody
{
public:
    RenderStellarBody(): mGeometrie(NULL) {}
    virtual ~RenderStellarBody();
    
protected:
    DRGeometrie* mGeometrie;
};


#endif	/* __SC_RENDER_STELLAR_BODY_H */

