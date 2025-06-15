#include "texture.hpp"

namespace Sierra {
    namespace Component {
        Texture::Texture() {
            mComponentName = "Texture";
            mTextureID = 0;
            changed = false;
            mColor = glm::ivec3(1);
        }

        Texture::Texture(GLTexture& gl_texture) {
            mComponentName = "Texture";
            mTextureID = gl_texture.getTexture();
            changed = false;
            mColor = glm::ivec3(1);
        }

        void Texture::setTexture(GLTexture& glTexture) {
            if(mTextureID != 0 && glTexture.getTexture() != mTextureID) {
                changed = true;
            }

            mTextureID = glTexture.getTexture();
        }

        void Texture::setColor(glm::ivec3 color) {
            mColor = color;
        }

        glm::ivec3 Texture::getColor() {
            return mColor;
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