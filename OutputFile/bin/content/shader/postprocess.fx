#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"
#include "func.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    //float fogFactor : FOG;
};

// =====================================
// PostProcess Shader
// Domain       : Post Process
// Mesh         : RectMesh
// Blend        : Default
// DepthStencil : NoTest NoWrite
#define PostProcessTarget   g_tex_0
#define PositionTarget      g_tex_1
#define ColorTarget         g_tex_2
#define IsBind              g_btex_0

//static float FogFactor  = 0.5;
static float FogStart = 0.;
static float FogEnd = 10000.;
// =====================================
VTX_OUT VS_PostProcess(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;    
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vUV = _in.vUV;

    
    return output;
}

float4 PS_PostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    // _in.vPosition; ÇÈ¼¿ ÁÂÇ¥
    float2 vScreenUV = _in.vPosition.xy / vResolution.xy;    

     float3 vViewPos = PositionTarget.Sample(g_sam_0, vScreenUV).xyz;
    float fogFactor = saturate((FogEnd - vViewPos.z) / (FogEnd - FogStart));
    
    float4 fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
    float4 finalColor = fogFactor * PostProcessTarget.Sample(g_sam_0, vScreenUV) +(1.0 - fogFactor) * fogColor;
    
    return finalColor;
}









#endif
