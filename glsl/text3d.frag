#version 130

uniform sampler2D textureSampler;

in vec2 UV;

out vec4 color;

void main()
{
        color = texture(textureSampler, UV);
}
