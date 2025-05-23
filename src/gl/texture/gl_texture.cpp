#include "gl_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace GMTKEngine {
	GLTexture::GLTexture(): tex(0) {}

    GLTexture::GLTexture(std::string imagePath): tex(0) {
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

		if (data == nullptr) {
			ERROR("Failed to load texture from path " << imagePath);
			return;
		}

		createTexture(data, width, height, channels);

		stbi_image_free(data);
	}

    GLTexture::GLTexture(GLTexture&& other) noexcept{
		tex = other.tex;

		other.tex = 0;
	} 

    void GLTexture::operator=(GLTexture&& other) noexcept{
		tex = other.tex;

		other.tex = 0;
	}

	GLTexture::~GLTexture() {
		if ( !isValid() ) {
			return;
		}

		glDeleteTextures(1, &tex);
	}

    void GLTexture::createTexture(uint8_t* data, int width, int height, int channels) {

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		GLint format;
		switch(channels) {
			case 1: 
				format = GL_RED;
				break;
			case 2: 
				format = GL_RG;
				break;
			case 3: 
				format = GL_RGB;
				break;
			case 4: 
				format = GL_RGBA;
				break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
    
	GLuint GLTexture::getTexture(){
		return tex;
	}

	bool GLTexture::isValid() {
		return tex != 0;
	}
}