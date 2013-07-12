#version 130

out vec4 color;
uniform sampler2D textureSampler;
in vec2 UV;

void main()
{
        color = texture(textureSampler, UV);
}
