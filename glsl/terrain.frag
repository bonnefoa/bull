#version 130

out vec4 color;
in vec2 uvFragmentTexture;
uniform sampler2D textureSampler;
uniform sampler2D normalmapSampler;

in vec3 vertexPosition_worldspace;
in vec3 lightDirection_cameraspace;
in vec3 lightDirection_tangentspace;

void main()
{
        vec3 texColor = texture(textureSampler, uvFragmentTexture).xyz;
        vec3 normal_tangentspace = normalize(texture(normalmapSampler,
            uvFragmentTexture).xyz * 2.0 - 1);

        float coef = clamp(dot(normal_tangentspace, lightDirection_tangentspace), 0, 1);

        color = vec4(texColor * coef, 1);
}
