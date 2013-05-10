#version 130

out vec3 color;
in vec2 UV;
uniform sampler2D textureSampler;

void main()
{
       /*color = vec3(UV, 1.0f);*/
       color = texture(textureSampler, UV);
}
