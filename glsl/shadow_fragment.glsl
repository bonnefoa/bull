#version 130

out vec3 color;

void main()
{
        gl_FragDepth = gl_FragCoord.z;
        color = vec3(gl_FragCoord.z, 0,0);
}
