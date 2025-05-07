#include "texture.hpp"

namespace GMTKEngine {
    Texture::Texture() {
        mComponentName = "Texture";
    }

    Texture::~Texture() {
        WARN("Nigga this shit ain't implemented");
    }

    void Texture::setTexture(GLTexture& gl_texture) {
        mTextureID = gl_texture.getTexture();
    }

    GLuint Texture::get_raw_handle() {
        return mTextureID;
    }

  
}