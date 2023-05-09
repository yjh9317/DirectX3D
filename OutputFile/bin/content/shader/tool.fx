#ifndef _TOOL
#define _TOOL

#include "value.fx"

// ======================
// Grid Shader
// Domain : TOOL
// BlendState   : AlphaBlend
// DepthStencil : NO_WRITE   
// g_int_0 : ���� �ܰ�
// g_float_0 : ī�޶� ���̿� ���� �׸����� ����
// g_float_1 : ���� �β�
// g_vec4_0  : ī�޶� ��ġ
// g_vec4_1  : Grid ����
// g_tex_0  : Position Target
// ======================
struct VTX_GRID_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_GRID_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
    float3 vViewPos : POSITION1;
};

VTX_GRID_OUT VS_Grid(VTX_GRID_IN _in)
{
    VTX_GRID_OUT output = (VTX_GRID_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vUV = _in.vUV;

    return output;
}

float4 PS_Grid(VTX_GRID_OUT _in) : SV_Target
{
    float2 vFullUV = float2(_in.vPosition.x / vResolution.x, _in.vPosition.y / vResolution.y);
    float3 vTargetViewPos = g_tex_0.Sample(g_sam_0, vFullUV).xyz;
    
    // �׸��� ����� ���̿�, Deferred ��ü ���� ���̰� �� ���� ������ ��ȿ
    if (abs(vTargetViewPos.z - _in.vViewPos.z) < 1.f)
    {
        clip(-1);
    }
    
    
    float fLen = length(vTargetViewPos);
    float fMax = fLen * 0.01f; // �Ÿ����� 10%�� �ִ뺸�������� ����.    
    
    float fDepthRatio = 1.f;
    if (vTargetViewPos.z != 0.f)
    {
        fDepthRatio = 1.f - saturate((length(_in.vViewPos) - length(vTargetViewPos)) / fMax);
    }
    
    
    // ���� ���� ��ġ���� ī�޶�κ��� �ָ�    
    float fDist = abs(distance(_in.vWorldPos, g_vec4_0.xyz));
    float fThickness = g_float_1 * (fDist / abs(g_vec4_0.y)) * 1.2f;

    // ���纸�� ���� �ܰ� ������ ��Ƴ���.
    int iStep = 1;
    for (int i = 0; i < g_int_0 + 1; ++i)
    {
        iStep *= 10;
    }

    if (fDist > 200.f && fDist > abs(g_vec4_0.y) * 5.f)
    {
        // �þ� �־ �Ⱥ���
        clip(-1);
    }

    float fX = abs(_in.vWorldPos.x - (float) ((int) _in.vWorldPos.x / iStep) * iStep);
    float fZ = abs(_in.vWorldPos.z - (float) ((int) _in.vWorldPos.z / iStep) * iStep);

    if (fThickness / 2.f < fX)
        fX = (float) iStep - fX;
    if (fThickness / 2.f < fZ)
        fZ = (float) iStep - fZ;
    
    float fXRatio = 0.f;
    float fZRatio = 0.f;
    float fRatio = 0.f;

    if (fX < fThickness / 2.f || fZ < fThickness / 2.f)
    {
        fXRatio = ((fThickness / 2.f) - abs(fX)) / (fThickness / 2.f);
        fZRatio = ((fThickness / 2.f) - abs(fZ)) / (fThickness / 2.f);

        fRatio = 0.f;
        if (fXRatio > fZRatio)
            fRatio = fXRatio;
        else
            fRatio = fZRatio;

        return float4(g_vec4_1.rgb, fRatio * fDepthRatio);
    }
    
    // ���� �ܰ� ����
    iStep = 1;
    for (i = 0; i < g_int_0; ++i)
    {
        iStep *= 10;
    }

    fX = abs(_in.vWorldPos.x - (float) ((int) _in.vWorldPos.x / iStep) * iStep);
    fZ = abs(_in.vWorldPos.z - (float) ((int) _in.vWorldPos.z / iStep) * iStep);

    if (fThickness / 2.f < fX)
        fX = (float) iStep - fX;
    if (fThickness / 2.f < fZ)
        fZ = (float) iStep - fZ;
    
    fXRatio = 0.f;
    fZRatio = 0.f;
    fRatio = 0.f;

    if (fX < fThickness / 2.f || fZ < fThickness / 2.f)
    {
        fXRatio = ((fThickness / 2.f) - abs(fX)) / (fThickness / 2.f);
        fZRatio = ((fThickness / 2.f) - abs(fZ)) / (fThickness / 2.f);

        fRatio = 0.f;
        if (fXRatio > fZRatio)
            fRatio = fXRatio;
        else
            fRatio = fZRatio;

        return float4(g_vec4_1.rgb, fRatio * g_float_0 * fDepthRatio);
    }
    
    clip(-1);
    return float4(0.f, 0.f, 0.f, 0.f);
}







#endif
