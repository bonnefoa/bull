#version 130

uniform sampler2D textureSampler;

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

void main()
{
        color = vec4(fragmentColor, 1);
}
