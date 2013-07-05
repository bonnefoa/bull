#version 130

out vec4 color;
in vec2 uvFragmentTexture;
uniform sampler2D textureSampler;
uniform sampler2D normalmapSampler;

in vec3 vertexPosition_worldspace;

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace);

void main()
{
        vec4 texColor = vec4(texture(textureSampler, uvFragmentTexture).xyz, 1);
        vec3 normal_tangentspace = normalize(texture(normalmapSampler,
            uvFragmentTexture).xyz * 2.0 - 1);
        color = processColorTangentspace(texColor, normal_tangentspace);
}
