#version 460 core

out vec4 color;

layout (location = 0) in vec2 texCoords;

uniform sampler2D tex;

void main(){
    color = texture(tex, texCoords);

    if (color.x < 0.1 && color.y < 0.1 && color.z < 0.1) {
        discard;
    }
}