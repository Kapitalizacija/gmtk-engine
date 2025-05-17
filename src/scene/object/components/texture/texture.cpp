#include "texture.hpp"

namespace GMTKEngine {
    namespace Component {
        Texture::Texture() {
            mComponentName = "Texture";
            mTextureID = 0;
            changed = false;
        }

        void Texture::setTexture(GLTexture& glTexture) {
            if(mTextureID != 0 && glTexture.getTexture() != mTextureID) {
                changed = true;
            }

            mTextureID = glTexture.getTexture();
        }

        GLuint Texture::getRawHandle() {
            return mTextureID;
        }

        bool Texture::hasChanged() {
            return changed;
        }
  
        void Texture::frameCleanup() {
            changed = false;
        }
    }
}