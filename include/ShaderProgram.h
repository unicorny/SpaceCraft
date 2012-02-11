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

