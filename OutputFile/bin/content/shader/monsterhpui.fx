#ifndef _MONSTERHPUI
#define _MONSTERHPUI

#include "value.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


#define HPRatio    g_float_0
VTX_OUT VS_MONSTERHPUI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_MONSTERHPUI(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    

    if (_in.vUV.x >= HPRatio)
    {
        vOutColor = (float4) float4(0.f, 1.f, 0.f, 1.f);
    }
    else
    {
        vOutColor = (float4) float4(1.f, 0.f, 0.f, 1.f);
    }
        
    
    if (g_btex_0)
    {
        //vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        if (_in.vUV.x <= HPRatio)
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            discard;
    }
    return vOutColor;
}

#endif