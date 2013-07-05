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

float getSpecularCoefficientCameraspace()
{
        vec3 eyeDirection_cameraspace = normalize(vec3(0,0,0) - vertexPosition_cameraspace);
        vec3 reflectedLight = reflect(-lightDirection_cameraspace, vertexNormal_cameraspace);
        float coef = clamp(dot(reflectedLight, eyeDirection_cameraspace), 0, 1);
        return coef;
}

float getDiffuseCoefficientCameraspace()
{
        float coef = dot(lightDirection_cameraspace, vertexNormal_cameraspace);
        coef = clamp(coef, 0.0, 1.0);
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
        vec4 ambientPart = vec4(ambientColor, 1) * texColor;
        vec4 diffusePart = texColor * vec4(lightColor, 1) * diffuseCoef * attenuation;
        vec4 specularPart = texColor * vec4(lightColor, 1) * pow(specularCoef, 5) * attenuation;
        vec4 color = ambientPart + (diffusePart + specularPart) * visibility;
        return color;
}

vec4 processColorCameraspace(vec4 texColor)
{
        float diffuseCoef = getDiffuseCoefficientCameraspace();
        float specularCoef = getSpecularCoefficientCameraspace();
        float attenuation = getAttenuation();
        float visibility = texture(shadowSampler,
                        vec3(shadowCoord.xy, (shadowCoord.z-biais) / shadowCoord.w));
        return processColor(texColor, diffuseCoef, specularCoef, attenuation, visibility);
}

in vec3 lightDirection_tangentspace;

float getDiffuseCoefficientTangentspace(vec3 normal_tangentspace)
{
        float coef = clamp(dot(normal_tangentspace, lightDirection_tangentspace), 0, 1);
        coef = clamp(coef, 0.0, 1.0);
        return coef;
}

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace)
{
        float diffuseCoef = getDiffuseCoefficientTangentspace(normal_tangentspace);
        float specularCoef = getSpecularCoefficientCameraspace();
        float attenuation = getAttenuation();
        return processColor(texColor, diffuseCoef, specularCoef, attenuation, 1);
}
