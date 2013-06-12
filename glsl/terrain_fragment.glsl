#version 130

out vec4 color;
in vec2 uvFragmentTexture;
uniform sampler2D textureSampler;

void main()
{
        color = texture(textureSampler, uvFragmentTexture);
}
