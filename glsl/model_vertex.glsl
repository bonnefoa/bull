#version 130

uniform mat4 MVP;

attribute vec3 vertexPosition_modelspace;

out vec3 vertexPosition_worldspace;

void main()
{
        gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0f);
}
