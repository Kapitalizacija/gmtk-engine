#version 460 core

layout (location = 0) in vec2 aVert;
layout (location = 1) in vec3 aPos;
layout (location = 2) in float rotation;
layout (location = 3) in vec2 scale;
layout (location = 4) in ivec3 color;
layout (location = 5) in int shouldDraw;
layout (location = 6) in int texOffset;

layout (location = 0) out vec2 texCoords;
layout (location = 1) out flat int offset;

uniform mat4 projection;

void main() {
    if ( shouldDraw == 0 ) {
        gl_Position = vec4(0);
        return;
    }

    mat2 rotationMatrix = mat2(
        vec2(cos(rotation), -sin(rotation)),
        vec2(sin(rotation), cos(rotation))
    );

    gl_Position = projection * vec4(aPos.xy + aVert * scale * rotationMatrix, aPos.z, 1.0);

    texCoords = vec2(0.5, 0.5) + aVert;

    offset = texOffset;
}
/*
#version 460 core

layout (location = 0) in vec2 aVert;
layout (location = 1) in vec3 aPos;
layout (location = 2) in float rotation;
layout (location = 3) in vec2 scale;
layout (location = 4) in int shouldDraw;
layout (location = 5) in int texOffset;

layout (location = 0) out vec2 texCoords;
layout (location = 1) out flat int offset;

uniform mat4 projection;

void main() {
    if ( shouldDraw == 0 ) {
        gl_Position = vec4(0);
        return;
    }

    const float PI = 3.14159;

    float distFromCenter = 0.7071067811865475;
    const float angles[] = {
        PI / 4 * 1,
        PI / 4 * 3,
        PI / 4 * 5,
        PI / 4 * 7
    };

    vec2 rotatedVert = vec2(
        cos(angles[gl_VertexID] + rotation) * distFromCenter,
        sin(angles[gl_VertexID] + rotation) * distFromCenter
    ) + vec2(distFromCenter, distFromCenter);

    gl_Position = projection * vec4(aPos.xy + scale * rotatedVert, aPos.z, 1.0);

    texCoords = aVert;

    offset = texOffset;
}*/