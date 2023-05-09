#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

// ========================
// Directional Light Shader
// MRT      : Light
// mesh     : RectMesh
// RS       : CULL_BACK
// DS       : NO_TEST_NO_WRITE
// BS       : ONE_ONE

#define LightIdx        g_int_0
#define PositionTarget  g_tex_0
#define NormalTarget    g_tex_1
#define ShadowMap       g_tex_2

#define LightVP         g_mat_0

#define value           g_float_0
// ========================

struct VS_DIR_IN
{
    float3 vPos : POSITION;
};

struct VS_DIR_OUT
{
    float4 vPosition : SV_Position;
};

VS_DIR_OUT VS_Directional(VS_DIR_IN _in)
{
    VS_DIR_OUT output = (VS_DIR_OUT) 0.f;

    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    
    return output;
}

struct PS_DIR_OUT
{
    float4 vDiffuse : SV_Target0;
    float4 vSpecular : SV_Target1;
    float4 vShadowPow : SV_Target2;
};

PS_DIR_OUT PS_Directional(VS_DIR_OUT _in)
{
    PS_DIR_OUT output = (PS_DIR_OUT)0.f;

    float2 vUV = _in.vPosition.xy / vResolution.xy;

    float3 vViewPos = PositionTarget.Sample(g_sam_0, vUV).xyz;
    float3 vViewNormal = NormalTarget.Sample(g_sam_0, vUV).xyz;
    float fShadowPow = 0.f;

    // 물체가 그려지지 않은 영역은 빛을 받을 수 없다.
    if (vViewPos.z == 0.f)
        clip(-1);

    tLightColor color = (tLightColor)0.f;
    CalculateLight3D(vViewPos, vViewNormal, LightIdx, color);

    // 그림자 판정
    // ViewPos -> WorldPos
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;

    // WorldPos -> Light 투영
    float4 vLightProj = mul(float4(vWorldPos, 1.f), LightVP);

    // w 로 나눠서 실제 xy 투영좌표를 구함
    vLightProj.xyz /= vLightProj.w;

    // 샘플링을 하기 위해서 투영좌표계를 UV 좌표계로 변환
    float2 vShadowMapUV = float2((vLightProj.x / 2.f) + 0.5f, -(vLightProj.y / 2.f) + 0.5f);
    float fShadowMapDepth = ShadowMap.Sample(g_sam_0, vShadowMapUV).r;

    float _pcf = 0.0f;

    float fOffsetX = 1.f / 4096.f;
    float fOffsetY = 1.f / 4096.f;

    int iStart = -2;
    int iEnd = 2;
    for (int iRow = iStart; iRow <= iEnd; ++iRow)
    {
        for (int iCol = iStart; iCol <= iEnd; ++iCol)
        {
            float2 fOffset = float2(iCol * fOffsetX, iRow * fOffsetY);
            _pcf += ShadowMap.Sample(g_sam_0, vShadowMapUV + fOffset);
        }
    }

    _pcf /= pow((iEnd - iStart + 1), 2);

    float GausFilter = 0.f;

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            float2 fOffset = float2(j * fOffsetX, i * fOffsetY);
            GausFilter += ShadowMap.Sample(g_sam_0, vShadowMapUV + fOffset) * GaussianFilter[i][j];
        }
    }


    float vResult;// = (_pcf * 0.35f + GausFilter * 0.65f);
    vResult = _pcf;

    // 광원에 기록된 깊이보다, 물체의 깊이가 더 멀 때, 그림자 판정
    /*
        0.f != fShadowMapDepth
        && 0.f <= vShadowMapUV.x && vShadowMapUV.x <= 1.f
        && 0.f <= vShadowMapUV.y && vShadowMapUV.y <= 1.f
        &&
    */
    if (0.f != fShadowMapDepth
        && 0.f <= vShadowMapUV.x && vShadowMapUV.x <= 1.f
        && 0.f <= vShadowMapUV.y && vShadowMapUV.y <= 1.f
        && vLightProj.z >= vResult + value)
    {
        fShadowPow = 0.9f;
    }


    
    output.vDiffuse = (color.vDiff + color.vAmb);
    output.vSpecular = color.vSpec; 
    output.vShadowPow = fShadowPow;

    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    output.vShadowPow.a = 1.f;

    return output;
}



// ==================
// Point Light Shader
// MRT      : Light
// Mesh     : Sphere
// RS_TYPE  : CULL_FRONT, 광원영역(Volume Mesh) 안으로 들어왔을 때를 대비
// BS_TYPE  : ONE_ONE, 기존에 그려진 빛(타겟) 을 누적
// DS_TYPE  : NO_TEST_NO_WRITE

//#define LightIdx        g_int_0
//#define PositionTarget  g_tex_0
//#define NormalTarget    g_tex_1
// ==================
VS_DIR_OUT VS_Point(VS_DIR_IN _in)
{
    VS_DIR_OUT output = (VS_DIR_OUT) 0.f;

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        
    return output;
}


