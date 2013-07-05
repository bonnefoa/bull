#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec3 vertexPosition_modelspace;
in vec2 uvTexture;

out vec2 uvFragmentTexture;

void computeTangentLightInformations();

void main()
{
        uvFragmentTexture = uvTexture;
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);
        computeTangentLightInformations();
}
