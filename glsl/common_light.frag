#version 130

uniform sampler2DShadow shadowSampler;

uniform vec3 ambientColor = vec3(0.1, 0.1, 0.1);
uniform vec3 lightColor;

in vec3 vertexNormal_cameraspace;
in vec3 vertexPosition_cameraspace;

in vec3 vertexPosition_worldspace;
in vec3 lightDirection_cameraspace;

uniform vec3 lightPosition_worldspace;
uniform float lightConstantAttenuation = 1.0;
uniform float lightLinearAttenuation = 0.0;
uniform float lightQuadraticAttenuation = 0.0;

in vec4 shadowCoord;
float biais = 0.05f;

in vec3 lightDirection_tangentspace;
in vec3 eyeDirection_tangentspace;

float getSpecularCoefficient(vec3 normal_tangentspace)
{
        vec3 reflectedLight = reflect(-lightDirection_tangentspace, normal_tangentspace);
        float coef = clamp(dot(reflectedLight, eyeDirection_tangentspace), 0, 1);
        return coef;
}

float getAttenuation()
{
        float lightDistance = length(lightPosition_worldspace - vertexPosition_worldspace);
        float attenuation = 1 / (lightConstantAttenuation + lightLinearAttenuation
                * lightDistance + lightQuadraticAttenuation * lightDistance * lightDistance);
        return attenuation;
}

vec4 processColor(vec4 texColor, float diffuseCoef, float specularCoef, float attenuation, float visibility)
{
        vec3 ambientPart = ambientColor * texColor.xyz;
        vec3 diffusePart = texColor.xyz * lightColor * diffuseCoef * attenuation;
        vec3 specularPart = texColor.xyz * lightColor * pow(specularCoef, 5) * attenuation;
        vec3 color = ambientPart + (diffusePart + specularPart) * visibility;
        return vec4(color, 1);
}

float getDiffuseCoefficientTangentspace(vec3 normal_tangentspace)
{
        float coef = dot(normal_tangentspace, lightDirection_tangentspace);
        coef = clamp(coef, 0.0, 1.0);
        return coef;
}

float getShadowVisibility()
{
        return texture(shadowSampler,
                        vec3(shadowCoord.xy, (shadowCoord.z-biais) / shadowCoord.w));
}

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace, float visibility)
{
        float diffuseCoef = getDiffuseCoefficientTangentspace(normal_tangentspace);
        float specularCoef = getSpecularCoefficient(normal_tangentspace);
        float attenuation = getAttenuation();
        return processColor(texColor, diffuseCoef, specularCoef, attenuation, visibility);
}
