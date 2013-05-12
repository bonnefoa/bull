#version 130

uniform sampler2D textureSampler;

out vec3 color;

in vec2 UV;

in vec3 vertexNormal_cameraspace;
in vec3 lightDirection_cameraspace;

void main()
{
       /*vec4 texColor = texture(textureSampler, UV);*/
       /*float coef = max(dot(vertexNormal_cameraspace*/
                /*, lightDirection_cameraspace), 0.0);*/
       /*coef = clamp(coef, 0.0, 1.0);*/
       /*color = texColor * coef;*/
       color = vertexNormal_cameraspace;
}
