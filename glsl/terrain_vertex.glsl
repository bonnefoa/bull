#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform int gridLenght;
uniform int gridWidth;
uniform sampler2D samplerTexture;

in vec3 vertexPosition_modelspace;

out vec2 UV;

void main()
{
        UV = vec2(vertexPosition_modelspace.x / gridWidth,
                  vertexPosition_modelspace.z / gridLenght);
        float heightPoint = texture(samplerTexture, UV).x;

        vec3 elevatedVertice = vertexPosition_modelspace;
        elevatedVertice.y += heightPoint;
        gl_Position = P * V * M * vec4(elevatedVertice, 1.0f);
}
