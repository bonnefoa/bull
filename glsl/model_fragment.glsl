#version 130

uniform sampler2D textureSampler;

out vec3 color;

in vec2 UV;
in vec3 normal_worldspace;

void main()
{
       /*color = texture(textureSampler, UV).xyz;*/
       color = normal_worldspace;
}
