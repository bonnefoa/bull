#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform int verticeNumber;
uniform sampler2D samplerTexture;

in vec3 vertexPosition_modelspace;

out vec2 UV;

void main()
{
        UV = vertexPosition_modelspace.xz / verticeNumber;
        float height = texture(samplerTexture, UV).x;

        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        gl_Position.y += height;
}
