#version 460 core

out vec4 color;

layout (location = 0) in vec2 texCoords;
layout (location = 1) in flat int offset;

uniform sampler2D textures[32];

void main(){
    color = texture(textures[offset], texCoords);
}