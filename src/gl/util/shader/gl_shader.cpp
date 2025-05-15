#include "gl_shader.hpp"

namespace GMTKEngine {
    GLShader::GLShader(const std::string name, const std::string vertex_path, const std::string fragment_path): program(0), name(name) {
        GLuint vertex_shader = create_shader(vertex_path, GL_VERTEX_SHADER);
        GLuint fragment_shader = create_shader(fragment_path, GL_FRAGMENT_SHADER);

        if ( !vertex_shader ) {
            ERROR("Failed to generate vertex shader from: : " << vertex_path);
            return;
        }

        if ( !fragment_shader ) {
            ERROR("Failed to generate fragment shader from: : " << fragment_path);
            return;
        }

        create_program(vertex_shader, fragment_shader);
    }
    
    GLShader::GLShader(GLShader&& other) {
        move(other);
    }

    void GLShader::operator=(GLShader&& other) {
        move(other);
    }

    GLShader::~GLShader() {
        if ( !is_valid() ) {
            return;
        }
        
        glDeleteProgram(program);
    }

    bool GLShader::is_valid() {
        return program != 0;
    }

    GLuint GLShader::getProgram() {
        return program;
    }
    
    std::string GLShader::get_name() {
        return name;
    }

    std::string GLShader::read_file(std::string path) {
        std::fstream stream(path, std::ios_base::in | std::ios_base::ate);

        if ( !stream.is_open() ) {
            ERROR("Failed to open shader file for " << name.c_str() << ": " << path);
            return {};
        }

        std::string buff;
        buff.resize(stream.tellg());

        stream.seekg(0);
        stream.read((char*)buff.data(), buff.size());

        return std::move(buff);
    }

    GLuint GLShader::create_shader(std::string shader_path, GLenum type) {
        std::string buff = read_file(shader_path);

        GLint size = buff.size();
        GLchar* src = buff.data();

        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, &size);
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if ( !status ) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);

            ERROR("Failed to compile shader file for " << name.c_str() << ": " << shader_path << "\n\t" << log);

            glDeleteShader(shader);

            return 0;
        }

        return shader;
        
    }

    void GLShader::create_program(GLuint vert_shader, GLuint frag_shader) {
        program = glCreateProgram();

        glAttachShader(program, vert_shader);
        glAttachShader(program, frag_shader);

        glLinkProgram(program);

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if ( !status ) {
            char log[512];
            glGetProgramInfoLog(program, 512, nullptr, log);

            std::string err = (std::ostringstream() << "Failed to link program for shader " << name.c_str() << "\n\t"
            << log).str();

            ERROR("Failed to link program for shader " << name.c_str() << "\n\t" << log);
            
            glDeleteProgram(program);
            program = 0;
        }

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
    }

    void GLShader::move(GLShader& other) {
        program = other.program;
        name = other.name;

        other.program = 0;
        other.name = "";
    }
}