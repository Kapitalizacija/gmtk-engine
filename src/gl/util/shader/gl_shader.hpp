#pragma once

#include <sstream>
#include <fstream>

#include <glad/glad.h>

#include "io/logging/logger.hpp"

namespace GMTKEngine {
    class GLShader {
        public:
            GLShader(const std::string name, const std::string vertex_path, const std::string fragment_path);

            GLShader(const GLShader&) = delete;

            GLShader(GLShader&& other);
            void operator=(GLShader&& other);

            ~GLShader();

            bool is_valid();
            GLuint getProgram();
            std::string get_name();
        private:
            std::string read_file(std::string path);
            GLuint create_shader(std::string shader_path, GLenum type);
            void create_program(GLuint vert_shader, GLuint frag_shader);

            void move(GLShader& other);

            GLuint program;
            std::string name;
    };
}