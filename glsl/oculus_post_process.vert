#version 130

in vec3 vertexPosition;
in vec2 vertexUV;
out vec2 UV;

void main()
{
        gl_Position = vec4(vertexPosition, 1);
        UV = vertexUV;
}
