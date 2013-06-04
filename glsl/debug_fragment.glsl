#version 130

uniform sampler2D textureSampler;
uniform bool hasTexture = false;

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

void main()
{
        if(hasTexture){
                color = texture(textureSampler, UV);
        } else {
                color = vec4(fragmentColor, 1);
        }
}
