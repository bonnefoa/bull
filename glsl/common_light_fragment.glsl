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

vec4 processColorCameraspace(vec4 texColor)
{
        vec4 ambientPart = vec4(ambientColor, 1) * texColor;

        float diffuseCoef = getDiffuseCoefficientCameraspace();
        float specularCoef = getSpecularCoefficientCameraspace();
        float attenuation = getAttenuation();

        vec4 diffusePart = texColor * vec4(lightColor, 1) * diffuseCoef * attenuation;
        vec4 specularPart = texColor * vec4(lightColor, 1) * pow(specularCoef, 5) * attenuation;

        float visibility = texture(shadowSampler,
                vec3(shadowCoord.xy, (shadowCoord.z-biais) / shadowCoord.w));

        vec4 color = ambientPart + (diffusePart + specularPart) * visibility;
        return color;
}



in vec3 lightDirection_tangentspace;
in vec3 eyeDirection_tangentspace;

float getSpecularCoefficientTangentspace(vec3 normal_tangentspace)
{
        vec3 reflectedLight = reflect(-lightDirection_tangentspace, normal_tangentspace);
        float coef = clamp(dot(reflectedLight, eyeDirection_tangentspace), 0, 1);
        return coef;
}

float getDiffuseCoefficientTangentspace(vec3 normal_tangentspace)
{
        float coef = dot(lightDirection_tangentspace, normal_tangentspace);
        coef = clamp(coef, 0.0, 1.0);
        return coef;
}

vec4 processColorTangentspace(vec4 texColor, vec3 normal_tangentspace)
{
        vec4 ambientPart = vec4(ambientColor, 1) * texColor;

        float diffuseCoef = getDiffuseCoefficientTangentspace(normal_tangentspace);
        float specularCoef = getSpecularCoefficientTangentspace(normal_tangentspace);
        float attenuation = getAttenuation();

        vec4 diffusePart = texColor * vec4(lightColor, 1) * diffuseCoef * attenuation;
        vec4 specularPart = texColor * vec4(lightColor, 1) * pow(specularCoef, 5) * attenuation;

        float visibility = texture(shadowSampler,
                vec3(shadowCoord.xy, (shadowCoord.z-biais) / shadowCoord.w));

        vec4 color = ambientPart + (diffusePart + specularPart) * visibility;
        return color;
}
