#version 460 core

out vec4 color;

layout (location = 0) in vec2 texCoords;
layout (location = 1) in flat int instanceID;

uniform sampler2D textures[32];
uniform int offsets[32];

void main(){
    int offset = 0;

    int offsets_cum = 0;
    for (int i = 0; i < 32; i++) {
        offsets_cum += offsets[i];

        if (instanceID < offsets_cum) {
            offset = i;
            break;
        }
    }

    color = texture(textures[offset], texCoords);
}