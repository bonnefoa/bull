#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 lightPosition_worldspace;

uniform sampler2D heightmapSampler;

in vec3 vertexPosition_modelspace;
in vec2 uvTexture;
in vec2 uvNormal;

out vec2 uvFragmentTexture;

in vec3 vertexTangent_modelspace;
in vec3 vertexBitangent_modelspace;
in vec3 vertexNormal_modelspace;

out vec3 vertexPosition_worldspace;
out vec3 lightDirection_cameraspace;
out vec3 lightDirection_tangentspace;

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);

        vertexPosition_worldspace = (M * vec4(vertexPosition_modelspace, 1.0f)).xyz;
        vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;

        vec3 lightPosition_cameraspace = (V * vec4(lightPosition_worldspace, 1)).xyz;
        lightDirection_cameraspace = normalize(lightPosition_cameraspace - vertexPosition_cameraspace);

        uvFragmentTexture = uvTexture;
        mat3 MV3x3 = mat3(V * M);

        vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
        vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
        vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;
        mat3 TBN = transpose(mat3(
                vertexTangent_cameraspace,
              vertexBitangent_cameraspace,
                 vertexNormal_cameraspace
        ));

        lightDirection_tangentspace = TBN * lightDirection_cameraspace;
}
