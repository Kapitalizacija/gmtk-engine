#version 460 core

layout (location = 0) in vec2 aVert;
layout (location = 1) in vec2 aPos;
layout (location = 2) in float depth;
layout (location = 3) in float rotation;
layout (location = 4) in vec2 scale;

layout (location = 0) out vec2 texCoords;
layout (location = 1) out flat int instanceID;

void main() {
    mat2 rotationMatrix = mat2(
        vec2(cos(rotation), -sin(rotation)),
        vec2(sin(rotation), cos(rotation))
    );

    gl_Position = vec4(aPos.xy + aVert * scale * rotationMatrix, depth, 1.0);

    texCoords = vec2(0.5, 0.5) - aVert;
    instanceID = gl_InstanceID;
}