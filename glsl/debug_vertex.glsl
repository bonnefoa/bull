#version 130

uniform mat4 V;
uniform mat4 P;

in vec3 vertexPosition_modelspace;
in vec3 vertexColor;

out vec3 fragmentColor;

void main()
{
        gl_Position = P * V * vec4(vertexPosition_modelspace, 1.0f);
        fragmentColor = vertexColor;
}
