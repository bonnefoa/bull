#version 130

uniform sampler2D textureSampler;

out vec3 color;

in vec2 UV;

in vec3 vertexNormal_worldspace;
in vec3 lightDirection_worldspace;

void main()
{
       vec3 texColor = texture(textureSampler, UV).xyz;
       float coef = max(dot(vertexNormal_worldspace
                , lightDirection_worldspace), 0.0);
       coef = clamp(coef, 0.0, 1.0);
       color = texColor * coef;
}
