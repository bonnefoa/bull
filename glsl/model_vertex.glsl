#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPosition_modelspace;
uniform vec3 lightColor;

attribute vec3 vertexPosition_modelspace;
attribute vec2 vertexUV;

attribute vec3 vertexNormal_modelspace;

out vec3 vertexPosition_worldspace;
out vec2 UV;

out vec3 vertexNormal_cameraspace;
out vec3 lightDirection_cameraspace;

vec3 getCameraspacePosition(vec3 pos)
{
        return (V * vec4(pos, 1.0f)).xyz;
}

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;

        vec3 vertexPosition_cameraspace =
                (V * M * vec4(vertexPosition_modelspace, 1.0f)).xyz;
        vec3 lightPosition_cameraspace =
                getCameraspacePosition(lightPosition_modelspace);
        vertexNormal_cameraspace =
                (M * vec4(vertexNormal_modelspace, 1.0f)).xyz;
                /*(V * M * vec4(vertexNormal_modelspace, 1.0f)).xyz;*/
        lightDirection_cameraspace = normalize(
                vertexPosition_cameraspace - lightPosition_cameraspace);
}
