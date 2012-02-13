/* 
 * File:   Shader.h
 * Author: Dario
 *
 * Created on 30. Oktober 2011, 19:45
 */

#ifndef __SC_SHADER_H
#define	__SC_SHADER_H

class ShaderProgram
{
public:
    ShaderProgram(DHASH id = 0);
    ~ShaderProgram();
    
    DRReturn init(const char* vertexShaderFile, const char* fragmentShaderFile);
    void bind() const;
    static void unbind();
	void setUniform3fv(const char* name, const DRVector3& data);
	void setUniform1f(const char* name, GLfloat data);
	//! \param transpose Specifies whether to transpose the matrix as the values are loaded into the uniform variable
	void setUniformMatrix(const char* name, const DRMatrix& matrix, bool transpose = false);

	DRMatrix getUniformMatrix(const char* name);
    
    __inline__ GLuint getProgram() {return mProgram;}
    __inline__ DHASH getID() {return mId;}
    
protected:
    unsigned char* readShaderFile(const char *filename);
    
    DHASH  mId;
    GLuint mVertexShader;
    GLuint mFragmentShader;
    GLuint mProgram;
};

#endif	/* __SC_SHADER_H */

