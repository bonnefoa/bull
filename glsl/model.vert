#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 lightPosition_worldspace;

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;

in vec3 vertexTangent_modelspace;
in vec3 vertexNormal_modelspace;
in vec3 vertexBitangent_modelspace;

out vec2 UV;
out vec3 lightDirection_tangentspace;

void computeLightInformations();

void main()
{
        computeLightInformations();
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;

        vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;
        vec3 lightPosition_cameraspace = ( V * vec4(lightPosition_worldspace, 1)).xyz;
        vec3 lightDirection_cameraspace = normalize(lightPosition_cameraspace - vertexPosition_cameraspace);
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
