#version 460 core

layout (location = 0) in vec2 aVert;
layout (location = 1) in vec2 texOffset;

layout (location = 0) out vec2 texCoords;

uniform mat4 projection;

uniform vec3 pos;
uniform vec2 scale;
uniform float rotation;

void main() {
    mat2 rotationMatrix = mat2(
        vec2(cos(rotation), -sin(rotation)),
        vec2(sin(rotation), cos(rotation))
    );

    gl_Position = projection * vec4(pos.xy + aVert * scale * rotationMatrix, pos.z, 1.0);

    texCoords = vec2(0.5, 0.5) + aVert;
    if (texCoords.x == 0) {
        texCoords.x = texOffset.x;
    } else {
        texCoords.x = texOffset.y;
    }

    texCoords.y = 1 - texCoords.y;
    
}

