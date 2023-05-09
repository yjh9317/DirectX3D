#ifndef _UI
#define _UI

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
    float3 vDirUV : TEXCOORD1;
};


#define HPRatio  g_float_0

VTX_OUT VS_UI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT)0.f;

    float3 vTmp = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
    // 그냥 투영행렬 곱해도 카메라에 적용됨 
    float3 vTmp2 = mul(float4(vTmp, 0.f), g_matViewInv);
    float3 vTmp3 = mul(float4(vTmp2, 0.f), g_matView);
    
    output.vPosition = mul(float4(vTmp3, 1.f), g_mat_0);
    //float4 f = mul(float4(vTmp3, 1.f), g_mat_0);
    
    //float3 tmp = mul(float4(_in.vPos, 1.f), g_mat_1); // 카메라의 y축 행렬만 가져와서 역행렬을 곱함. y축 빌보드
    
    //output.vPosition = mul(float4(tmp, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}

float4 PS_UI(VTX_OUT _in) : SV_Target
{
    float4 vOutColor;// = (float4) float4(0.3f, 0.3f, 0.3f, 1.f);

    if (_in.vUV.x <= HPRatio) //HPRatio
    {
        vOutColor = (float4) float4(0.f, 1.f, 0.f, 1.f);
    }
    else
    {
        vOutColor = (float4) float4(1.f, 0.f, 0.f, 1.f);
    }
        
    
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        
        if (_in.vUV.x <= HPRatio)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
        else
            discard;
    }
    
     return vOutColor;
}
 

//	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(//L"material\\UIMtrl.mtrl");
//  Matrix tmp = m_vecCam[0] - > m_Orthographic;
//	pMainCam->GetViewMat()._11;
//	pMainCam->GetViewMat()._13;
//	pMainCam->GetViewMat()._31;
//	pMainCam->GetViewMat()._33;
//  Matrix b = { };
//	b._11 = pMainCam->GetViewMat()._11;
//	b._13 = pMainCam->GetViewMat()._13;
//	b._31 = pMainCam->GetViewMat()._31;
//	b._33 = pMainCam->GetViewMat()._33;
//Matrix c = XMMatrixInverse(nullptr, b);

//	pMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, &tmp);
//	pMtrl->SetScalarParam(SCALAR_PARAM::MAT_1, &c);
//  y축 빌보드 행렬 구하는 공식

#endif