PS_DIR_OUT PS_Point(VS_DIR_OUT _in)
{
    PS_DIR_OUT output = (PS_DIR_OUT) 0.f;
        
    float2 vUV = _in.vPosition.xy / vResolution.xy;
    float3 vViewNormal = NormalTarget.Sample(g_sam_0, vUV).xyz;
        
    float3 vViewPos = PositionTarget.Sample(g_sam_0, vUV).xyz;
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    if (length(vLocalPos) < 0.5f)
    {
        tLightColor color = (tLightColor) 0.f;
        CalculateLight3D(vViewPos, vViewNormal, LightIdx, color);
                      
        output.vDiffuse = color.vDiff + color.vAmb;
        output.vSpecular = color.vSpec;
        output.vShadowPow = (float4) 0.f;
    
        output.vDiffuse.a = 1.f;
        output.vSpecular.a = 1.f;
        output.vShadowPow.a = 1.f;
    }
    else
    {
        clip(-1);
        //output.vDiffuse = float4(1.f, 0.f, 0.f, 1.f);
    }
         
    return output;
}



// =================
// Spot Light Shader
// =================



// ==============================
// Merge Shader
// Mesh : RectMesh
#define ColorTarget g_tex_0
#define DiffuseTarget g_tex_1
#define SpecularTarget g_tex_2
#define ShadowPowTarget g_tex_3
#define PositionTarget g_tex_4
#define EmissionTarget g_tex_5
// ==============================
struct VS_MERGE_IN
{
    float3 vPos : POSITION;
};

struct VS_MERGE_OUT
{
    float4 vPosition : SV_Position;
};

VS_MERGE_OUT VS_Merge(VS_MERGE_IN _in)
{
    VS_MERGE_OUT output = (VS_MERGE_OUT) 0.f;

    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    
    
    return output;
}

static const float Weight4[25] =
{
    0.0050, 0.0162, 0.0382, 0.0561, 0.1353, 0.1953, 0.278, 0.4868, 0.6018, 0.7261, 0.8131, 0.9231,
    1, 0.9231, 0.8131, 0.7261, 0.6018, 0.4868, 0.278, 0.1953, 0.1353, 0.0561, 0.0382, 0.0162, 0.0050
};

float4 PS_Merge(VS_MERGE_OUT _in) : SV_Target0
{
    float4 vOutColor = (float4) 0.f;
    
    float2 vUV = _in.vPosition.xy / vResolution.xy;

    
    float3 vColor = ColorTarget.Sample(g_sam_0, vUV).xyz;
    float3 vDiffuse = DiffuseTarget.Sample(g_sam_0, vUV).xyz;
    float3 vSpecular = SpecularTarget.Sample(g_sam_0, vUV).xyz;
    float  fShadowPow = ShadowPowTarget.Sample(g_sam_0, vUV).x;
    float3 fEmissive = EmissionTarget.Sample(g_sam_0, vUV);

    float3 vViewPos = PositionTarget.Sample(g_sam_0, vUV).xyz;



    vOutColor = float4(vColor * vDiffuse * (1.f - fShadowPow)
        + vSpecular * (1.f - fShadowPow) + fEmissive, 1.f);


    float fWeightOnePixel = 1.f / vResolution.x;
    float fHeightOnePixel = 1.f / vResolution.y;

    float2 t = vUV;
    float2 uv = (float2) 0.f;
    float4 vBloomColor = (float4) 0.f;

    for (int i = -12; i < 12; ++i)
    {
        uv = t + float2(fWeightOnePixel * i, 0.f);
        vBloomColor += Weight4[12 + i] * g_tex_0.Sample(g_sam_0, uv);
    }

    for (int i = -12; i < 12; ++i)
    {
        uv = t + float2(0, fHeightOnePixel * i);
        vBloomColor += Weight4[12 + i] * g_tex_0.Sample(g_sam_0, uv);
    }

    vBloomColor /= 12.f;

    float vBrightness = dot(vBloomColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));

    if (vBrightness > 0.99)
        vOutColor = float4(vBloomColor.rgb, 1.f);

    return vOutColor;
}


// ===============
// ShdowMap Shader
// MRT : ShadowMap MRT
// RS : CULL_BACK
// BS : Default
// DS : Less
// ===============


struct VS_SHADOW_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_SHADOW_OUT
{
    float4 vPosition : SV_Position;
    float4 vProjPos : POSITION;
};

VS_SHADOW_OUT VS_ShadowMap(VS_SHADOW_IN _in)
{
    VS_SHADOW_OUT output = (VS_SHADOW_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vProjPos = output.vPosition;    
    
    return output;
}

float PS_ShadowMap(VS_SHADOW_OUT _in) : SV_Target
{
    float fOut = 0.f;    
    fOut = _in.vProjPos.z / _in.vProjPos.w;
    return fOut;
}





#endif