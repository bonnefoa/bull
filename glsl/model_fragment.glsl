#version 130

uniform sampler2D textureSampler;

uniform vec3 ambientColor;

uniform vec3 lightColor;
uniform float lightConstantAttenuation;
uniform float lightLinearAttenuation;
uniform float lightQuadraticAttenuation;

out vec3 color;

in vec2 UV;

in vec3 vertexNormal_worldspace;
in vec3 vertexPosition_worldspace;
in vec3 lightDirection_worldspace;
in vec3 lightPosition_worldspace;

float getDiffuseCoefficient()
{
        float coef = max(dot(vertexNormal_worldspace
                                , lightDirection_worldspace), 0.0);
        coef = clamp(coef, 0.0, 1.0);
        return coef;
}

float getAttenuation()
{
       float lightDistance = length(lightPosition_worldspace - vertexPosition_worldspace);
       float attenuation = 1 / (lightConstantAttenuation + lightLinearAttenuation * lightDistance + lightQuadraticAttenuation * lightDistance * lightDistance);
       return attenuation;
}

void main()
{
       vec3 texColor = texture(textureSampler, UV).xyz;
       vec3 ambientPart = ambientColor * texColor;
       float coef = getDiffuseCoefficient();
       float attenuation = getAttenuation();
       vec3 diffusePart = texColor * lightColor * coef * attenuation;
       color = ambientPart + diffusePart;
}
