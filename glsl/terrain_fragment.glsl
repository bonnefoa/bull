#version 130

out vec4 color;
in vec2 uvFragmentTexture;
in vec2 uvNormal;
uniform sampler2D textureSampler;
uniform sampler2D normalmapSampler;

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace);

void main()
{
        vec4 texColor = texture(textureSampler, uvFragmentTexture);
        vec3 normal_tangentspace = texture(normalmapSampler, uvNormal).xyz;
        color = processColorTangentspace(texColor, normal_tangentspace);
}
