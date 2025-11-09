#pragma once

#ifndef GLAD_INCLUDED
#include <glad/glad.h>
#endif

#include <string>

class GLShaderManager {
public: 
    static std::string loadShaderSource(const std::string filepath);

    static GLuint compileShader(GLenum shaderType, const std::string source);

    static GLuint createShaderProgram(const std::string frag_source, const std::string vert_source, const std::string geom_source = "");

    static bool checkShaderCompileStatus(GLuint shader);

    static bool checkProgramLinkStatus(GLuint program);
};
