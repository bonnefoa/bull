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

out vec3 vertexNormal_worldspace;
out vec3 lightDirection_worldspace;

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;

        vec3 vertexPosition_worldspace =
                (M * vec4(vertexPosition_modelspace, 1.0f)).xyz;
        vertexNormal_worldspace =
                (M * vec4(vertexNormal_modelspace, 1.0f)).xyz;
        lightDirection_worldspace = normalize(
                vertexPosition_worldspace - lightPosition_modelspace);
}
