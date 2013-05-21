#version 130

out vec3 color;
in vec2 UV;
uniform sampler2D samplerTexture;

void main()
{
        color = texture(samplerTexture, UV).xyz;
}
