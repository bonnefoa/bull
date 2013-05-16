#version 130

out vec3 color;
in vec2 UV;

uniform sampler2DShadow samplerTexture;

void main()
{
        color.x = texture(samplerTexture, vec3(UV, 1));
}
