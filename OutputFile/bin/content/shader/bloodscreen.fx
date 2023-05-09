#ifndef _BLOODSCREEN
#define _BLOODSCREEN

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

#define Time                g_float_0
// =====================================
VTX_OUT VS_BloodScreen(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_BloodScreen(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    // _in.vPosition; ÇÈ¼¿ ÁÂÇ¥
    float2 vScreenUV = _in.vPosition.xy / vResolution.xy;
 

    // Vignette 
    // https://stackoverflow.com/questions/30122743/colored-vignette-shader-the-outer-part-libgdx
    if (IsBind)
    {
        const float outerRadius = .9, innerRadius = .55, intensity = .6;
        const float3 vignetteColor = float3(1.0, 0.0, 0.0); //red
        //Time
        float4 color = PostProcessTarget.Sample(g_sam_0, vScreenUV);
        float2 relativePosition = _in.vPosition.xy / vResolution - .5;
        relativePosition.y *= vResolution.x / vResolution.y;
        float len = length(relativePosition);
        float vignetteOpacity = smoothstep(innerRadius, outerRadius, len) * intensity * Time; //note inner and outer swapped to switch darkness to opacity
        color.rgb = lerp(color.rgb, vignetteColor, vignetteOpacity);
        
        vOutColor = color;
    }
    else
    {
        vOutColor = PostProcessTarget.Sample(g_sam_0, vScreenUV);
    }
    return vOutColor;
}



#endif
