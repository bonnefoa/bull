#version 130

uniform mat4 biasMatrix = mat4(
          0.5, 0, 0, 0
        , 0, 0.5, 0, 0
        , 0, 0, 0.5, 0
        , 0.5, 0.5, 0.5, 1.0
);
uniform mat4 shadowVP;
uniform vec3 lightPosition_worldspace;
uniform mat4 M;
uniform mat4 V;

in vec3 vertexPosition_modelspace;
in vec3 vertexNormal_modelspace;

out vec3 vertexPosition_cameraspace;
out vec3 vertexNormal_cameraspace;
out vec3 lightDirection_cameraspace;
out vec4 shadowCoord;

void computeLightInformations()
{
        mat3 MV = mat3(V * M);
        vertexPosition_cameraspace = MV * vertexPosition_modelspace;
        vertexNormal_cameraspace = MV * vertexNormal_modelspace;
        vec3 lightPosition_cameraspace = mat3(V) * lightPosition_worldspace;
        lightDirection_cameraspace = normalize(lightPosition_cameraspace - vertexPosition_cameraspace);
        shadowCoord = (biasMatrix * (shadowVP * M) * vec4(vertexPosition_modelspace, 1.0f));
}

in vec3 vertexTangent_modelspace;
in vec3 vertexBitangent_modelspace;

out vec3 lightDirection_tangentspace;
out vec3 eyeDirection_tangentspace;

out mat3 cameraToTangent;

void computeTangentLightInformations()
{
        mat4 MV = V * M;
        mat3 MV3x3 = mat3(V * M);
        vertexNormal_cameraspace = (MV3x3 * vertexNormal_modelspace);
        vec3 vertexTangent_cameraspace = (MV3x3 * vertexTangent_modelspace);
        vec3 vertexBitangent_cameraspace = (MV3x3 * vertexBitangent_modelspace);
        cameraToTangent = transpose(
                mat3(vertexTangent_cameraspace,
                        vertexBitangent_cameraspace,
                        vertexNormal_cameraspace));

        vertexPosition_cameraspace = (MV
                * vec4(vertexPosition_modelspace, 1)).xyz;
        vec3 lightPosition_cameraspace = (V
                * vec4(lightPosition_worldspace, 1)).xyz;
        lightDirection_cameraspace = normalize(
                lightPosition_cameraspace - vertexPosition_cameraspace);

        vec3 eyeDirection_cameraspace = normalize(- vertexPosition_cameraspace);

        eyeDirection_tangentspace = cameraToTangent * eyeDirection_cameraspace;
        lightDirection_tangentspace = cameraToTangent * lightDirection_cameraspace;
}
