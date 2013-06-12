#version 130

attribute vec3 vertexPosition;
out vec2 UV;

void main()
{
        gl_Position = vec4(vertexPosition, 1.0f);
        UV = (vertexPosition.xy + vec2(1, 1)) / 2.0f;
}
