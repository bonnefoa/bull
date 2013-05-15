#version 130

uniform mat4 depthVP;
uniform mat4 depthM;

attribute vec3 vertexPosition_modelspace;

void main()
{
        gl_Position = depthVP * depthM * vec4(vertexPosition_modelspace, 1.0f);
}
