#version 130

out vec3 color;
in vec2 UV;

uniform sampler2D renderTexture;

void main()
{
        color = texture(renderTexture, UV).xyz;
}
