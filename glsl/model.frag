#version 130

uniform sampler2D diffuseSampler;
uniform sampler2D normalSampler;

out vec4 color;

in vec2 UV;

vec4 processColorCameraspace(vec4 texColor);

in vec3 lightDirection_tangentspace;

void main()
{
        vec3 texColor = texture(diffuseSampler, UV).rgb;
        vec3 normal = (texture(normalSampler, UV).rgb * 2.0 - 1);

        float coef = dot(normal, lightDirection_tangentspace);
        color = processColorCameraspace(vec4(texColor * coef, 1));
}
