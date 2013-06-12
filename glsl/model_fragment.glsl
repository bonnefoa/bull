#version 130

uniform mat4 V;
uniform sampler2D textureSampler;
uniform sampler2DShadow shadowSampler;

uniform vec3 ambientColor = vec3(0.1, 0.1, 0.1);

uniform vec3 lightColor;
uniform float lightConstantAttenuation = 1.0;
uniform float lightLinearAttenuation = 0.0;
uniform float lightQuadraticAttenuation = 0.0;
uniform vec3 lightPosition_worldspace;

out vec4 color;

in vec2 UV;

in vec3 vertexPosition_cameraspace;
in vec3 vertexNormal_cameraspace;
in vec3 vertexPosition_worldspace;
in vec3 lightDirection_cameraspace;

in vec4 shadowCoord;
float biais = 0.05f;

float getSpecularCoefficient()
{
        vec3 eyeDirection_cameraspace = normalize(vec3(0,0,0) - vertexPosition_cameraspace);
        vec3 reflectedLight = reflect(-lightDirection_cameraspace, vertexNormal_cameraspace);
        float coef = clamp(dot(reflectedLight, eyeDirection_cameraspace), 0, 1);
        return coef;
}

float getDiffuseCoefficient()
{
        float coef = dot(lightDirection_cameraspace, vertexNormal_cameraspace);
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

        float diffuseCoef = getDiffuseCoefficient();
        float specularCoef = getSpecularCoefficient();
        float attenuation = getAttenuation();

        vec3 diffusePart = texColor * lightColor * diffuseCoef * attenuation;
        vec3 specularPart = texColor * lightColor * pow(specularCoef, 5) * attenuation;

        float visibility = texture(shadowSampler,
                vec3(shadowCoord.xy, (shadowCoord.z-biais) / shadowCoord.w));

        color = vec4(ambientPart + (diffusePart + specularPart) * visibility, 1);
}
