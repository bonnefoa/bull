#version 130

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;

out vec3 vertexPosition_worldspace;
out vec2 UV;

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;
}
