#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 lightPosition_worldspace;

in vec3 vertexPosition_modelspace;
in vec2 vertexUV;

out vec2 UV;

void computeTangentLightInformations();
void computeShadowCoord();

void main()
{
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        UV = vertexUV;
        computeShadowCoord();
        computeTangentLightInformations();
}
