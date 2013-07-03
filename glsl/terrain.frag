#version 130

out vec4 color;
in vec2 uvFragmentTexture;
in vec2 uvFragmentNormal;
uniform sampler2D textureSampler;
uniform sampler2D normalmapSampler;

in vec3 vertexPosition_worldspace;
in vec3 lightDirection_cameraspace;
in vec3 lightDirection_tangentspace;

void main()
{
        vec3 texColor = texture(textureSampler, uvFragmentTexture).xyz;
        vec3 normal_tangentspace = texture(normalmapSampler, uvFragmentNormal).xyz;

        float coef =  dot(normal_tangentspace, lightDirection_tangentspace);

        /*color = vec4(texColor * coef, 1);*/
        /*color = vec4(lightDirection_tangentspace, 1);*/
        color = vec4(texColor, 1);
}
