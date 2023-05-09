#ifndef _TEST
#define _TEST

#include "value.fx"

// Vertex Shader
struct VTX_IN
{
    float3 vPos     : POSITION; // semantic    
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition    : SV_Position;
    float4 vColor       : COLOR;
    float2 vUV          : TEXCOORD;
};

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;   
    
    // Local Mesh �� ��ǥ�� ����� ��ġ
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);    
    
    // World ��ǥ�� ī�޶� �����̽��� �̵�
    float4 vViewPos = mul(vWorldPos, g_matView);
    
    // ī�޶� �����̽� ��ǥ�� ������ǥ�� �̵�
    float4 vProjPos = mul(vViewPos, g_matProj);
        
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;    
    output.vUV = _in.vUV;
    
    return output;
}


// Rasterizer
// ������ ����� ����(Topology) �ȿ� ������ �ȼ��� ���� (�ȼ� ���̴� �ĺ�)
// �ش� �ȼ��� ���� �ȼ� ���̴� ȣ��


float4 PS_Test(VTX_OUT _in): SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    //vOutColor = _in.vColor;
    
    //if (g_int_0)
    //    vOutColor = float4(1.f, 0.f, 0.f, 1.f);
    //else
    //    vOutColor = float4(0.f, 0.f, 1.f, 1.f);
        
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);   
    
    return vOutColor;
}











#endif