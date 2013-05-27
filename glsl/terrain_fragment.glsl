#version 130

out vec3 color;
in vec2 uvFragmentTexture;
uniform sampler2D textureSampler;

void main()
{
        color = texture(textureSampler, uvFragmentTexture).xyz;
}
