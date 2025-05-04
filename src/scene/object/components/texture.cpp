#include "texture.hpp"

namespace GMTKEngine {
    Texture::Texture() {
        mComponentName = "Texture";
    }

    Texture::~Texture() {
        WARN("Nigga this shit ain't implemented");
    }

    void Texture::set_texture(GLTexture& gl_texture) {
//        mTextureID = gl_texture.get_texture();
    }
}