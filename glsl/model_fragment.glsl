#version 130

uniform sampler2D textureSampler;

uniform vec3 ambientColor;

uniform vec3 lightColor;
uniform float lightConstantAttenuation;
uniform float lightLinearAttenuation;
uniform float lightQuadraticAttenuation;
uniform vec3 lightPosition_worldspace;

out vec3 color;
/*out vec3 ga2;*/

in vec2 UV;

in vec3 vertexPosition_cameraspace;
in vec3 vertexNormal_worldspace;
in vec3 vertexPosition_worldspace;
in vec3 lightDirection_worldspace;

float getSpecularCoefficient()
{
        vec3 eyeDirection_cameraspace = normalize(- vertexPosition_cameraspace);
        vec3 reflectedLight = reflect(lightDirection_worldspace, vertexNormal_worldspace);
        float coef = clamp(dot(reflectedLight, eyeDirection_cameraspace), 0, 1);
        return coef;
}

float getDiffuseCoefficient()
{
        float coef = dot(lightDirection_worldspace, vertexNormal_worldspace);
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
        vec3 specularPart = texColor * lightColor * pow(specularCoef, 5) ;

        color = ambientPart + diffusePart + specularPart;
}
