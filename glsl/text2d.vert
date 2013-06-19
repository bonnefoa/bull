#version 130

uniform int width;
uniform int height;

attribute vec2 vertexPosition_screenspace;
attribute vec2 vertexUV;

out vec2 UV;

void main()
{
        vec2 vertexPosition_homogeneousspace = vertexPosition_screenspace - vec2(width, height);
        vertexPosition_homogeneousspace /= vec2(width, height);
        gl_Position = vec4(vertexPosition_homogeneousspace, 0, 1);
        UV = vertexUV;
}
