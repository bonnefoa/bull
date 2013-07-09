#version 130

in vec3 UV;
out vec4 color;
uniform samplerCube cubemapSampler;

void main()
{
        color = texture(cubemapSampler, UV);
}
