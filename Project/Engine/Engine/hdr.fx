#ifndef _HDR
#define _HDR

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
};

// =====================================
// PostProcess Shader
// Domain       : Post Process
// Mesh         : RectMesh
// Blend        : Default
// DepthStencil : NoTest NoWrite

#define PostProcessTarget   g_tex_0
#define IsBind              g_btex_0

#define MiddleGrey          g_float_0
#define LumWhiteSqr         g_float_1
#define AvgLum              g_float_2

//StructuredBuffer<float> AvgLum : register(t23);
const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);



float3 ToneMapping(float3 HDRColor)
{
    // Find the luminance scale for the current pixel
    float LScale = dot(HDRColor, LUM_FACTOR);

    LScale *= MiddleGrey / AvgLum; //AvgLum[0];
    LScale = (LScale + LScale * LScale / LumWhiteSqr) / (1.0 + LScale);
    
    // Apply the luminance scale to the pixels color
    return HDRColor * LScale;
}



// =====================================
VTX_OUT VS_HDR(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_HDR(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    // _in.vPosition; ÇÈ¼¿ ÁÂÇ¥
    float2 vScreenUV = _in.vPosition.xy / vResolution.xy;

    float3 color = PostProcessTarget.Sample(g_sam_0, vScreenUV).xyz;

    color = ToneMapping(color);

    
    vOutColor = float4(color, 1.f);
        
    
    return vOutColor;
}



#endif
