#version 130

out vec4 color;
in vec2 uvFragmentTexture;
uniform sampler2D textureSampler;
uniform sampler2D normalmapSampler;

in vec3 vertexPosition_worldspace;

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace, float visibility);

void main()
{
        vec4 texColor = texture(textureSampler, uvFragmentTexture);
        vec3 normal_tangentspace = normalize(texture(normalmapSampler,
            uvFragmentTexture).xyz * 2.0 - 1);
        color = processColorTangentspace(texColor, normal_tangentspace, 1.0f);
}
