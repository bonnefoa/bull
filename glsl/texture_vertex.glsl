#version 130

in vec3 vertexPosition_modelspace;
out vec2 UV;

void main()
{
        gl_Position = vec4(vertexPosition_modelspace, 1.0f);
        UV = (vertexPosition_modelspace.xy + vec2(1, 1)) / 2.0f;
}
