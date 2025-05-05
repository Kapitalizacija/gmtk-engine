#include "gl_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace GMTKEngine {
    GLTexture::GLTexture() {
		tex = rand();
	}
    
	GLuint GLTexture::get_texture(){
		return tex;
	}

	GLuint GLTexture::load_texture() {
		unsigned char *data = stbi_load("someImage.png", &width, &height, &channels, 0);

		unsigned int texture;
		glGenTextures(1, &texture);
		
        assert(0);
        return 0;//TODO
	}
}