#version 130

uniform sampler2D diffuseSampler;
uniform sampler2D normalSampler;

out vec4 color;

in vec2 UV;

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace, float visibility);
float getShadowVisibility();

void main()
{
        vec4 texColor = texture(diffuseSampler, UV);
        vec3 normal = (texture(normalSampler, UV).rgb * 2.0 - 1);
        float visibility = getShadowVisibility();
        color = processColorTangentspace(texColor, normal, visibility);
}
