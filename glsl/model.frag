#version 130

uniform sampler2D textureSampler;

out vec4 color;

in vec2 UV;

vec4 processColorCameraspace(vec4 texColor);

void main()
{
        vec4 texColor = texture(textureSampler, UV);
        color = processColorCameraspace(texColor);
}
