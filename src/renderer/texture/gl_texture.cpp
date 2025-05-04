#include "gl_texture.hpp"

namespace GMTKEngine {
	GLTexture::load_texture() {
		unsigned char *data = stbi_load("someImage.png", &width, &height, &channels, 0);

		unsigned int texture;
		glGenTextures(1, &texture);
		
	}
}