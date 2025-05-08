#version 460 core

out vec4 color;

layout (location = 0) in vec2 texCoords;

uniform sampler2D textures[32];

void main(){
    color = texture(textures[0], texCoords);
}