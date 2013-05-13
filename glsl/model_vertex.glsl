#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 lightPosition_worldspace;

attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;
attribute vec3 vertexNormal_modelspace;

out vec3 vertexPosition_worldspace;
out vec3 vertexPosition_cameraspace;
out vec2 UV;

out vec3 vertexNormal_worldspace;
out vec3 lightDirection_worldspace;

out vec3 ga;

void main()
{
        ga = vertexPosition_modelspace;

        vec3 vertexPosition_worldspace =
                (M * vec4(vertexPosition_modelspace, 1.0f)).xyz;
        vertexPosition_cameraspace =
                (V * vec4(vertexPosition_worldspace, 1.0f)).xyz;

        gl_Position = P * vec4(vertexPosition_cameraspace, 1.0f);
        UV = vertexUV;

        vertexNormal_worldspace = mat3(M) * vertexNormal_modelspace;
        lightDirection_worldspace = normalize(lightPosition_worldspace - vertexPosition_worldspace);
}
