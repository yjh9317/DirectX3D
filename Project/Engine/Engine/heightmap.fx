#ifndef _HEIGHTMAP
#define _HEIGHTMAP

#include "value.fx"

RWTexture2D<float> HEIGHT_MAP : register(u0); // unordered acess
StructuredBuffer<tRaycastOut> LOCATION : register(t16); // �귯�� ��ġ(�»�� ����)

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

#define BRUSH_TEX   g_texarr_0
//#define BRUSH_TEX   g_tex_0


#define SCALE       g_vec2_0   // �귯�� ũ��
#define BRUSH_IDX   g_int_2    // �귯�� �ε���
#define BRUSH_DIR   g_int_3

#define BRUSH_POW   g_float_0
#define BRUSH_MOD   g_float_1

#define Load        g_float_2

[numthreads(32, 32, 1)]
void CS_HeightMap(int3 _iThreadID : SV_DispatchThreadID)
{
    if(Load)
    {
        HEIGHT_MAP[_iThreadID.xy] = HEIGHT_MAP[_iThreadID.xy];
    }
    
    int2 vCenterPos = float2(WIDTH, HEIGHT) * LOCATION[0].vUV;
    int2 vScale = float2(WIDTH, HEIGHT) * SCALE;

    // brush texture ���� ���ø� �� UV ���
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);


    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].success)
    {
        return;
    }


    if (_iThreadID.x < vCenterPos.x - (vScale.x / 2) || vCenterPos.x + (vScale.x / 2) < _iThreadID.x
        || _iThreadID.y < vCenterPos.y - (vScale.y / 2) || vCenterPos.y + (vScale.y / 2) < _iThreadID.y)
    {
        return;
    }

    // �귯���� ���� ���İ� ���ø�
    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, float3(vUV, BRUSH_IDX), 0);
    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, vUV, 0);    

    if (BRUSH_MOD == 0)
    {
        if (0 == BRUSH_DIR)
        {
            HEIGHT_MAP[_iThreadID.xy] += fDT * vBrushColor.a * BRUSH_POW; // �귯�� ���İ����� ���� ����
        }
        else if (1 == BRUSH_DIR)
        {
            HEIGHT_MAP[_iThreadID.xy] -= fDT * vBrushColor.a * BRUSH_POW;
        }
    }
    else if (BRUSH_MOD == 1)
    {
        int2 vCenterUV = vLTPos + vScale / 2.f;
        float CenterHeight = HEIGHT_MAP[vCenterUV.xy];
        
        if(CenterHeight > HEIGHT_MAP[_iThreadID.xy])
        {
            HEIGHT_MAP[_iThreadID.xy] += fDT * BRUSH_POW * vBrushColor.a;
        }
        else if (CenterHeight < HEIGHT_MAP[_iThreadID.xy])
        {
            HEIGHT_MAP[_iThreadID.xy] -= fDT * BRUSH_POW * vBrushColor.a;
        }

    }

    // cos �׷����� ���� ����
    //float vDist = (distance(vCenterPos, _iThreadID.xy) / vScale) * 3.1415926535f;        
    //HEIGHT_MAP[_iThreadID.xy] += saturate(fDT * cos(vDist) * 0.2f);    
}


#endif