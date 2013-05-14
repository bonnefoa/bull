#version 130

uniform mat4 depthMVP;

attribute vec3 vertexPosition_modelspace;

void main()
{
        gl_Position = depthMVP * vec4(vertexPosition_modelspace, 1.0f);
}
