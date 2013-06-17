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
        vec3 vertexPosition_worldspace =
                (M * vec4(vertexPosition_modelspace, 1.0f)).xyz;
        vertexPosition_cameraspace =
                (V * vec4(vertexPosition_worldspace, 1.0f)).xyz;
        vertexNormal_cameraspace = normalize(mat3(V) * mat3(M) * vertexNormal_modelspace);
        lightDirection_cameraspace = normalize(mat3(V) * (lightPosition_worldspace - vertexPosition_worldspace));
        shadowCoord = (biasMatrix * (shadowVP * M) * vec4(vertexPosition_modelspace, 1.0f));
}
