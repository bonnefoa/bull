#version 130

out vec4 color;
in vec2 uvFragmentTexture;
in vec2 uvNormal;
uniform sampler2D textureSampler;
uniform sampler2D normalmapSampler;

vec4 processColor(vec4 texColor);

void main()
{
        vec4 texColor = texture(textureSampler, uvFragmentTexture);
        vec4 normal = texture(normalmapSampler, uvNormal);
        color = processColor(texColor);
}
