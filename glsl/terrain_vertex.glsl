#version 130

uniform mat4 MVP;

in vec3 vertexPosition_modelspace;

void main()
{
        gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0f);
}
