#version 130

out vec4 color;
in vec2 UV;

uniform sampler2D samplerTexture;

void main()
{
        color = texture(samplerTexture, UV);
}
