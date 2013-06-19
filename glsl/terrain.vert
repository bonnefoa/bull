#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform sampler2D heightmapSampler;

in vec3 vertexPosition_modelspace;
in vec2 uvTexture;
in vec2 uvNormal;

out vec2 uvFragmentTexture;
out vec2 uvFragmentNormal;

void computeTangentLightInformations();

void main()
{
        computeTangentLightInformations();
        gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0f);

        uvFragmentTexture = uvTexture;
        uvFragmentNormal = uvNormal;
}
