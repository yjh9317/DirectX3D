#ifndef _WEIGHTMAP
#define _WEIGHTMAP

#include "value.fx"
#include "func.fx"

struct tWeight_4
{
    float arrWeight[4];
};

RWStructuredBuffer<tWeight_4> WEIGHT_MAP : register(u0); // unordered access
RWTexture2D<float4> WEIGHT_TEX : register(u1); // unordered access

StructuredBuffer<tRaycastOut> LOCATION : register(t16); // �귯�� ��ġ(�»�� ����)

#define WIDTH       g_int_0
#define HEIGHT      g_int_1

#define BRUSH_TEX   g_texarr_0
//#define BRUSH_TEX   g_tex_0

#define SCALE       g_vec2_0   // �귯�� ũ��
#define BRUSH_IDX   g_int_2    // �귯�� �ε���
#define WEIGHT_IDX  g_int_3    // �ø� ����ġ ��ġ

#define Save        g_float_0
#define Load        g_float_1


[numthreads(32, 32, 1)]
void CS_WeightMap(int3 _iThreadID : SV_DispatchThreadID)
{
    if (Save)
    {
        float4 weight;
        weight.x =WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[0];
        weight.y =WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[1];
        weight.z =WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[2];
        WEIGHT_TEX[_iThreadID.xy] = float4(weight[0], weight[1], weight[2], 1);
        return;
    }
    
    if (Load)
    {
        WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[0] = float(WEIGHT_TEX[_iThreadID.xy].r);
        WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[1] = float(WEIGHT_TEX[_iThreadID.xy].g);
        WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[2] = float(WEIGHT_TEX[_iThreadID.xy].b);
        
        
        //if (WEIGHT_TEX[_iThreadID.xy].r == 0 &&
        //    WEIGHT_TEX[_iThreadID.xy].g == 0 &&
        //    WEIGHT_TEX[_iThreadID.xy].b == 0)
        //{
        //    WEIGHT_MAP[(_iThreadID.y * WIDTH) + _iThreadID.x].arrWeight[2] = 1;
        //}
        
        for (int i = 0; i < 32;++i)
        {
            WEIGHT_MAP[(_iThreadID.y * WIDTH) + i].arrWeight[2] = 1;
        }
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
        return;
    }

    // brush texture �� ���� ����
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);

    float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, float3(vUV, BRUSH_IDX), 0);
    //float4 vBrushColor = BRUSH_TEX.SampleLevel(g_sam_0, vUV, 0);

    // 2���� �ε��� ��ǥ�� 1���� �ε����� ���
    uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;

    // ����ġ �� Ư�� ������ ���� �׽�Ʈ
    // WEIGHT_MAP[iIdx].arrWeight[1] += g_fDT * 0.5f * vBrushColor.a;



    // ���� ����ġ
    float arrWeight[4] = WEIGHT_MAP[iIdx].arrWeight;
    
    // ���� ��
    float fAdd = fDT * vBrushColor.a * 2.f;

    // ����ġ�� �ø���, ������ ������ ������ ���߾� ������.
    arrWeight[WEIGHT_IDX] = saturate(arrWeight[WEIGHT_IDX] + fAdd);

    // �ʰ���
    float fTotal = 0.f;
    for (int i = 0; i < 4; ++i)
    {
        fTotal += arrWeight[i];
    }

    // �� ���� 1�� �Ѿ�� ���
    if (1.f < fTotal)
    {
        float fOver = fTotal - 1.f;

        // ������ ����ġ�� ����
        fTotal = fTotal - arrWeight[WEIGHT_IDX];

        for (int j = 0; j < 4; ++j)
        {
            if (j == WEIGHT_IDX)
                continue;

            // �ʰ����� ������ ���߾� �����, �ٸ� ����ġ �ʿ��� ����.
            arrWeight[j] = saturate(arrWeight[j] - fOver * (arrWeight[j] / fTotal));
        }
    }



    // ���ۿ� ���
    //arrWeight[iIdx] = 1;
    WEIGHT_MAP[iIdx].arrWeight = arrWeight;

   
}

#endif
