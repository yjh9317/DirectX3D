#include "pch.h"
#include "CHDRScript.h"

#include <Engine/CHDRShader.h>
#include <Engine/CHDRFirstPass.h>

CHDRScript::CHDRScript()
	:CScript((int)SCRIPT_TYPE::HDRSCRIPT)
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	m_FirstBuffer = new CStructuredBuffer;
	m_FirstBuffer->Create(sizeof(float), (UINT)ceil((vResolution.x * vResolution.y) / (16 * 1024)), SB_TYPE::READ_WRITE, true, nullptr);

	m_SecondBuffer = new CStructuredBuffer;
	m_SecondBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);


}

CHDRScript::~CHDRScript()
{
	SAFE_DELETE(m_FirstBuffer);
	SAFE_DELETE(m_SecondBuffer);

}

void CHDRScript::update()
{
	Ptr<CHDRFirstPass> pFirstShader = (CHDRFirstPass*)CResMgr::GetInst()->FindRes<CComputeShader>(L"DownScaleFirstPassShader").Get();
	Ptr<CHDRShader> pUpdateShader2 = (CHDRShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"DownScaleSecondPassShader").Get();

	
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	Ptr<CTexture> ppTex = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");

	pFirstShader->SetBuffer(m_FirstBuffer);
	pFirstShader->SetTex(ppTex);
	pFirstShader->Excute();

	pUpdateShader2->SetFirstBuffer(m_FirstBuffer);
	pUpdateShader2->SetSecondBuffer(m_SecondBuffer);
	pUpdateShader2->Excute();

	float a = 1.6f;
	float b = 3.06f;

	float g_fMiddleGreyMax = 6.0;
	float g_fMiddleGrey = 0.863f;
	float g_fWhiteMax = 6.0f;
	float g_fWhite = 1.53f;

	g_fWhite *= g_fMiddleGrey;
	g_fWhite *= g_fWhite;

	float f = 0;
	m_SecondBuffer->GetData(&f);
	//m_SecondBuffer->UpdateData(PIPELINE_STAGE::PS, 23);
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial(0);
	
	MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0 ,&g_fMiddleGrey);
	MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &g_fWhite);
	MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_2, &f);

	


	float LScale = Vec3(0.5, 0.5, 0.5).Dot(Vec3(0.299, 0.587, 0.114));
	LScale *= g_fMiddleGrey / f; //AvgLum[0];
	LScale = (LScale + LScale * LScale / g_fWhite) / (1.0 + LScale);
	Vec3 vv = Vec3(0.5, 0.5, 0.5) * LScale;
	int aaaa = 0;
	

	//float ConvertDepthToLinear(float depth)
	//{
	//	float linearDepth = PerspectiveValues.z / (depth +
	//		PerspectiveValues.w);
	//	return linearDepth;
	//}
}

