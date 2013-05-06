#version 130

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

attribute vec3 vertexPosition_modelspace;

out vec3 vertexPosition_worldspace;

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        /*gl_Position = vec4(vertexPosition_modelspace, 1.8f);*/
}
