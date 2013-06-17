#version 130

uniform mat4 V;
uniform mat4 P;

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;

out vec2 UV;

void main()
{
        gl_Position = P * V * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;
}
