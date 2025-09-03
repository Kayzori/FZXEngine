#include "GLShaderManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string GLShaderManager::LoadShaderSource(const std::string filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[Error] Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint GLShaderManager::CompileShader(GLenum shaderType, const std::string source) {
    GLuint shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    if (!CheckShaderCompileStatus(shader)) {
        std::cerr << "[Error] Shader compilation failed for type: " << shaderType << std::endl;
    }
    return shader;
}

GLuint GLShaderManager::CreateShaderProgram(const std::string frag_source, const std::string vert_source, const std::string geom_source)
{
    GLuint program = glCreateProgram();

    GLuint vertShader = 0, fragShader = 0, geomShader = 0;
    if (!vert_source.empty()) {
        vertShader = CompileShader(GL_VERTEX_SHADER, vert_source);
        if (vertShader) glAttachShader(program, vertShader);
    }
    if (!frag_source.empty()) {
        fragShader = CompileShader(GL_FRAGMENT_SHADER, frag_source);
        if (fragShader) glAttachShader(program, fragShader);
    }
    if (!geom_source.empty()) {
        geomShader = CompileShader(GL_GEOMETRY_SHADER, geom_source);
        if (geomShader) glAttachShader(program, geomShader);
    }

    glLinkProgram(program);

    if (!CheckProgramLinkStatus(program)) {
        glDeleteProgram(program);
        program = 0;
    }

    if (fragShader) glDeleteShader(fragShader);
    if (vertShader) glDeleteShader(vertShader);
    if (geomShader) glDeleteShader(geomShader);

    return program;
}


bool GLShaderManager::CheckShaderCompileStatus(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, ' ');
        glGetShaderInfoLog(shader, logLength, nullptr, &log[0]);
        std::cerr << "[Shader Compile Error]\n" << log << std::endl;
        return false;
    }
    return true;
}

bool GLShaderManager::CheckProgramLinkStatus(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, ' ');
        glGetProgramInfoLog(program, logLength, nullptr, &log[0]);
        std::cerr << "[Program Link Error]\n" << log << std::endl;
        return false;
    }
    return true;
}
