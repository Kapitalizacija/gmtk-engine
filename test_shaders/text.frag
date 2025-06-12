#version 460 core

out vec4 color;

layout (location = 0) in vec2 texCoords;

uniform sampler2D tex;

void main(){
    float intensity = texture(tex, texCoords).r;

    color = vec4(intensity, intensity, intensity, 1.0);

    if (color.x < 0.1 && color.y < 0.1 && color.z < 0.1) {
        discard;
    }
}