/* 
 * File:   Shader.h
 * Author: Dario
 *
 * Created on 30. Oktober 2011, 19:45
 */

#ifndef __SC_SHADER_H
#define	__SC_SHADER_H

#include "main.h"

struct ShaderProgramParameter
{
    ShaderProgramParameter() {};
    ShaderProgramParameter(const char* _vertexShaderName, const char* _fragmentShaderName)
    : vertexShaderName(_vertexShaderName), fragmentShaderName(_fragmentShaderName)
    {
        
    }
        
    DRString vertexShaderName;
    DRString fragmentShaderName;
    
};


class Shader : public DRIResource
{
public:
    Shader(DHASH id = 0);
    ~Shader();
    
    DRReturn init(const char* shaderFile, GLenum shaderType);
    __inline__ GLuint getShader() {return mShaderID;}
    __inline__ DHASH getID() {return mID;}
    
    virtual const char* getResourceType() const {return "Shader";}
    virtual bool less_than(DRIResource& shader) const
    {
        return mID <  dynamic_cast<Shader&>(shader).mID;
    }
    
    operator GLuint() {return mShaderID;}
protected:
    unsigned char* readShaderFile(const char *filename);
    
private:
    DHASH mID;
    GLuint mShaderID;
};

typedef DRResourcePtr<Shader> ShaderPtr;

class ShaderProgram : public DRIResource
{
public:
    ShaderProgram(DHASH id = 0);
    ~ShaderProgram();
    
    DRReturn init(ShaderPtr vertexShader, ShaderPtr fragmentShader);
    void bind() const;
    static void unbind();
	void setUniform3fv(const char* name, const DRVector3& data);
	void setUniform1f(const char* name, GLfloat data);
    void setUniform2fv(const char* name, const DRVector2& data);
    void setUniform1i(const char* name, GLint data);
    void setUniform2iv(const char* name, const DRVector2i& data);
	//! \param transpose Specifies whether to transpose the matrix as the values are loaded into the uniform variable
	void setUniformMatrix(const char* name, const DRMatrix& matrix, bool transpose = false);

	DRMatrix getUniformMatrix(const char* name);
    
    __inline__ GLuint getProgram() {return mProgram;}
    __inline__ DHASH getID() {return mId;}
    
    virtual const char* getResourceType() const {return "ShaderProgram";}
    virtual bool less_than(DRIResource& shader) const
    {
        return mId <  dynamic_cast<ShaderProgram&>(shader).mId;
    }
    
protected:
    DHASH  mId;
    ShaderPtr mVertexShader;
    ShaderPtr mFragmentShader;
    GLuint mProgram;
};

typedef DRResourcePtr<ShaderProgram> ShaderProgramPtr;

#endif	/* __SC_SHADER_H */

