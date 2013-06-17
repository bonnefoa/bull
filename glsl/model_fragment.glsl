#version 130

uniform sampler2D textureSampler;

out vec4 color;

in vec2 UV;

vec4 processColor(vec3 texColor);

void main()
{
        vec3 texColor = texture(textureSampler, UV).xyz;
        color = processColor(texColor);
}
