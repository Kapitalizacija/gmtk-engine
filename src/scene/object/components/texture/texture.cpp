#include "texture.hpp"

namespace GMTKEngine {
    Texture::Texture() {
        mComponentName = "Texture";
    }

    void Texture::setTexture(GLTexture& gl_texture) {
        mTextureID = gl_texture.getTexture();
    }

    GLuint Texture::getRawHandle() {
        return mTextureID;
    }

    bool Texture::changedSinceLastUpdate() {
        return false;
    }
  
}