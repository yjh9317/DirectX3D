#ifndef _OBJECTHEIGHT
#define _OBJECTHEIGHT

#include "value.fx"


//Texture2D<float> _HEIGHT : register(t0);
RWStructuredBuffer<float> _BUFFER : register(u0);

#define HEIGHT_MAP      g_tex_0


[numthreads(1, 1, 1)]
void CS_ObjectHeight(int3 _iThreadID : SV_DispatchThreadID)
{
    
    if (g_vec2_1.x < 0.f || g_vec2_1.x > 1.f || g_vec2_1.y < 0.f || g_vec2_1.y > 1.f)
        return;
    
    _BUFFER[0] = HEIGHT_MAP.SampleLevel(g_sam_0, g_vec2_1, 0).x;
    
}


#endif