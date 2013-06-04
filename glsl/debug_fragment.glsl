#version 130

uniform sampler2D textureSampler;

in vec3 fragmentColor;
in vec2 UV;

out vec3 color;

void main()
{
        color = texture(textureSampler, UV).xyz;
        if(color == vec3(0,0,0)){
                color = fragmentColor;
        }
}
