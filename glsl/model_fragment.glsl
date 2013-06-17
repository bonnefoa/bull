#version 130

uniform sampler2D textureSampler;

out vec4 color;

in vec2 UV;

vec4 processColor(vec4 texColor);

void main()
{
        vec4 texColor = texture(textureSampler, UV);
        color = processColor(texColor);
}
