#version 130

out vec3 color;
in vec2 UV;

uniform sampler2D samplerTexture;

void main()
{
        color = vec3(texture(samplerTexture, UV).r);
}
