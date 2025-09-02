#pragma once
#include <string>
#include <glad/glad.h>

class GLShaderManager {
public: 
    static std::string LoadShaderSource(const std::string filepath);

    static GLuint CompileShader(GLenum shaderType, const std::string source);

    static GLuint CreateShaderProgram(const std::string frag_source, const std::string vert_source, const std::string geom_source = "");

    static bool CheckShaderCompileStatus(GLuint shader);

    static bool CheckProgramLinkStatus(GLuint program);
};
