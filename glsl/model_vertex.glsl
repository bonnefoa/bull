#version 130

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPosition_modelspace;
uniform vec3 lightColor;

attribute vec3 vertexPosition_modelspace;
attribute vec3 normal_modelspace;
attribute vec2 vertexUV;

out vec3 vertexPosition_worldspace;
out vec2 UV;
out vec3 normal_worldspace;

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;
        normal_worldspace = normal_modelspace;
}
