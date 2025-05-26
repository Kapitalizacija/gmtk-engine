#pragma once

#include <sstream>
#include <fstream>

#include <glad/glad.h>

#include "io/logging/logger.hpp"

namespace Sierra {
    class GLShader {
        public:
            GLShader(const std::string name, const std::string vertex_path, const std::string fragment_path);

            GLShader(const GLShader&) = delete;

            GLShader(GLShader&& other) noexcept;
            void operator=(GLShader&& other) noexcept;

            ~GLShader();

            bool is_valid();
            GLuint getProgram();
            std::string get_name();
        private:
            std::string read_file(std::string path);
            GLuint create_shader(std::string shader_path, GLenum type);
            void create_program(GLuint vert_shader, GLuint frag_shader);

            void move(GLShader& other) noexcept;

            GLuint program;
            std::string name;
    };
}