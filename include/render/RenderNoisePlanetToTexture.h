#ifndef __SC_RENDER_NOISE_PLANET_TO_TEXTURE__
#define __SC_RENDER_NOISE_PLANET_TO_TEXTURE__

class DRGeometrieHeightfield;

class RenderNoisePlanetToTexture : public RenderInStepsToTexture
{
public:
    RenderNoisePlanetToTexture(const char* vertexShaderName, const char* fragmentShaderName);
    ~RenderNoisePlanetToTexture();
    
    void update(float theta, float h) {mTheta = theta; mH = h;}
    DRReturn init(float stepSize, float theta, float h, Texture* texture, const DRMatrix& rotation = DRMatrix::identity());
    
    virtual DRReturn renderStuff();
    
    __inline__ const DRMatrix& getRotationsMatrix() {return mRotation;}
    __inline float getTheta() {return mTheta;}

    
protected:
private:
    ShaderProgram* mShader;
    DRGeometrieHeightfield* mRenderSphere;
    float         mTheta;
    float         mH;
    DRMatrix      mRotation;
};

#endif //__SC_RENDER_NOISE_PLANET_TO_TEXTURE__