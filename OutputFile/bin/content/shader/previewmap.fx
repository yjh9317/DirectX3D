#ifndef _PREVIEWMAP
#define _PREVIEWMAP

#include "value.fx"

RWTexture2D<float> PREVIEW_MAP : register(u0); // unordered acess
StructuredBuffer<tRaycastOut> LOCATION : register(t16); // �귯�� ��ġ(�»�� ����)

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

#define BRUSH_TEX   g_texarr_0
//#define BRUSH_TEX   g_tex_0

#define SCALE       g_vec2_0   // �귯�� ũ��
#define BRUSH_IDX   g_int_2    // �귯�� �ε���
#define Zero        g_int_3

[numthreads(32, 32, 1)]
void CS_PreviewMap(int3 _iThreadID : SV_DispatchThreadID)
{
    if (Zero)
    {
        PREVIEW_MAP[_iThreadID.xy] = 0;
        return;
    }
    
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].success)
    {
        return;
    }
    
    int2 vCenterPos = float2(WIDTH, HEIGHT) * LOCATION[0].vUV;
    int2 vScale = float2(WIDTH, HEIGHT) * SCALE;

    if (_iThreadID.x < vCenterPos.x - (vScale.x / 2) || vCenterPos.x + (vScale.x / 2) < _iThreadID.x
        || _iThreadID.y < vCenterPos.y - (vScale.y / 2) || vCenterPos.y + (vScale.y / 2) < _iThreadID.y)
    {
        PREVIEW_MAP[_iThreadID.xy] = 0.f;
        return;
    }

    // brush texture ���� ���ø� �� UV ���
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);

    // �귯���� ���� ���İ� ���ø�
    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, float3(vUV, BRUSH_IDX), 0);
    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, vUV, 0);

    if (Zero)
    {
        PREVIEW_MAP[_iThreadID.xy] = 0;
        return;
    }
    else
    {
        PREVIEW_MAP[_iThreadID.xy] = vBrushColor.a;
    }
     
    
    

    
}


#endif