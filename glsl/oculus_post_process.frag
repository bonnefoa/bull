#version 130

out vec4 color;
in vec2 UV;

uniform sampler2D samplerTexture;

const vec2 LeftLensCenter = vec2(0.2863248, 0.5);
const vec2 RightLensCenter = vec2(0.7136753, 0.5);
const vec2 LeftScreenCenter = vec2(0.25, 0.5);
const vec2 RightScreenCenter = vec2(0.75, 0.5);
const vec2 Scale = vec2(0.1469278, 0.2350845);
const vec2 ScaleIn = vec2(4, 2.5);
const vec4 HmdWarpParam   = vec4(1, 0.22, 0.24, 0);

vec2 HmdWarp(vec2 in01, vec2 LensCenter)
{
        vec2 theta = (in01 - LensCenter) * ScaleIn;
        float rSq = theta.x * theta.x + theta.y * theta.y;
        vec2 rvector = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq +
                        HmdWarpParam.z * rSq * rSq +
                        HmdWarpParam.w * rSq * rSq * rSq);
        return LensCenter + Scale * rvector;
}

void main()
{
        vec2 LensCenter = gl_FragCoord.x < 640 ? LeftLensCenter : RightLensCenter;
        vec2 ScreenCenter = gl_FragCoord.x < 640 ? LeftScreenCenter : RightScreenCenter;

        vec2 tc = HmdWarp(UV * vec2(0.5, 1), LensCenter);
        if (any(bvec2(clamp(tc,ScreenCenter - vec2(0.25,0.5), ScreenCenter + vec2(0.25,0.5)) - tc)))
        {
                color = vec4(0.0, 0.0, 0.0, 1.0);
                return;
        }

        tc.x = gl_FragCoord.x < 640 ? (2.0 * tc.x) : (2.0 * (tc.x - 0.5));
        color = texture(samplerTexture, tc);
}
