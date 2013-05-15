#version 130

out vec3 color;
in vec2 UV;

uniform sampler2DShadow samplerTexture;

void main()
{
        color = vec3(texture(samplerTexture, vec3(UV, 1)));
}
