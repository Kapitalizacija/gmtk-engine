#include "gl_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Sierra {
	GLTexture::GLTexture(): mTex(0) {}

    GLTexture::GLTexture(uint8_t* data, int width, int height, int channels): mWidth(width), mHeight(height) {
        createTexture((uint8_t*) data, width, height, channels);
    }

    GLTexture::GLTexture(std::string imagePath): mTex(0) {
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

    void GLTexture::partialUpdate(uint8_t* data, int xOffset, int yOffset) {
        glBindTexture(GL_TEXTURE_2D, mTex);

        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, mWidth, mHeight, mFormat, GL_UNSIGNED_BYTE, data);
	}

    GLTexture::GLTexture(GLTexture&& other) noexcept{
		mTex = other.mTex;

		other.mTex = 0;
	} 

    void GLTexture::operator=(GLTexture&& other) noexcept{
		mTex = other.mTex;

		other.mTex = 0;
	}

	GLTexture::~GLTexture() {
		if ( !isValid() ) {
			return;
		}

		glDeleteTextures(1, &mTex);
	}

    void GLTexture::createTexture(uint8_t* data, int width, int height, int channels) {

		glGenTextures(1, &mTex);
		glBindTexture(GL_TEXTURE_2D, mTex);

		switch(channels) {
			case 1: 
				mFormat = GL_RED;
				break;
			case 2: 
				mFormat = GL_RG;
				break;
			case 3: 
				mFormat = GL_RGB;
				break;
			case 4: 
				mFormat = GL_RGBA;
				break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mFormat, width, height, 0, mFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
    
	GLuint GLTexture::getTexture(){
		return mTex;
	}

	bool GLTexture::isValid() {
		return mTex != 0;
	}
}