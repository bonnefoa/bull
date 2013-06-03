#version 130

uniform sampler2D textureSampler;

in vec3 fragmentColor;
in vec2 UV;

out vec3 color;

void main()
{
        color = fragmentColor + texture(textureSampler, UV).xyz;
}
