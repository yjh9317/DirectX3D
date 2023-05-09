#include "pch.h"
#include "CHDRFirstPass.h"

CHDRFirstPass::CHDRFirstPass()
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	m_iGroupX = (UINT)ceil((float)(vResolution.x * vResolution.y) / (16 * 1024));
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

CHDRFirstPass::~CHDRFirstPass()
{
}


void CHDRFirstPass::UpdateData()
{
	//static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// Resolution of down scale (Res : Back buffer width and height divided by 4)
	m_Param.v2Arr[0] = Vec2(vResolution.x / 4.f, vResolution.y / 4.f);

	// Total pixel in the downscaled image ( Domain :  Back buffer width times height divided by 16)
	m_Param.iArr[0] = (vResolution.x * vResolution.y) / 16;

	// Number of groups dispached on the first pass ( GroupSize : Back buffer width times height divided by 16 times 1024)
	m_Param.iArr[1] = (UINT)ceil((float)(vResolution.x * vResolution.y) / (16 * 1024));

	m_pTex->UpdateData_CS(0, true);
	m_pBuffer->UpdateData_CS(0, false);
}

void CHDRFirstPass::Clear()
{
	m_pTex->Clear(0);
	m_pBuffer->Clear();
}

