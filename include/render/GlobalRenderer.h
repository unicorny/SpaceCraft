/* 
 * File:   GlobalRenderer.h
 * Author: Dario
 *
 * Created on 7. Oktober 2011, 17:57
 */

#ifndef SPACE_CRAFT_GLOBAL_RENDERER_H
#define	SPACE_CRAFT_GLOBAL_RENDERER_H

class GlobalRenderer
{
public:
    ~GlobalRenderer() {if(isInitialized()) exit();};

    // Singleton-Daten
    static GlobalRenderer&		Instance();
    inline static GlobalRenderer& getSingleton() {return Instance();};
    inline static GlobalRenderer* getSingletonPtr() {return &Instance();};
    static bool	isInitialized()	{return Instance().m_bInitialized;};

    //init und exit
    DRReturn init();
    void exit();
    
    inline ShaderProgram* getPlanetShaderPtr() {return &mPlanetShader;}
    inline GenerateNoisePlanet* getGenerateNoisePlanet() {return mPlanetGen;}
    
    GLUquadricObj* getQuadric() {if(m_bInitialized) return mQuadratic; else return NULL;}
    
private:
    GlobalRenderer();
    
    bool				m_bInitialized;
    GLUquadricObj*                      mQuadratic; 
    GenerateNoisePlanet*                mPlanetGen;
    ShaderProgram                       mPlanetShader;
};


#endif	/* SPACE_CRAFT_GLOBAL_RENDERER_H */

