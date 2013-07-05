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
in vec3 vertexTangent_modelspace;
in vec3 vertexBitangent_modelspace;

out vec3 vertexPosition_worldspace;
out vec3 vertexPosition_cameraspace;
out vec3 vertexNormal_cameraspace;

out vec3 lightDirection_cameraspace;
out vec4 shadowCoord;
out vec3 lightDirection_tangentspace;

void computeLightInformations()
{
        mat3 MV3x3 = mat3(V * M);
        vertexPosition_cameraspace = MV3x3 * vertexPosition_modelspace;
        vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;
        vec3 lightPosition_cameraspace = mat3(V) * lightPosition_worldspace;
        lightDirection_cameraspace = normalize(lightPosition_cameraspace - vertexPosition_cameraspace);
        shadowCoord = (biasMatrix * (shadowVP * M) * vec4(vertexPosition_modelspace, 1.0f));
}


void computeTangentLightInformations()
{
        vertexPosition_worldspace = (M * vec4(vertexPosition_modelspace, 1.0f)).xyz;
        vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;

        vec3 lightPosition_cameraspace = (V * vec4(lightPosition_worldspace, 1)).xyz;
        lightDirection_cameraspace = normalize(lightPosition_cameraspace - vertexPosition_cameraspace);

        mat3 MV3x3 = mat3(V * M);

        vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
        vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
        vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;
        mat3 TBN = transpose(mat3(
                vertexTangent_cameraspace,
              vertexBitangent_cameraspace,
                 vertexNormal_cameraspace
        ));

        lightDirection_tangentspace = TBN * lightDirection_cameraspace;
}
